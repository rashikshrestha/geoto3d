#include "nccarve.h"

#include "ncfbo.h"
#include "ncqtprogram.h"

int          ncCarve::refcount      = 0;
ncProgram*   ncCarve::vox_prog      = 0;
ncProgram*   ncCarve::color_prog    = 0;

ncCarve::ncCarve(ncVoxTree* vtree_  , ncQtConfig3D* cfg_) {

    vtree       = vtree_;
    gvm         = 0;
    cfg         = cfg_;
    init();
}

ncCarve::ncCarve(ncGVM* gvm_  , ncQtConfig3D* cfg_) {
    vtree       = 0;
    gvm         = gvm_;
    cfg         = cfg_;
    init();
}

ncCarve::~ncCarve() {
    delete vPBO;
    delete pPBO;

    refcount--;

    if (refcount == 0)
    {
        fprintf(stderr,"DEinitializing shaders...\n");
        delete vox_prog;
        delete color_prog;
    }
}

void ncCarve::init() {

    vPBO        = new ncPBO;
    pPBO        = new ncPBO;

    sprintf(mystr,"mkdir -p \"%s\"",cfg->getDirName(GVM_DIR).c_str());
    system(mystr);
    sprintf(mystr,"mkdir -p \"%s\"",cfg->getDirName(PLY_DIR).c_str());
    system(mystr);

    if (refcount == 0)
    {
        fprintf(stderr,"initializing shaders...\n");
        vox_prog    = new ncQtProgram(":/shaders/carve/voxel.glsl,:/shaders/carve/voxel.vert",":/shaders/carve/voxel.glsl,:/shaders/carve/voxel.geom",":/shaders/carve/voxel.glsl,:/shaders/carve/voxel.frag");
        color_prog  = new ncQtProgram(":/shaders/gvm.vert",":/shaders/gvm.geom",":/shaders/gvm.glsl,:/shaders/carve/vcolor.frag");
    }
    refcount++;
}

void ncCarve::run() {

    fprintf(stderr,"Carving...\n");

    ncPtr<ncTexImage2D> zTexFBO;
    int w,h,lod;
    ncImageBuffer<ncc::v2f> buf;

    cfg->initProgress();

    vtree->subs.clear();
    vtree->LRU.clear();

	std::vector<int> cids = cfg->getActiveCids();

    for (unsigned int cc = 0 ; cc<cids.size();++cc)
    {
        int cid = cids[cc];
        ncIDList id_list = cfg->createIDList(cid);
        double zfar = cfg->project.cameras.zfar[cid];

        //id_list.print();
        int start_id    = 0;
        int end_id      = id_list.size()-1;

        if (id_list.size()>start_id)
        {
            ncTexInfo texinfo;

            cfg->setProgress(id_list[start_id].cid , id_list[start_id].vid);
            cfg->loadImage<ncc::v2f>(DEPTH_CP,zTexFBO , id_list[start_id].cid , id_list[start_id].vid , texinfo);

            process(zTexFBO, texinfo.KGL(), texinfo.MV() , zfar );

            for (unsigned int i=start_id+1;i<=end_id;++i)
            {
                cfg->setProgress(id_list[i].cid , id_list[i].vid);
                cfg->loadImage<ncc::v2f>(DEPTH_CHECK_CP,zTexFBO , id_list[i].cid , id_list[i].vid , texinfo);
                process(zTexFBO, texinfo.KGL() , texinfo.MV() , zfar );
            }
        }
    }

    zTexFBO.reset();

    updateSignal();

    cfg->exitProgress();
}

void ncCarve::process(ncTexImage2D* im, ncc::m4x4d KGL, ncc::RTd MV , double zfar) {
    //fprintf(stderr,"+process()\n");

    std::vector<ncc::v3i> sub_list = vtree->getSubLevelsAtCameraPosition(MV[3].xyz());

    MV = vtree->getTransform().inverse()*MV;
    im->setFilters(GL_LINEAR , GL_LINEAR);

    ncc::v3d    campos  = MV[3].xyz();
    ncc::m4x4d  MVP     = KGL*MV.inverse();
    ncc::m4x4d  Ktex    = ncc::PGLtoPTEX(KGL);
    ncc::m4x4d  Ktexi   = Ktex.inverse();

    int upscale = cfg->vox.upscale;

//#define MSTEX

#ifdef MSTEX
    ncTexImageMultisample2D* mytex = new ncTexImageMultisample2D(GL_RGBA8, (im->width)<<upscale,(im->height)<<upscale , 1 );
#else
    ncTexImage2D* mytex = new ncTexImage2D(GL_RGBA8,(im->width)<<upscale,(im->height)<<upscale);
#endif

    ncc::v2f tc_scale(1.0f/mytex->width,1.0f/mytex->height);

    vox_prog->Activate();
    vox_prog->setUniform("MVP"      , MVP);
    vox_prog->setUniform("Ktex"     , Ktex);
    vox_prog->setUniform("Ktexi"    , Ktexi);
    vox_prog->setUniform("roi"      , zfar);
    vox_prog->setUniform("campos"   , campos);
    vox_prog->setUniform("tc_scale" , tc_scale);
    vox_prog->setSampler("dTex"     , im , 0);


    //! check voxels

    ncgl::FBO1->bind(GL_FRAMEBUFFER);
    ncgl::FBO1->attach(GL_COLOR_ATTACHMENT0 ,mytex);
    glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
    glEnable(GL_DEPTH_CLAMP);
    glDisable(GL_CULL_FACE);

#ifdef MSTEX
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_SAMPLE_SHADING);
    glMinSampleShading(1.0f);
#endif

    glViewport(0,0,mytex->width,mytex->height);

    proc_src    = new std::vector<ncVoxTreeIterator>;
    proc_dst    = new std::vector<ncVoxTreeIterator>;

    for (unsigned int i=0;i<sub_list.size();++i)
    {
        unsigned int k = vtree->addSubLevel(sub_list[i]);

        ncVoxTreeIterator   tmp = vtree->subs[k];
        ncSubVoxTree*       vt  = (ncSubVoxTree*)vtree->voxels.getValue<NC_PTR>(tmp.id).value;

        //fprintf(stderr,"test2 = %p\n",vt);

        vox_prog->setUniform("minlevel" , vt->minlevel);
        vox_prog->setUniform("maxlevel" , vt->maxlevel);

        proc_src->clear();
        proc_src->push_back(ncVoxTreeIterator(0,tmp.pos));

        while ((proc_src->size() > 0))
            update(vt);
    }

    delete proc_src;
    delete proc_dst;

#ifdef MSTEX
    glDisable(GL_SAMPLE_SHADING);
    glDisable(GL_MULTISAMPLE);
#endif

    glDisable(GL_DEPTH_CLAMP);
    glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
    ncgl::FBO1->detach(GL_COLOR_ATTACHMENT0);
    ncgl::FBO1->unbind(GL_FRAMEBUFFER);

    delete mytex;

    //fprintf(stderr,"vox = %4d MB | mem_pool = %4d MB | total = %4d MB ...\n", ((vtree->voxels.size()-memory_pool.size()*8)*sizeof(ncCarveVoxel))>>20 , (memory_pool.size()*8*sizeof(ncCarveVoxel))>>20 , (voxels.size()*sizeof(ncCarveVoxel))>>20 );
    unsigned int max_mem = (1<<12); //MB
    cfg->vox_mem->setRange(0,max_mem);
    //cfg->vox_mem->setValue(0,double(((vtree->voxels.size()-vtree->memory_pool.size()*8)*vtree->voxels.getItemSize())>>20)/max_mem);
    //cfg->vox_mem->setValue(1,double((vtree->memory_pool.size()*8*vtree->voxels.getItemSize())>>20)/max_mem);
    cfg->vox_mem->update();

    //ncgl::checkErrors();
    //ncgl::checkFBO();

    updateSignal();

    //fprintf(stderr,"-process()\n");



    /*
    fprintf(stderr,"    resident =");
    for ( std::list<ncVoxTreeIterator>::iterator it=resident.begin() ;it != resident.end(); it++ )
        fprintf(stderr," %d",it->id);
    fprintf(stderr,"\n");
    fprintf(stderr,"non_resident =");
    for ( std::list<ncVoxTreeIterator>::iterator it=non_resident.begin() ;it != non_resident.end(); it++ )
        fprintf(stderr," %d",it->id);
    fprintf(stderr,"\n");
    */
}

void ncCarve::update(ncSubVoxTree* vt) {
    //fprintf(stderr,"+update()\n");

    //fprintf(stderr,"updating %12d voxels...\n",(unsigned int)proc_src->size());

    unsigned int count = proc_src->size();

    if (count > vindex.size())
    {
        vindex.resize(count);
        vPBO->setup(count,&vindex[0]);
    }
    vox_prog->setUniform("vPBO", vPBO, GL_WRITE_ONLY);


    pPBO->setup(count,&((*proc_src)[0]));
    pPBO->bind(GL_ARRAY_BUFFER);
    glVertexAttribIPointer((GLuint)0, 4, GL_INT, sizeof(ncVoxTreeIterator), 0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_POINTS,0,count);
    glDisableVertexAttribArray(0);
    pPBO->unbind(GL_ARRAY_BUFFER);

    //! test vIndex
    vPBO->bind(GL_ARRAY_BUFFER);
    glMakeBufferNonResidentNV(GL_ARRAY_BUFFER);
    glGetBufferSubData(GL_ARRAY_BUFFER, 0, count*sizeof(int), &vindex[0]);
    vPBO->unbind(GL_ARRAY_BUFFER);

    proc_dst->clear();

    for (unsigned int i=0; i<count; ++i)
    {
        bool front = (vindex[i]&0x1 ? true : false);
        bool back  = (vindex[i]&0x2 ? true : false);
        bool edge  = (vindex[i]&0x4 ? true : false);
        bool camp  = (vindex[i]&0x8 ? true : false);
        ncVoxTreeIterator vproc = (*proc_src)[i];

        //vproc.print("vproc");
        //campos.print("campos");

        if (camp || (front && back) || (front && edge))
        {
            if ((vproc.pos.w > vt->minlevel) && (vt->voxels.getValue<NC_CARVE>(vproc.id) < vt->maxcarve))
            {
                vt->split(vproc.id , vproc.pos.w);
                int offset = 1<<(vproc.pos.w-1);
                for (unsigned int c=0; c<8; ++c)
                {
                    unsigned int vox_ = vt->voxels.getValue<NC_CHILD>(vproc.id)+c;
                    ncc::v4i    pos_ = ncc::v4i(vproc.pos.x + ( c&0x1 ? offset : 0),vproc.pos.y + ( c&0x2 ? offset : 0),vproc.pos.z + ( c&0x4 ? offset : 0),vproc.pos.w-1);

                    proc_dst->push_back(ncVoxTreeIterator(vox_,pos_));
                }
            }
        }
        else
        {
            if (front && (!back))
            {
                vt->incCarve(vproc.id,vproc.pos.w);
            }
        }
    }

    ncc::swap(proc_src,proc_dst);

    //fprintf(stderr,"-update()\n");
}

void ncCarve::createColors(ncTexImage2D* cTex, ncc::m4x4d KGL , ncc::RTd MV , double zfar) {
    glClearDepth(1.0f);

    gvm->setCameraPosition(KGL,MV,cTex->width,cTex->height,GL_CW);

    for ( std::list<unsigned int>::iterator it = gvm->LRU.begin() ;  it!=gvm->LRU.end() ; ++it)
    {
        ncVoxTreeIterator tmp = gvm->subs[*it];
        ncSubGVM* subgvm = ((ncSubGVM*)gvm->voxels.getValue<NC_PTR>(tmp.id).value);

        glClear(GL_DEPTH_BUFFER_BIT);

        ncc::RTd    MV_ = subgvm->getTransform().inverse()*MV;
        ncc::RTd    MVi = MV_.inverse();
        ncc::v3d campos = MV_[3].xyz();

        color_prog->Activate();
        color_prog->setUniform("campos"     ,campos);
        color_prog->setUniform("MVP"		,KGL*MVi);
        color_prog->setUniform("MVi"		,MVi);
        color_prog->setUniform("zfar"		,zfar);
        color_prog->setUniform("maxlevel"	,(int)subgvm->maxlevel);
        color_prog->setUniform("lod"	    ,(int)0);
        if (subgvm->voxels.data_mask&NC_CHILD)
        color_prog->setUniform("child" 	    ,subgvm->child_pbo, GL_READ_WRITE);
        if (subgvm->voxels.data_mask&NC_RGBM)
        color_prog->setUniform("rgbm" 	    ,subgvm->rgbm_pbo, GL_READ_WRITE);
        if (subgvm->voxels.data_mask&NC_COLOR)
        color_prog->setUniform("color" 	    ,subgvm->color_pbo, GL_READ_WRITE);
        color_prog->setSampler("cTex"       ,cTex,0);
        color_prog->setSampler("dTex"       ,gvm->dtex,1);

        subgvm->drawMesh(KGL,MV,GL_CW);

        color_prog->Deactivate();
    }
}

void ncCarve::createColors() {

     //! create textures
    ncPtr<ncTexImage2D> cTex;
    ncPtr<ncTexImage2D> dTex = new ncTexImage2D(GL_DEPTH_COMPONENT32F,64,64);

    fprintf(stderr,"\n");
   	std::vector<int> cids = cfg->getActiveCids();

    for (unsigned int cc = 0 ; cc < cids.size() ; ++cc)
    {
        ncIDList id_list = cfg->createIDList(cids[cc]);
        //unsigned int i = id_list.size()/2;
        for (unsigned int i = 0; i < id_list.size(); ++i)
        {
            unsigned int cid =   id_list[i].cid;
            unsigned int vid =   id_list[i].vid;

            std::string imfile = cfg->getImageFileName(cid,vid);

            double zfar = cfg->project.cameras.zfar[cid]/1.5;

            if (ncc::checkFile(imfile))
            {
                ncTexInfo texinfo;
                cfg->loadImage<ncc::v3ub>(COLOR_CP,cTex , cid,vid , texinfo);

                fprintf(stderr,"\rColoring %02d / %06d",cid,vid);

                ncc::v3d campos = texinfo.MV()[3].xyz();

                cTex->setFilters(GL_LINEAR,GL_LINEAR);
                dTex->resize(cTex->width,cTex->height);
                dTex->setFilters(GL_LINEAR,GL_LINEAR);

//#define MSTEX

/*
#ifdef MSTEX
                ncTexImageMultisample2D* fTex = new ncTexImageMultisample2D(GL_DEPTH_COMPONENT32F, dTex->width,dTex->height , 1);
              	glEnable(GL_MULTISAMPLE);
           		glEnable(GL_SAMPLE_SHADING);
                glMinSampleShading(1.0f);
#else
                ncTexImage2D* fTex = new ncTexImage2D(GL_DEPTH_COMPONENT32F, dTex->width,dTex->height);
#endif
*/

                //! draw depth
                ncgl::FBO1->bind(GL_FRAMEBUFFER);
                glDrawBuffer(GL_NONE);
                ncgl::FBO1->attach(GL_DEPTH_ATTACHMENT  ,dTex);

                glViewport(0,0,dTex->width,dTex->height);


                createColors(cTex,texinfo.KGL(),texinfo.MV() , zfar);


                ncgl::FBO1->detach(GL_DEPTH_ATTACHMENT);
                glDrawBuffer(GL_COLOR_ATTACHMENT0);
                ncgl::FBO1->unbind(GL_FRAMEBUFFER);

/*
#ifdef MSTEX
           		glDisable(GL_SAMPLE_SHADING);
                glDisable(GL_MULTISAMPLE);
#endif
                delete fTex;
*/

            }


        }

    }
    fprintf(stderr,"\n");


    //! cleanup
    cTex.reset();
}

void ncCarve::writePLY(const char* filename) {
    fprintf(stderr,"Writing %s...", filename);

    unsigned int nrvertex       = mesh_vert.size();
    unsigned int nrindices      = mesh_index.size();

    FILE* plyfile = fopen(filename,"wb");

    fprintf(plyfile,"ply\n");
    fprintf(plyfile,"format binary_little_endian 1.0\n");
    fprintf(plyfile,"comment GeoToVox generated\n");
    fprintf(plyfile,"element vertex %d\n",nrvertex);
    fprintf(plyfile,"property float x\n");
    fprintf(plyfile,"property float y\n");
    fprintf(plyfile,"property float z\n");
    fprintf(plyfile,"property uchar red\n");
    fprintf(plyfile,"property uchar green\n");
    fprintf(plyfile,"property uchar blue\n");
    fprintf(plyfile,"property uchar alpha\n");
    fprintf(plyfile,"element face %d\n",nrindices/3);
    fprintf(plyfile,"property list uchar int vertex_indices\n");
    fprintf(plyfile,"end_header\n");

    fwrite((char*)&(mesh_vert[0].x), sizeof(ncc::v3f4ub), nrvertex,plyfile);

    unsigned char tmp = 3;
    for (unsigned int i=0;i<nrindices/3;++i)
    {
        fwrite((char*)&tmp, sizeof(unsigned char), 1 ,plyfile);
        //fwrite((char*)&mesh_index[3*i], sizeof(unsigned int), 3 ,plyfile);
        fwrite((char*)&mesh_index[3*i+0], sizeof(unsigned int), 1 ,plyfile);
        fwrite((char*)&mesh_index[3*i+2], sizeof(unsigned int), 1 ,plyfile);
        fwrite((char*)&mesh_index[3*i+1], sizeof(unsigned int), 1 ,plyfile);
    }

    fclose(plyfile);


    ncc::RTd tf  = cfg->project.poses.poselist->getTransform();
    char ply_name_tr[1024];
    sprintf(ply_name_tr,"%s.tr",filename);

    std::ofstream ofile1(ply_name_tr,std::ios_base::out);
	ofile1.setf(std::ios::fixed,std::ios::floatfield);
    ofile1 << tf[0][0] << " " << tf[1][0] << " " << tf[2][0] << " " << tf[3][0] << std::endl;
    ofile1 << tf[0][1] << " " << tf[1][1] << " " << tf[2][1] << " " << tf[3][1] << std::endl;
    ofile1 << tf[0][2] << " " << tf[1][2] << " " << tf[2][2] << " " << tf[3][2] << std::endl;
    ofile1 << tf[0][3] << " " << tf[1][3] << " " << tf[2][3] << " " << tf[3][3] << std::endl;
    ofile1.close();



    fprintf(stderr,"done\n");
}

void ncCarve::getMesh(int levelT_) {
    //sprintf(mystr,cfg-PLY_CP.c_str(),0,0,0);
    //getMesh( 0 , ncc::v4i(0,0,0,vtree->maxlevel) , mystr , levelT_);
}

void ncCarve::getMesh(unsigned int vox, ncc::v4i pos , const char* filename, int levelT_) {
    levelT  = levelT_;

    fprintf(stderr,"Creating mesh...\n");
    mesh_vert.clear();
    mesh_index.clear();

    updateMesh(vox,pos);

    fprintf(stderr,"Vertices size = %5ld MB (%ld)\n",mesh_vert.size() *sizeof(ncc::v4i   ) >> 20 , mesh_vert.size());
    fprintf(stderr,"Indices  size = %5ld MB (%ld)\n",mesh_index.size()*sizeof(unsigned int) >> 20 , mesh_index.size());

    optimizeMesh();

    fprintf(stderr,"Vertices size = %5ld MB (%ld)\n",mesh_vert.size() *sizeof(ncc::v4i   ) >> 20 , mesh_vert.size());
    fprintf(stderr,"Indices  size = %5ld MB (%ld)\n",mesh_index.size()*sizeof(unsigned int) >> 20 , mesh_index.size());

    //! convert 2 float
    fprintf(stderr,"Converting mesh...");

    //for (unsigned int i=0; i<mesh_vert.size(); ++i)
    //    mesh_vert[i].color() = ncc::v4ub(0,0,255,255); //!blue

    writePLY(filename);
    fprintf(stderr,"done\n");
}

void ncCarve::updateMesh(unsigned int vox, ncc::v4i pos) {
    //fprintf(stderr,"updateMesh( %d, ncc::v4i ( %X , %X , %X , %X ))\n",vox, pos.x,pos.y,pos.z,pos.w);

    if (vtree->voxels.getValue<NC_CHILD>(vox) && (pos.w>levelT))
    {
        int offset = 1<<(pos.w-1);
        for (unsigned int i=0; i<8; ++i)
            if (vtree->voxels.getValue<NC_CARVE>(vox)&(1<<i))
                updateMesh(vtree->voxels.getValue<NC_CHILD>(vox)+i,ncc::v4i(pos.x + ( i&0x1 ? offset : 0),pos.y + ( i&0x2 ? offset : 0),pos.z + ( i&0x4 ? offset : 0),pos.w-1));
    }
    else
        addVox(pos, NEG_X|POS_X|NEG_Y|POS_Y|NEG_Z|POS_Z );
}

template <class T>
struct vptr
{
    unsigned int    index;
    T*              ptr;
};

bool LVertex( const vptr<ncc::v3f4ub> & left, const vptr<ncc::v3f4ub>& right ) {
    return (left.ptr->pos() < right.ptr->pos());
}

bool EQVertex( const vptr<ncc::v3f4ub> & left, const vptr<ncc::v3f4ub>& right ) {
    return (left.ptr->pos() == right.ptr->pos());
}

void ncCarve::optimizeMesh() {

    fprintf(stderr,"Optimizing mesh...\n");

    if (mesh_vert.size()>1)
    {
        unsigned int* index_map = new unsigned int[mesh_vert.size()];

        //! remove duplicate vertices

        std::vector< vptr<ncc::v3f4ub > > plist;
        plist.resize(mesh_vert.size());
        for (unsigned int i=0;i<mesh_vert.size();++i)
        {
            plist[i].index  = i;
            plist[i].ptr    = &mesh_vert[i];
        }
        std::sort(plist.begin(), plist.end(),LVertex);

        unsigned int p2 = 0;

        while (p2<plist.size())
        {
            unsigned int p1 = p2;
            ncc::v4f wcolor = ncc::v4f(0.0f,0.0f,0.0f,0.0f);
            do
            {
                wcolor += plist[p2].ptr->color();
                index_map[plist[p2].index]  = plist[p1].index;
                p2++;
            }
            while ((p2<plist.size()) && (EQVertex(plist[p2] , plist[p1])));

            if (wcolor.w>0.0f)
                plist[p1].ptr->color() = (255.0f/wcolor.w)*wcolor;
            else
                plist[p1].ptr->color() = ncc::v4ub(0,0,0,0);
        }

        for (unsigned int i=0; i<mesh_index.size(); ++i)
            mesh_index[i] = index_map[mesh_index[i]];


        //! remove unused vertices

        for (unsigned int i=0;i<mesh_vert.size();++i)
            index_map[i] = 0;
        for (unsigned int i=0;i<mesh_index.size();++i)
            index_map[mesh_index[i]] = 1;

        int wp = 0;
        for (unsigned int i=0;i<mesh_vert.size();++i)
        {
            if (index_map[i])
            {
                index_map[i]    = wp;
                mesh_vert[wp]   = mesh_vert[i];
                wp++;
            }
        }
        mesh_vert.resize(wp);

        for (unsigned int i=0;i<mesh_index.size();++i)
            mesh_index[i] = index_map[mesh_index[i]];

        //! cleanup
        delete[] index_map;
    }

}

void ncCarve::addVox(ncc::v4i pos, unsigned int locmask) {
    //fprintf(stderr,"addVox(ncc::v4i ( %X , %X , %X , %X ) , %X)\n",pos.x,pos.y,pos.z,pos.w,locmask);

    if (pos.w>levelT)
        addLargeVox(pos,locmask);
    else
        addSmallVox(pos,locmask);
}

void ncCarve::addSmallVox(ncc::v4i pos, unsigned int locmask) {
    //fprintf(stderr,"addSmallVox(ncc::v4i ( %X , %X , %X , %X ) , %X)\n",pos.x,pos.y,pos.z,pos.w,locmask);

    //! get valuemasks
    unsigned int    vmask = 0;
    unsigned int    vmask_cur = 0;

    getValue(vmask_cur , pos);

    if ((vmask_cur&0x1))
    {
        int             offset = 1<<(pos.w);

        int    maxx = 1<<vtree->maxlevel;
        int    maxy = 1<<vtree->maxlevel;
        int    maxz = 1<<vtree->maxlevel;

        if (pos.x == 0)
            locmask&=~(NEG_X);
        if (pos.x+offset >= maxx)
            locmask&=~(POS_X);

        if (pos.y == 0)
            locmask&=~(NEG_Y);
        if (pos.y+offset >= maxy)
            locmask&=~(POS_Y);

        if (pos.z == 0)
            locmask&=~(NEG_Z);
        if (pos.z+offset >= maxz)
            locmask&=~(POS_Z);

        unsigned char vert_used = 0x0;
        unsigned char face_used = 0x0;

        if (locmask&NEG_X)
        {
            getValue(vmask , ncc::v4i(pos.x-offset    , pos.y         , pos.z         , pos.w));
            if (!(vmask&0x1))
            {
                face_used|=NEG_X;
                vert_used|=0x55;
            }
        }
        if (locmask&POS_X)
        {
            getValue(vmask , ncc::v4i(pos.x+offset    , pos.y         , pos.z         , pos.w));
            if (!(vmask&0x1))
            {
                face_used|=POS_X;
                vert_used|=0xAA;
            }
        }
        if (locmask&NEG_Y)
        {
            getValue(vmask , ncc::v4i(pos.x    , pos.y-offset         , pos.z         , pos.w));
            if (!(vmask&0x1))
            {
                face_used|=NEG_Y;
                vert_used|=0x33;
            }
        }
        if (locmask&POS_Y)
        {
            getValue(vmask , ncc::v4i(pos.x    , pos.y+offset         , pos.z         , pos.w));
            if (!(vmask&0x1))
            {
                face_used|=POS_Y;
                vert_used|=0xCC;
            }
        }
        if (locmask&NEG_Z)
        {
            getValue(vmask , ncc::v4i(pos.x    , pos.y         , pos.z-offset         , pos.w));
            if (!(vmask&0x1))
            {
                face_used|=NEG_Z;
                vert_used|=0x0F;
            }
        }
        if (locmask&POS_Z)
        {
            getValue(vmask , ncc::v4i(pos.x    , pos.y         , pos.z+offset         , pos.w));
            if (!(vmask&0x1))
            {
                face_used|=POS_Z;
                vert_used|=0xF0;
            }
        }

        unsigned int vindex = mesh_vert.size();
        unsigned int vmap[8];
        int count = 0;
        for (unsigned int i=0;i<8;++i)
            if (vert_used&(1<<i))
            {
                vmap[i] = vindex+count;
                ++count;
                ncc::v3f tmp = pos2vert(ncc::v4i( (i&0x1 ? pos.x+offset : pos.x) , (i&0x2 ? pos.y+offset : pos.y) , (i&0x4 ? pos.z+offset : pos.z) ,pos.w));
                mesh_vert.push_back(ncc::v3f4ub(tmp , ncc::v4ub((vmask_cur>>24)&0xFF , (vmask_cur>>16)&0xFF , (vmask_cur>>8)&0xFF,0xFF)));
            }

        if (face_used&NEG_X)
        {
            mesh_index.push_back(vmap[4]);
            mesh_index.push_back(vmap[0]);
            mesh_index.push_back(vmap[2]);
            mesh_index.push_back(vmap[6]);
            mesh_index.push_back(vmap[4]);
            mesh_index.push_back(vmap[2]);
        }
        if (face_used&POS_X)
        {
            mesh_index.push_back(vmap[3]);
            mesh_index.push_back(vmap[1]);
            mesh_index.push_back(vmap[7]);
            mesh_index.push_back(vmap[1]);
            mesh_index.push_back(vmap[5]);
            mesh_index.push_back(vmap[7]);
        }
        if (face_used&NEG_Y)
        {
            mesh_index.push_back(vmap[1]);
            mesh_index.push_back(vmap[0]);
            mesh_index.push_back(vmap[4]);
            mesh_index.push_back(vmap[5]);
            mesh_index.push_back(vmap[1]);
            mesh_index.push_back(vmap[4]);
        }
        if (face_used&POS_Y)
        {
            mesh_index.push_back(vmap[7]);
            mesh_index.push_back(vmap[2]);
            mesh_index.push_back(vmap[3]);
            mesh_index.push_back(vmap[7]);
            mesh_index.push_back(vmap[6]);
            mesh_index.push_back(vmap[2]);
        }
        if (face_used&NEG_Z)
        {
            mesh_index.push_back(vmap[2]);
            mesh_index.push_back(vmap[0]);
            mesh_index.push_back(vmap[1]);
            mesh_index.push_back(vmap[1]);
            mesh_index.push_back(vmap[3]);
            mesh_index.push_back(vmap[2]);
        }
        if (face_used&POS_Z)
        {
            mesh_index.push_back(vmap[5]);
            mesh_index.push_back(vmap[4]);
            mesh_index.push_back(vmap[6]);
            mesh_index.push_back(vmap[7]);
            mesh_index.push_back(vmap[5]);
            mesh_index.push_back(vmap[6]);
        }
    }
}

void ncCarve::addLargeVox(ncc::v4i pos, unsigned int locmask) {
    //fprintf(stderr,"addLargeVox(ncc::v4i ( %X , %X , %X , %X ) , %X)\n", pos.x,pos.y,pos.z,pos.w,locmask);

    unsigned int    vmask   = 0;
    int             offset1 = 1<<(pos.w);
    int             offset  = 1<<(pos.w-1);

    getValue(vmask , pos);
    if ((vmask&0x1))
    {
        if (locmask&NEG_X)
        {
            bool split = getValue(vmask , ncc::v4i(pos.x-offset1          ,pos.y          ,pos.z          ,pos.w));
            if (split)
            {
                addVox(ncc::v4i(pos.x          ,pos.y          ,pos.z          ,pos.w-1), NEG_X);
                addVox(ncc::v4i(pos.x          ,pos.y+offset   ,pos.z          ,pos.w-1), NEG_X);
                addVox(ncc::v4i(pos.x          ,pos.y          ,pos.z+offset   ,pos.w-1), NEG_X);
                addVox(ncc::v4i(pos.x          ,pos.y+offset   ,pos.z+offset   ,pos.w-1), NEG_X);
            }
            else
                addSmallVox(pos, NEG_X);
        }

        if (locmask&POS_X)
        {
            bool split = getValue(vmask , ncc::v4i(pos.x+offset1          ,pos.y          ,pos.z          ,pos.w));
            if (split)
            {
                addVox(ncc::v4i(pos.x+offset   ,pos.y          ,pos.z          ,pos.w-1), POS_X);
                addVox(ncc::v4i(pos.x+offset   ,pos.y+offset   ,pos.z          ,pos.w-1), POS_X);
                addVox(ncc::v4i(pos.x+offset   ,pos.y          ,pos.z+offset   ,pos.w-1), POS_X);
                addVox(ncc::v4i(pos.x+offset   ,pos.y+offset   ,pos.z+offset   ,pos.w-1), POS_X);
            }
            else
                addSmallVox(pos, POS_X);
        }

        if (locmask&NEG_Y)
        {
            bool split = getValue(vmask , ncc::v4i(pos.x          ,pos.y-offset1          ,pos.z          ,pos.w));
            if (split)
            {
                addVox(ncc::v4i(pos.x          ,pos.y          ,pos.z          ,pos.w-1), NEG_Y);
                addVox(ncc::v4i(pos.x+offset   ,pos.y          ,pos.z          ,pos.w-1), NEG_Y);
                addVox(ncc::v4i(pos.x          ,pos.y          ,pos.z+offset   ,pos.w-1), NEG_Y);
                addVox(ncc::v4i(pos.x+offset   ,pos.y          ,pos.z+offset   ,pos.w-1), NEG_Y);
            }
            else
                addSmallVox(pos, NEG_Y);

        }

        if (locmask&POS_Y)
        {
            bool split = getValue(vmask , ncc::v4i(pos.x          ,pos.y+offset1          ,pos.z          ,pos.w));
            if (split)
            {
                addVox(ncc::v4i(pos.x          ,pos.y+offset   ,pos.z          ,pos.w-1), POS_Y);
                addVox(ncc::v4i(pos.x+offset   ,pos.y+offset   ,pos.z          ,pos.w-1), POS_Y);
                addVox(ncc::v4i(pos.x          ,pos.y+offset   ,pos.z+offset   ,pos.w-1), POS_Y);
                addVox(ncc::v4i(pos.x+offset   ,pos.y+offset   ,pos.z+offset   ,pos.w-1), POS_Y);
            }
            else
                addSmallVox(pos, POS_Y);

        }

        if (locmask&NEG_Z)
        {
            bool split =  getValue(vmask , ncc::v4i(pos.x          ,pos.y          ,pos.z-offset1          ,pos.w));
            if (split)
            {
                addVox(ncc::v4i(pos.x          ,pos.y          ,pos.z          ,pos.w-1), NEG_Z);
                addVox(ncc::v4i(pos.x+offset   ,pos.y          ,pos.z          ,pos.w-1), NEG_Z);
                addVox(ncc::v4i(pos.x          ,pos.y+offset   ,pos.z          ,pos.w-1), NEG_Z);
                addVox(ncc::v4i(pos.x+offset   ,pos.y+offset   ,pos.z          ,pos.w-1), NEG_Z);
            }
            else
                addSmallVox(pos, NEG_Z);
        }

        if (locmask&POS_Z)
        {
            bool split =  getValue(vmask , ncc::v4i(pos.x          ,pos.y          ,pos.z+offset1          ,pos.w));
            if (split)
            {
                addVox(ncc::v4i(pos.x          ,pos.y          ,pos.z+offset   ,pos.w-1), POS_Z);
                addVox(ncc::v4i(pos.x+offset   ,pos.y          ,pos.z+offset   ,pos.w-1), POS_Z);
                addVox(ncc::v4i(pos.x          ,pos.y+offset   ,pos.z+offset   ,pos.w-1), POS_Z);
                addVox(ncc::v4i(pos.x+offset   ,pos.y+offset   ,pos.z+offset   ,pos.w-1), POS_Z);
            }
            else
                addSmallVox(pos, POS_Z);
        }
    }
}

ncc::v3f ncCarve::pos2vert(ncc::v4i pos) {
    ncc::v3f out;
    out.x = 0.001f*(pos.x);
    out.y = 0.001f*(pos.y);
    out.z = 0.001f*(pos.z);
    return out;
}

bool ncCarve::getValue(unsigned int& vmask, ncc::v4i pos) {
    int level_ = vtree->maxlevel;
    return getValue(0,vmask,pos,level_);
}

bool ncCarve::getValue(unsigned int vox, unsigned int& vmask, ncc::v4i pos, int& level_) {
    vmask = vtree->voxels.getValue<NC_CARVE>(vox);

    if (vtree->voxels.getValue<NC_CHILD>(vox))
    {
        if (level_> std::max(pos.w,levelT))
        {
            level_--;
            int mask    = 1<<level_;
            int c = (((pos.z&mask)>>level_)<<2) | (((pos.y&mask)>>level_)<<1) | ((pos.x&mask)>>level_);
            return getValue(vtree->voxels.getValue<NC_CHILD>(vox)+c,vmask,pos,level_);
        }
        else
            return true;
    }
    else
        return false;
}
