#include "ncflow.h"
#include <algorithm>

int          ncFlow::refcount       = 0;
float        ncFlow::Gsigma         = 0;

ncPtr<ncProgram>   ncFlow::grayscale_prog;
ncPtr<ncProgram>   ncFlow::gauss_c1_X;
ncPtr<ncProgram>   ncFlow::gauss_c1_Y;
ncPtr<ncProgram>   ncFlow::gauss_c2_X;
ncPtr<ncProgram>   ncFlow::gauss_c2_Y;
ncPtr<ncProgram>   ncFlow::sub_X;
ncPtr<ncProgram>   ncFlow::sub_Y;
ncPtr<ncProgram>   ncFlow::delaunay_prog;
ncPtr<ncProgram>   ncFlow::edge_prog;
ncPtr<ncProgram>   ncFlow::draw_rg_prog;
ncPtr<ncProgram>   ncFlow::mesh_prog;
ncPtr<ncProgram>   ncFlow::gvm_prog;
ncPtr<ncProgram>   ncFlow::bp_sweep;
ncPtr<ncPBO>       ncFlow::gFactPBO;
ncPtr<ncPBO>       ncFlow::gFactPBO_RG;

ncPtr<ncProgram>   ncFlow::laplace_prog;
ncPtr<ncProgram>   ncFlow::carve_prog;
ncPtr<ncProgram>   ncFlow::cd_sweep;
ncPtr<ncProgram>   ncFlow::mdl_sweep;

ncFlow::ncFlow(ncQtConfig3D* cfg_) {

    cfg     = cfg_;
    imw     = imh   = 0;
    lodw    = lodh  = imlod     = 0;
    winw    = winh  = wind      = winlod    = 0;

    zTexFBO_resource= 0;
    zTexFBO_array   = 0;

    nrim    = 2*cfg->flow.imrange+1;
    texinfo.resize(nrim);
    Ptex.resize(nrim);
    PtexPBO = new ncPBO;
    Zlut.resize(cfg->flow.winD);
    ZlutPBO = new ncPBO;

    if (refcount == 0)
    {
        bp_sweep        = new ncQtProgram(":/shaders/flow/sweep_common.vert",0,":/shaders/flow/sweep_common.frag,:/shaders/flow/bp_sweep.frag"      );
        grayscale_prog  = new ncQtProgram(":/shaders/flow/grayscale.vert"   ,0,":/shaders/flow/grayscale.frag"   );
        gauss_c1_X      = new ncQtProgram(":/shaders/flow/gauss_c1_X.vert"  ,0,":/shaders/flow/gauss_c1_X.frag"  );
        gauss_c1_Y      = new ncQtProgram(":/shaders/flow/gauss_c1_Y.vert"  ,0,":/shaders/flow/gauss_c1_Y.frag"  );
        gauss_c2_X      = new ncQtProgram(":/shaders/flow/gauss_c2_X.vert"  ,0,":/shaders/flow/gauss_c2_X.frag"  );
        gauss_c2_Y      = new ncQtProgram(":/shaders/flow/gauss_c2_Y.vert"  ,0,":/shaders/flow/gauss_c2_Y.frag"  );
        sub_X           = new ncQtProgram(":/shaders/flow/sub_X.vert"       ,0,":/shaders/flow/sub_X.frag"       );
        sub_Y           = new ncQtProgram(":/shaders/flow/sub_Y.vert"       ,0,":/shaders/flow/sub_Y.frag"       );
        edge_prog       = new ncQtProgram(":/shaders/flow/edge.vert"        ,0,":/shaders/flow/edge.frag"        );
        delaunay_prog   = new ncQtProgram(":/shaders/flow/delaunay.vert"    ,0,":/shaders/flow/delaunay.frag"    );
        draw_rg_prog    = new ncQtProgram(":/shaders/flow/draw_rg.vert"     ,0,":/shaders/flow/draw_rg.frag"     );
        mesh_prog       = new ncQtProgram(":/shaders/flow/mesh.vert"        ,0,":/shaders/flow/mesh.frag"        );
        gvm_prog        = new ncQtProgram(":/shaders/gvm.vert"              ,0,":/shaders/gvm.glsl,:/shaders/flow/gvminit.frag"                  );

        laplace_prog    = new ncQtProgram(":/shaders/flow/laplace.vert"     ,0,":/shaders/flow/laplace.frag"     );
        carve_prog      = new ncQtProgram(":/shaders/flow/carve.vert"       ,0,":/shaders/flow/carve.frag"       );
        cd_sweep        = new ncQtProgram(":/shaders/flow/sweep_common.vert",0,":/shaders/flow/sweep_common.frag,:/shaders/flow/cd_sweep.frag"          );
        mdl_sweep       = new ncQtProgram(":/shaders/flow/sweep_common.vert",0,":/shaders/flow/sweep_common.frag,:/shaders/flow/mdl_sweep.frag"         );

        gFactPBO_RG = new ncPBO;
        setupGauss(gFactPBO_RG,1.0f);
    }
    refcount++;
}

ncFlow::~ncFlow() {

    clearStorage();

    PtexPBO.reset();
    Ptex.clear();
    ZlutPBO.reset();
    Zlut.clear();
    texinfo.clear();

    refcount--;

    if (refcount==0)
    {
        bp_sweep.reset();
        grayscale_prog.reset();
        gauss_c1_X.reset();
        gauss_c1_Y.reset();
        gauss_c2_X.reset();
        gauss_c2_Y.reset();
        sub_X.reset();
        sub_Y.reset();
        edge_prog.reset();
        delaunay_prog.reset();
        draw_rg_prog.reset();
        mesh_prog.reset();
        gvm_prog.reset();

        gFactPBO_RG.reset();

        laplace_prog.reset();
        carve_prog.reset();
        cd_sweep.reset();
    }

}

void ncFlow::clearStorage() {

    gFactPBO.reset();
    fbo.reset();
    zTexFBO.reset();
    cTex.reset();
    zTex.reset();
    loadTex.reset();
    undistortTex.reset();
    tmpTex[0].reset();
    tmpTex[1].reset();
    pbo3D0.reset();
    pbo3D1.reset();
    pbo3D2.reset();

    imw = imh = winw = winh = imlod = 0;
}

void ncFlow::setupGauss(ncPBO* pbo, float sigma, bool even) {

    int     nrelements  = 1+4*sigma;
    float   tmp         = 2.0f*sigma*sigma;
    float* gdata = new float[nrelements];

    if (even)
    {
        for (int i=0;i<nrelements;++i)
            gdata[i] = (1.0/(sqrt(2.0*M_PI)*sigma))*exp(-((i+0.5)*(i+0.5)/tmp));

        float gsum = 0.0f;
        for (int i=0;i<nrelements;++i)
            gsum += 2.0f*gdata[i];

        for (int i=0;i<nrelements;++i)
            gdata[i]/=gsum;
    }
    else
    {
        for (int i=0;i<nrelements;++i)
            gdata[i] = (1.0/(sqrt(2.0*M_PI)*sigma))*exp(-(i*i/tmp));

        float gsum = gdata[0];
        for (int i=1;i<nrelements;++i)
            gsum += 2.0f*gdata[i];

        for (int i=0;i<nrelements;++i)
            gdata[i]/=gsum;
    }

    pbo->setup(nrelements,&gdata[0]);
    delete[] gdata;
}

void ncFlow::drawFSrect() {

    ncc::v4f tmp[4];
    tmp[0] = ncc::v4f(-1.0f,-1.0f,0.0f,1.0f);
    tmp[1] = ncc::v4f(+1.0f,-1.0f,0.0f,1.0f);
    tmp[2] = ncc::v4f(-1.0f,+1.0f,0.0f,1.0f);
    tmp[3] = ncc::v4f(+1.0f,+1.0f,0.0f,1.0f);

    glVertexAttribPointer((GLuint)0, 4, GL_FLOAT, GL_FALSE, 0, &tmp[0].x);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    glDisableVertexAttribArray(0);

}

void ncFlow::gaussC1(ncFBO* fbo, ncTexImage2D* src,ncTexImage2D* tmp, ncTexImage2D* dst,ncPBO* gfact, int lod) {

    fbo->bind(GL_FRAMEBUFFER);
    glViewport(0,0,src->width >> lod,src->height >> lod);


	//!x
    fbo->attach(GL_COLOR_ATTACHMENT0,tmp,lod);

    gauss_c1_X->Activate();
    gauss_c1_X->setUniform("gSize"  ,gfact->nrelements);
    gauss_c1_X->setUniform("gFact"  ,gfact, GL_READ_ONLY);
    gauss_c1_X->setUniform("lod"    ,lod);
    gauss_c1_X->setSampler("cTex"   ,src,0);

	drawFSrect();

	//!y
    fbo->attach(GL_COLOR_ATTACHMENT0 ,dst,lod);

    gauss_c1_Y->Activate();
    gauss_c1_Y->setUniform("gSize"  ,gfact->nrelements);
    gauss_c1_Y->setUniform("gFact"  ,gfact , GL_READ_ONLY);
    gauss_c1_Y->setUniform("lod"    ,lod);
    gauss_c1_Y->setSampler("cTex"   ,tmp,0);

    drawFSrect();

    fbo->detach(GL_COLOR_ATTACHMENT0);
    fbo->unbind(GL_FRAMEBUFFER);
}

void ncFlow::gaussC1Sub(ncFBO* fbo, ncTexImage2D* src,ncTexImage2D* tmp,ncPBO* gfact, int lod) {

    fbo->bind(GL_FRAMEBUFFER);

	//!x
    fbo->attach(GL_COLOR_ATTACHMENT0,tmp,lod);
    glViewport(0,0,src->width >> (lod+1),src->height >> lod);

    sub_X->Activate();
    sub_X->setUniform("gSize"  ,gfact->nrelements);
    sub_X->setUniform("gFact"  ,gfact, GL_READ_ONLY);
    sub_X->setUniform("lod"    ,lod);
    sub_X->setSampler("cTex"   ,src,0);

	drawFSrect();

	//!y
    fbo->attach(GL_COLOR_ATTACHMENT0 ,src,lod+1);
    glViewport(0,0,src->width >> (lod+1),src->height >> (lod+1));

    sub_Y->Activate();
    sub_Y->setUniform("gSize"  ,gfact->nrelements);
    sub_Y->setUniform("gFact"  ,gfact , GL_READ_ONLY);
    sub_Y->setUniform("lod"    ,lod);
    sub_Y->setSampler("cTex"   ,tmp,0);

    drawFSrect();

    fbo->detach(GL_COLOR_ATTACHMENT0);
    fbo->unbind(GL_FRAMEBUFFER);
}

void ncFlow::gaussC2(ncFBO* fbo, ncTexImage2D* src,ncTexImage2D* tmp, ncTexImage2D* dst,ncPBO* gfact, int lod) {

    fbo->bind(GL_FRAMEBUFFER);
    glViewport(0,0,src->width >> lod,src->height >> lod);


	//!x
    fbo->attach(GL_COLOR_ATTACHMENT0,tmp,lod);

    gauss_c2_X->Activate();
    gauss_c2_X->setUniform("gSize"  ,gfact->nrelements);
    gauss_c2_X->setUniform("gFact"  ,gfact, GL_READ_ONLY);
    gauss_c2_X->setUniform("lod"    ,lod);
    gauss_c2_X->setSampler("cTex"   ,src,0);

	drawFSrect();

	//!y
    fbo->attach(GL_COLOR_ATTACHMENT0 ,dst,lod);

    gauss_c2_Y->Activate();
    gauss_c2_Y->setUniform("gSize"  ,gfact->nrelements);
    gauss_c2_Y->setUniform("gFact"  ,gfact , GL_READ_ONLY);
    gauss_c2_Y->setUniform("lod"    ,lod);
    gauss_c2_Y->setSampler("cTex"   ,tmp,0);

    drawFSrect();

    fbo->detach(GL_COLOR_ATTACHMENT0);
    fbo->unbind(GL_FRAMEBUFFER);
}

void ncFlow::grayscale(ncTexImage2D* src, ncTexImage2D* dst, int lod) {

    fbo->bind(GL_FRAMEBUFFER);
    glViewport(0,0,dst->width,dst->height);

    fbo->attach(GL_COLOR_ATTACHMENT0,dst,0);

    grayscale_prog->Activate();
    grayscale_prog->setUniform("lod"    ,lod);
    grayscale_prog->setSampler("cTex"   ,src,0);

	drawFSrect();

    fbo->detach(GL_COLOR_ATTACHMENT0);
    fbo->unbind(GL_FRAMEBUFFER);
}

float getMedianValue(std::vector<float>& vec , float zrange) {

#if 0
    unsigned int    sz          = vec.size();
    float           meanval     = vec[vec.size()/2];

    if (meanval>0.99f)
        return meanval;
    else
    {

        float total_value   = 0.0f;
        float total_weight  = 0.0f;
        float value,weight;

        float gsigm = 0.05f;
        float gfact = -0.5f/(gsigm*gsigm);
        for (unsigned int i = 0; i<sz; ++i)
        {
            value         = vec[i];
            if ((value<0.99f) && (fabs(meanval-value)/((1.0f-meanval)*(1.0f-value)) < zrange))
            {
                weight        = 1.0f;
                total_value  += weight*value;
                total_weight += weight;
            }

        }

        return total_value/total_weight;
    }
#else

    return vec[vec.size()/2];

#endif
}

void ncFlow::medianC2(ncTexImage2D* rg_depth , int range , float zrange) {

    if (range>0)
    {
        int w = rg_depth->width;
        int h = rg_depth->height;

        unsigned int MT = 0;//(2*range+1)*(2*range+1)/2;

        ncc::v2f*      d_in  = new ncc::v2f[w*h];
        ncc::v2f*      d_out = new ncc::v2f[w*h];
        for (int i=0;i<w*h;++i)
            d_out[i] = ncc::v2f(0.0f,0.0f);

        rg_depth->bind(0);
        glGetTexImage(rg_depth->target ,0 ,GL_RG ,GL_FLOAT ,d_in);

        for (int i=0;i<w*h;++i)
            if (isnanf(d_in[i].x))
                fprintf(stderr,"BLEEEEEEEEEEEEEEEEEEEEEH %d %d\n",i/w,i%w);

        for (int p = 0;p<h;++p)
        {

            std::vector<float>* columns = new std::vector<float>[w];
            for (int x=0;x<w;++x)
            {
                columns[x].clear();

                for (int y=(std::max)(0,p-range);y<=(std::min)(p+range,h-1);++y)
                    if (d_in[y*w+x].y > 0.0f)
                        columns[x].push_back(d_in[y*w+x].x/d_in[y*w+x].y);

                std::sort(columns[x].begin(),columns[x].end());
            }

            std::vector<float> mdata1;
            std::vector<float> mdata2;

            std::vector<float>* mdata_in  = &mdata1;
            std::vector<float>* mdata_out = &mdata2;
            std::vector<float>* mdata_tmp;

            mdata_in->clear();
            for (int i=0;i<=range;++i)
                for (int j=0;j<(int)columns[i].size();++j)
                    mdata_in->push_back(columns[i][j]);
            std::sort(mdata_in->begin(),mdata_in->end());

            if (mdata_in->size()>MT)
                d_out[p*w] = ncc::v2f( getMedianValue(*mdata_in , zrange) , 1.0f);

            std::vector<float> dummy;

            std::vector<float>* c_in  = &columns[0];
            std::vector<float>* c_out = &columns[0];
            for (int x = 1;x<w;++x)
            {
                c_out = ( x-range-1 >= 0 ? &columns[x-range-1] : &dummy);
                c_in  = ( x+range    < w ? &columns[x+range  ] : &dummy);

                mdata_out->clear();

                int index_in  = 0;
                int index_out = 0;

                for (int i=0;i<(int)mdata_in->size();++i)
                {
                    float value = (*mdata_in)[i];
                    if ((index_out<(int)c_out->size()) && (value == (*c_out)[index_out]))
                        index_out++;
                    else
                    {
                        while((index_in<(int)c_in->size()) && (*c_in)[index_in]<value)
                            mdata_out->push_back((*c_in)[index_in++]);

                        mdata_out->push_back(value);
                    }
                }

                while(index_in<(int)c_in->size())
                     mdata_out->push_back((*c_in)[index_in++]);

                if (mdata_out->size()>MT)
                    d_out[p*w+x] = ncc::v2f(getMedianValue(*mdata_out , zrange),1.0f);

                mdata_tmp = mdata_in;
                mdata_in  = mdata_out;
                mdata_out = mdata_tmp;

            }

            delete[] columns;
        }

        rg_depth->bind(0);
        glTexSubImage2D(rg_depth->target ,0 ,0 ,0 ,w ,h ,GL_RG ,GL_FLOAT ,d_out);

        delete[] d_in;
        delete[] d_out;
    }
}

void ncFlow::setupPtexPBO(int index) {

    for (int i=0;i<nrim;++i)
        Ptex[i]  = texinfo[i].Ktex()*(texinfo[i].MV().inverse()*texinfo[ref].MV())*texinfo[ref].Ktex().inverse();

    PtexPBO->setup(nrim,&Ptex[0]);

}

void ncFlow::setupZlutPBO(int index) {

    int 	nrlin   = int(cfg->flow.linP*(cfg->flow.winD-1));
    float	Zn 		= cfg->project.cameras.znear[index];
    float	Zf      = cfg->project.cameras.zfar [index];

#if 1
   	for (int i=0;i<nrlin;++i)
	{
		float 	lrp 	= float(i)/(nrlin-1);
		float	realZ 	= (1.0f-lrp)*Zn + lrp*Zf;
		float 	winZ	= (realZ-Zn)/realZ;
		Zlut[i] 		= winZ;
		//fprintf(stderr,"%03d: %f (%f)\n",i,realZ,winZ);
	}

	for (int i=0;i<cfg->flow.winD-nrlin;++i)
	{
		float 	lrp 	= float(i)/(cfg->flow.winD-nrlin-1);
		float 	winZ	= (1.0f-lrp)*( (Zf-Zn)/Zf ) + lrp*1.0f;
		Zlut[nrlin+i]	= winZ;
		//float 	realZ	= Zn/(1.0f-winZ);
		//fprintf(stderr,"%03d: %f (%f)\n",nrlin+i,realZ,winZ);
	}
#else
   	for (int i=0;i<cfg->flow.winD;++i)
	{
		Zlut[i] 		= (i+0.5f)/cfg->flow.winD;
		//fprintf(stderr,"%03d: %f (%f)\n",i,realZ,winZ);
	}

#endif

    ZlutPBO->setup(Zlut.size(),&Zlut[0]);
}

extern "C" void beliefPropagation(cudaArray* my_array, float* idata0 , float* idata1 , float* idata2 , int w, int h , int d , float* zlut_dev);
extern "C" void normalizeCPU(float* dst , float* src , int w, int h, int d);
extern "C" void smoothXYCPU(float* dst , float* src , int w, int h, int d);
extern "C" void smoothZCPU(float* dst , float* src , int w, int h, int d);

void ncFlow::initBpSweep() {

    size_t num_bytes;

    cudaGraphicsGLRegisterBuffer(&pbo3D0cuda, pbo3D0->id, cudaGraphicsMapFlagsNone);
    cudaGraphicsGLRegisterBuffer(&pbo3D1cuda, pbo3D1->id, cudaGraphicsMapFlagsNone);
    cudaGraphicsGLRegisterBuffer(&pbo3D2cuda, pbo3D2->id, cudaGraphicsMapFlagsNone);
    cudaGraphicsMapResources(1, &pbo3D0cuda, 0);
    cudaGraphicsMapResources(1, &pbo3D1cuda, 0);
    cudaGraphicsMapResources(1, &pbo3D2cuda, 0);
    cudaGraphicsResourceGetMappedPointer((void**)&idata0,&num_bytes,pbo3D0cuda);
    cudaGraphicsResourceGetMappedPointer((void**)&idata1,&num_bytes,pbo3D1cuda);
    cudaGraphicsResourceGetMappedPointer((void**)&idata2,&num_bytes,pbo3D2cuda);

    cudaGraphicsGLRegisterImage (&zTexFBO_resource, zTexFBO->id, zTexFBO->target, cudaGraphicsRegisterFlagsSurfaceLoadStore);

    cudaGraphicsGLRegisterBuffer(&ZlutCuda, ZlutPBO->id, cudaGraphicsMapFlagsNone);
}

void ncFlow::exitBpSweep() {

    cudaGraphicsUnregisterResource(ZlutCuda);

    cudaGraphicsUnregisterResource(zTexFBO_resource);

    cudaGraphicsUnmapResources(1, &pbo3D0cuda, 0);
    cudaGraphicsUnmapResources(1, &pbo3D1cuda, 0);
    cudaGraphicsUnmapResources(1, &pbo3D2cuda, 0);

    cudaGraphicsUnregisterResource(pbo3D0cuda);
    cudaGraphicsUnregisterResource(pbo3D1cuda);
    cudaGraphicsUnregisterResource(pbo3D2cuda);
}

void ncFlow::bpSweep() {

    fbo->bind(GL_FRAMEBUFFER);
    fbo->attach(GL_COLOR_ATTACHMENT0,zTexFBO,0);
    glViewport(0,0,winw,winh);
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    setupPtexPBO(ref);

    ncc::v3i size(winw,winh,wind);
    bp_sweep->Activate();
    bp_sweep->setUniform("size"     ,size);
	bp_sweep->setUniform("Zlut"		,ZlutPBO    , GL_READ_ONLY);
    bp_sweep->setUniform("nrim"     ,nrim  );
    bp_sweep->setUniform("ref"      ,ref   );
    bp_sweep->setUniform("Ptex"     ,PtexPBO	, GL_READ_ONLY);
    bp_sweep->setSampler("cTex"     ,cTex,0     );
    bp_sweep->setUniform("pbo3D"    ,pbo3D0    	, GL_READ_WRITE );

    ncc::v4f tmp[4];
    tmp[0] = ncc::v4f(-1.0f,-1.0f,0.0f,0.0f);
    tmp[1] = ncc::v4f(+1.0f,-1.0f,1.0f,0.0f);
    tmp[2] = ncc::v4f(-1.0f,+1.0f,0.0f,1.0f);
    tmp[3] = ncc::v4f(+1.0f,+1.0f,1.0f,1.0f);
    glVertexAttribPointer((GLuint)0, 4, GL_FLOAT, GL_FALSE, 0, &tmp[0].x);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    glDisableVertexAttribArray(0);

    bp_sweep->Deactivate();

    glMemoryBarrierEXT(GL_ALL_BARRIER_BITS_EXT);

    fbo->detach(GL_COLOR_ATTACHMENT0);
    fbo->unbind(GL_FRAMEBUFFER);

	size_t num_bytes;
    cudaGraphicsMapResources(1 , &zTexFBO_resource);
    cudaGraphicsSubResourceGetMappedArray(&zTexFBO_array, zTexFBO_resource, 0, 0);
    cudaGraphicsMapResources(1, &ZlutCuda, 0);
    cudaGraphicsResourceGetMappedPointer((void**)&ZlutDev,&num_bytes,ZlutCuda);
    beliefPropagation(zTexFBO_array,idata0,idata1,idata2,winw,winh,wind,ZlutDev);
    cudaGraphicsUnmapResources(1, &ZlutCuda, 0);
    cudaGraphicsUnmapResources(1, &zTexFBO_resource);
}

void ncFlow::run() {

	//cfg->rectify = true;

    sprintf(syscall,"mkdir -p \"%s\"",cfg->getDirName(TEST_DIR).c_str());
    system(syscall);
    sprintf(syscall,"mkdir -p \"%s\"",cfg->getDirName(MODEL_DIR).c_str());
    system(syscall);
    sprintf(syscall,"mkdir -p \"%s\"",cfg->getDirName(MERGED_DIR).c_str());
    system(syscall);
    sprintf(syscall,"mkdir -p \"%s\"",cfg->getDirName(DATA_DIR).c_str());
    system(syscall);
    sprintf(syscall,"mkdir -p \"%s\"",cfg->getDirName(PLY_DIR).c_str());
    system(syscall);

	std::vector<int> cids = cfg->getActiveCids();

    cfg->initProgress();
    for (unsigned int cc = 0 ; cc<cids.size();++cc)
    {
        int cid = cids[cc];

#if 1
		runPass1(cid);
		runCD(cid);
#endif

#if 0
		for (int i=0;i<1;++i) {
			runDEL(cid);
			runCD(cid);
		}
#endif

#if 1
	    runMDL(cid);
#endif

	}
    cfg->exitProgress();


#if 1
    std::vector<ncc::v3f4ub> merged_pdata;
    ncc::RTd merged_transform = cfg->project.poses.poselist->getTransform();
    ncc::RTd merged_transformi= merged_transform.inverse();

    std::vector<ncc::v3f4ub> file_pdata;
    ncc::RTd file_transform;

    unsigned int batch_size     = 1<<24;
    unsigned int batch_index;

    for (unsigned int cc = 0 ; cc<cids.size();++cc)
    {
        batch_index = 0;

        ncIDList id_list = cfg->createIDList( cids[cc]);

        int cid,vid;

        for (unsigned int i = 0 ; i<id_list.size();++i)
        {
            cid = id_list[i].cid;
            vid = id_list[i].vid;

            char fname[1024];
            sprintf(fname,"%s/%02d.%06d.mdl",cfg->getDirName(MODEL_DIR).c_str(),cid,vid);

            fprintf(stderr,"\rmerging %s...",fname);
            ncMDL::load(fname,file_transform,file_pdata);

            ncc::RTd tr = merged_transformi*file_transform;
            for (unsigned int i=0;i<file_pdata.size();++i)
            {
                ncc::v4d tmp = tr*ncc::v4d(file_pdata[i].x,file_pdata[i].y,file_pdata[i].z,1.0f);
                tmp/=tmp.w;
                merged_pdata.push_back(ncc::v3f4ub(ncc::v3f(tmp.x,tmp.y,tmp.z),file_pdata[i].color()));

                if (merged_pdata.size() == batch_size)
                {
                    sprintf(mystr,"%s/merged.%02d.%06d.mdl",cfg->getDirName(MERGED_DIR).c_str(),cid,batch_index++);
                    ncMDL::save(mystr,merged_transform,merged_pdata);
                    merged_pdata.clear();
                }
            }
        }

        if (merged_pdata.size() > 0)
        {
            sprintf(mystr,"%s/merged.%02d.%06d.mdl",cfg->getDirName(MERGED_DIR).c_str(),cid,batch_index++);
            ncMDL::save(mystr,merged_transform,merged_pdata);
            merged_pdata.clear();
        }
    }
    fprintf(stderr,"\n");
#endif
}

//! pass 1

void ncFlow::runPass1(int cid) {

    fprintf(stderr,"Pass1...\n");

    allocateStoragePass1(cid);

    initBpSweep();

    for(cur_cmd = 0;cur_cmd<cmd_list.size();++cur_cmd)
    {
        loadNextImagePass1();
        if (cmd_list[cur_cmd].ref!=-1)
        {
            sweepPass1();
            cfg->setProgress(texinfo[ref].cid,texinfo[ref].vid);
            cfg->saveImage<ncc::v2f>(DEPTH_CP,zTexFBO,0,texinfo[ref].cid,texinfo[ref].vid,winlod);
            updateSignal();
        }
    }
    exitBpSweep();
    clearStorage();
}

void ncFlow::sweepPass1() {

    //initSweep();
    bpSweep();
    medianC2(zTexFBO , 1 , 0.05f);

}

void ncFlow::allocateStoragePass1(int cam_id) {

    cmd_list = cfg->createCMDList(cam_id);

	int imw_     = (*cfg->project.cameras.calset)[cam_id].width(0) ;
    int imh_     = (*cfg->project.cameras.calset)[cam_id].height(0);
    int winlod_  = cfg->flow.winlod;
    int imlod_   = cfg->flow.imlod;
    int wind_    = cfg->flow.winD;

    setupZlutPBO(cam_id);

    if ((imw_!=imw) || (imh_!=imh) || (imlod_!=imlod) || (winlod_!=winlod) || (wind!=wind_))
    {
        clearStorage();

        imw      = imw_;
        imh      = imh_;
        imlod    = imlod_;
        lodw     = imw>>imlod;
        lodh     = imh>>imlod;
        winlod   = winlod_;
        winw     = imw>>winlod;
        winh     = imh>>winlod;
        wind     = wind_;

        fprintf(stderr,"allocating Pass1 storage %dx%d:%d:%d\n",imw,imh,imlod,winlod);

        Gsigma      = cfg->flow.gauss/(1<<imlod);
        gFactPBO    = new ncPBO;
        setupGauss(gFactPBO,Gsigma,false);
        fprintf(stderr,"Gsigma = %f\n",Gsigma);

        fbo     = new ncFBO;
        zTexFBO = new ncTexImage2D(GL_RG32F           ,winw,winh);

        cTex    = new ncTexImage2DArray(GL_RGBA16F    ,lodw,lodh,nrim);
        cTex->setFilters(GL_LINEAR,GL_LINEAR);

        loadTex = new ncTexImage2D( GL_RGBA8 ,imw,imh);
        loadTex->setFilters(GL_LINEAR_MIPMAP_LINEAR,GL_LINEAR);
        loadTex->generateMipmap();

        undistortTex = new ncTexImage2D( GL_RGBA8     ,imw,imh);
        undistortTex->setFilters(GL_LINEAR_MIPMAP_LINEAR,GL_LINEAR);
        undistortTex->generateMipmap();

        tmpTex[0]   = new ncTexImage2D(GL_RGBA16F       ,lodw,lodh);
        tmpTex[0]->setFilters(GL_LINEAR,GL_LINEAR);

        tmpTex[1]   = new ncTexImage2D(GL_RGBA16F       ,lodw,lodh);
        tmpTex[1]->setFilters(GL_LINEAR,GL_LINEAR);

        pbo3D0 = new ncPBO(winw*winh*wind,(float*)0,GL_DYNAMIC_COPY,GL_READ_WRITE);
        pbo3D1 = new ncPBO(winw*winh*wind,(float*)0,GL_DYNAMIC_COPY,GL_READ_WRITE);
        pbo3D2 = new ncPBO(winw*winh*wind,(float*)0,GL_DYNAMIC_COPY,GL_READ_WRITE);
    }
}

void ncFlow::loadNextImagePass1() {

    int cid    = cmd_list[cur_cmd].cid;
    int vid    = cmd_list[cur_cmd].vid;
    int index  = cmd_list[cur_cmd].index;
    ref = cmd_list[cur_cmd].ref;

    if (index != -1)
    {
        fprintf(stderr,"Loading %s to %d/%d (%d) ...\n",cfg->getImageFileName(cid,vid).c_str(),index,nrim-1,ref);

        texinfo[index] 		= cfg->getTexInfo(cid,vid,imlod);

        cfg->loadImage(loadTex,cid,vid,true);

        //! undistort
        ncCalibration cal 	= cfg->getCalibration(cid,vid);

        ncgl::undistorter->undistort(loadTex,cal,undistortTex,texinfo[index].calib);
        undistortTex->generateMipmap();

        //! store on disk

        grayscale(undistortTex,tmpTex[0],imlod);

        tmpTex[0]->generateMipmap();
        cfg->saveImage<ncc::v4ub>(COLOR_CP , tmpTex[0] , winlod-imlod,cid,vid,winlod);

#if 0
		{
			fbo->bind(GL_FRAMEBUFFER);
			fbo->attach(GL_COLOR_ATTACHMENT0 ,tmpTex[0],0,GL_FRAMEBUFFER);
			ncImageBuffer<ncc::v3ub> t1dat(lodw,lodh);
			glReadPixels(0,0,lodw,lodh,GL_RGB,GL_UNSIGNED_BYTE,t1dat.data);
			sprintf(mystr,"%s/%02d.%06d.RGB.ppm" ,cfg->getDirName(TEST_DIR).c_str(),cid,vid);
			ncc::writeImage(mystr,t1dat);
			fbo->detach(GL_COLOR_ATTACHMENT0);
			fbo->unbind(GL_FRAMEBUFFER);
        }
#endif

#if 0
        {
        	fbo->bind(GL_FRAMEBUFFER);
			fbo->attach(GL_COLOR_ATTACHMENT0 ,tmpTex[0],0,GL_FRAMEBUFFER);
			ncImageBuffer<unsigned char> t1dat(lodw,lodh);
			glReadPixels(0,0,lodw,lodh,GL_ALPHA,GL_UNSIGNED_BYTE,t1dat.data);
			sprintf(mystr,"%s/%02d.%06d.A.pgm" ,cfg->getDirName(TEST_DIR).c_str(),cid,vid);
			ncc::writeImage(mystr,t1dat);
			fbo->detach(GL_COLOR_ATTACHMENT0);
			fbo->unbind(GL_FRAMEBUFFER);
		}
#endif

        gaussC1(fbo,tmpTex[0],tmpTex[1],tmpTex[0],gFactPBO,0);

#if 0
        fbo->bind(GL_FRAMEBUFFER);
        fbo->attach(GL_COLOR_ATTACHMENT0 ,tmpTex[0],0,GL_FRAMEBUFFER);
        ncImageBuffer<ncc::v3ub> t2dat(lodw,lodh);
        glReadPixels(0,0,lodw,lodh,GL_RGB,GL_UNSIGNED_BYTE,t2dat.data);
        sprintf(mystr,"%s/%02d.%06d.test2.%d.ppm" ,cfg->getDirName(TEST_DIR).c_str(),cid,vid,cfg->TEST_INDEX);
        ncc::writeImage(mystr,t2dat);
        fbo->detach(GL_COLOR_ATTACHMENT0);
        fbo->unbind(GL_FRAMEBUFFER);
#endif

        fbo->bind(GL_FRAMEBUFFER);
        fbo->attach(GL_COLOR_ATTACHMENT0 ,cTex,index,0,GL_FRAMEBUFFER);

        glViewport(0,0,cTex->width,cTex->height);
        laplace_prog->Activate();
        laplace_prog->setUniform("lod"    ,0);
        laplace_prog->setUniform("sigma"  ,Gsigma);
        laplace_prog->setSampler("cTex"   ,tmpTex[0],0);
        drawFSrect();

#if 0
        {
        ncImageBuffer<unsigned char> ldat(lodw,lodh);
        glReadPixels(0,0,lodw,lodh,GL_RED,GL_UNSIGNED_BYTE,&ldat[0]);
        sprintf(mystr,"%s/%02d.%06d.laplaceR.%d.pgm" ,cfg->getDirName(TEST_DIR).c_str(),cid,vid,cfg->TEST_INDEX);
        ncc::writeImage(mystr,ldat);
        }

        {
        ncImageBuffer<unsigned char> ldat(lodw,lodh);
        glReadPixels(0,0,lodw,lodh,GL_GREEN,GL_UNSIGNED_BYTE,&ldat[0]);
        sprintf(mystr,"%s/%02d.%06d.laplaceG.%d.pgm" ,cfg->getDirName(TEST_DIR).c_str(),cid,vid,cfg->TEST_INDEX);
        ncc::writeImage(mystr,ldat);
        }

        {
        ncImageBuffer<unsigned char> ldat(lodw,lodh);
        glReadPixels(0,0,lodw,lodh,GL_BLUE,GL_UNSIGNED_BYTE,&ldat[0]);
        sprintf(mystr,"%s/%02d.%06d.laplaceB.%d.pgm" ,cfg->getDirName(TEST_DIR).c_str(),cid,vid,cfg->TEST_INDEX);
        ncc::writeImage(mystr,ldat);
        }

        {
        ncImageBuffer<unsigned char> ldat(lodw,lodh);
        glReadPixels(0,0,lodw,lodh,GL_ALPHA,GL_UNSIGNED_BYTE,&ldat[0]);
        sprintf(mystr,"%s/%02d.%06d.laplaceA.%d.pgm" ,cfg->getDirName(TEST_DIR).c_str(),cid,vid,cfg->TEST_INDEX);
        ncc::writeImage(mystr,ldat);
        }
#endif

        fbo->detach(GL_COLOR_ATTACHMENT0);
        fbo->unbind(GL_FRAMEBUFFER);
    }

}

//! CD

void ncFlow::sweepCD(int cam_id) {

    fbo->bind(GL_FRAMEBUFFER);
    fbo->attach(GL_COLOR_ATTACHMENT0,zTexFBO,0);
    glViewport(0,0,winw,winh);
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    setupPtexPBO(ref);

    zTex->setFilters(GL_NEAREST,GL_NEAREST);

    cd_sweep->Activate();
    cd_sweep->setUniform("Zn"       ,cfg->project.cameras.znear[cam_id] );
    cd_sweep->setUniform("nrim"     ,(int)nrim    						);
    cd_sweep->setUniform("ref"      ,(int)ref     						);
    cd_sweep->setUniform("Ptex"     ,PtexPBO    , GL_READ_ONLY			);
    //cd_sweep->setSampler("cTex"   ,cTex   	,0  					);
    cd_sweep->setSampler("zTex"     ,zTex   	,1    					);

    ncc::v4f tmp[4];
    tmp[0] = ncc::v4f(-1.0f,-1.0f,0.0f,0.0f);
    tmp[1] = ncc::v4f(+1.0f,-1.0f,1.0f,0.0f);
    tmp[2] = ncc::v4f(-1.0f,+1.0f,0.0f,1.0f);
    tmp[3] = ncc::v4f(+1.0f,+1.0f,1.0f,1.0f);
    glVertexAttribPointer((GLuint)0, 4, GL_FLOAT, GL_FALSE, 0, &tmp[0].x);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    glDisableVertexAttribArray(0);

    fbo->detach(GL_COLOR_ATTACHMENT0);
    fbo->unbind(GL_FRAMEBUFFER);

}

void ncFlow::runCD(int cid) {

    fprintf(stderr,"Depth check...\n");

    cfg->initProgress();

    allocateStorageCD(cid);

    for(cur_cmd = 0;cur_cmd<cmd_list.size();++cur_cmd)
    {
        loadNextImageCD();
        if (cmd_list[cur_cmd].ref!=-1)
        {
            sweepCD(cid);

            cfg->setProgress(texinfo[ref].cid,texinfo[ref].vid);
            cfg->saveImage<ncc::v2f>(DEPTH_CHECK_CP,zTexFBO,0,texinfo[ref].cid,texinfo[ref].vid,winlod);
            updateSignal();
        }
    }
    clearStorage();
}

void ncFlow::allocateStorageCD(int cam_id) {

    cmd_list = cfg->createCMDList(cam_id);

	int imw_     = (*cfg->project.cameras.calset)[cam_id].width(0) ;
    int imh_     = (*cfg->project.cameras.calset)[cam_id].height(0);
    int winlod_  = cfg->flow.winlod;
    int imlod_   = cfg->flow.imlod;
    int wind_    = cfg->flow.winD;

    if ((imw_!=imw) || (imh_!=imh) || (imlod_!=imlod) || (winlod_!=winlod) || (wind!=wind_))
    {
        clearStorage();

        imw      = imw_;
        imh      = imh_;
        imlod    = imlod_;
        lodw     = imw>>imlod;
        lodh     = imh>>imlod;
        winlod   = winlod_;
        winw     = imw>>winlod;
        winh     = imh>>winlod;
        wind     = wind_;

        fprintf(stderr,"allocating CD storage %dx%d:%d:%d\n",imw,imh,imlod,winlod);

        fbo     = new ncFBO;
        zTexFBO = new ncTexImage2D(GL_RG32F           ,winw,winh);

        //cTex    = new ncTexImage2DArray(GL_RGBA32F    ,winw,winh,nrim);
        //cTex->setFilters(GL_LINEAR,GL_LINEAR);

        zTex    = new ncTexImage2DArray(GL_RG32F    ,winw,winh,nrim);
        zTex->setFilters(GL_LINEAR,GL_LINEAR);
    }
}

void ncFlow::loadNextImageCD() {

    int cid    = cmd_list[cur_cmd].cid;
    int vid    = cmd_list[cur_cmd].vid;
    int index  = cmd_list[cur_cmd].index;
    ref = cmd_list[cur_cmd].ref;

    if (index != -1)
    {
        texinfo[index] = cfg->getTexInfo(cid,vid,winlod);

        //cfg->loadImage<ncc::v4ub>(cTex,index,cid,vid,texinfo[index]);
        cfg->loadImage<ncc::v2f>(DEPTH_CP,  zTex,index,cid,vid,texinfo[index]);

#if 0
        fbo->bind(GL_FRAMEBUFFER);
        fbo->attach(GL_COLOR_ATTACHMENT0 ,cTex,index,0,GL_FRAMEBUFFER);
        ncImageBuffer<ncc::v3ub> dat(winw,winh);
        glReadPixels(0,0,winw,winh,GL_RGB,GL_UNSIGNED_BYTE,dat.data);
        sprintf(mystr,"%s/%02d.%06d.ctest.%d.ppm" ,cfg->getDirName(TEST_DIR).c_str(),cid,vid,cfg->TEST_INDEX);
        ncc::writeImage(mystr,dat);
        fbo->detach(GL_COLOR_ATTACHMENT0);
        fbo->unbind(GL_FRAMEBUFFER);
#endif

#if 0
        fbo->bind(GL_FRAMEBUFFER);
        fbo->attach(GL_COLOR_ATTACHMENT0 ,zTex,index,0,GL_FRAMEBUFFER);
        ncImageBuffer<unsigned char> dat(winw,winh);
        glReadPixels(0,0,winw,winh,GL_RED,GL_UNSIGNED_BYTE,dat.data);
        sprintf(mystr,"%s/%02d.%06d.dtest.%d.pgm" ,cfg->getDirName(TEST_DIR).c_str(),cid,vid,cfg->TEST_INDEX);
        ncc::writeImage(mystr,dat);
        fbo->detach(GL_COLOR_ATTACHMENT0);
        fbo->unbind(GL_FRAMEBUFFER);
#endif

    }
}

//! MDL

void ncFlow::runMDL(int cid) {

    fprintf(stderr,"running MDL...\n");

    cfg->initProgress();

    allocateStorageMDL(cid);

    for(cur_cmd = 0;cur_cmd<cmd_list.size();++cur_cmd)
    {
        loadNextImageMDL();
        if (cmd_list[cur_cmd].ref!=-1)
        {
            sweepMDL();
            cfg->setProgress(texinfo[ref].cid,texinfo[ref].vid);
            updateSignal();
        }
    }
    clearStorage();
}

void ncFlow::sweepMDL() {

    int upscale = 0;

    int w = winw<<upscale;
    int h = winh<<upscale;

    ncTexImage2D* my_cTex = new ncTexImage2D(GL_RGBA32F  ,w,h);
    ncTexImage2D* my_zTex = new ncTexImage2D(GL_RG32F    ,w,h);

    fbo->bind(GL_FRAMEBUFFER);
    fbo->attach(GL_COLOR_ATTACHMENT0,my_cTex,0);
    fbo->attach(GL_COLOR_ATTACHMENT1,my_zTex,0);

  	GLenum drawbuffers[] = {GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1};
	glDrawBuffers(2,drawbuffers);

    glViewport(0,0,w,h);
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    setupPtexPBO(ref);

    mdl_sweep->Activate();
    mdl_sweep->setUniform("nrim"       ,(int)nrim  );
    mdl_sweep->setUniform("ref"        ,(int)ref   );
    mdl_sweep->setUniform("Ptex"       ,PtexPBO    , GL_READ_ONLY);
    mdl_sweep->setSampler("cTex"       ,cTex   ,0  );
    mdl_sweep->setSampler("zTex"       ,zTex   ,1  );

    ncc::v4f tmp[4];
    tmp[0] = ncc::v4f(-1.0f,-1.0f,0.0f,0.0f);
    tmp[1] = ncc::v4f(+1.0f,-1.0f,1.0f,0.0f);
    tmp[2] = ncc::v4f(-1.0f,+1.0f,0.0f,1.0f);
    tmp[3] = ncc::v4f(+1.0f,+1.0f,1.0f,1.0f);
    glVertexAttribPointer((GLuint)0, 4, GL_FLOAT, GL_FALSE, 0, &tmp[0].x);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    glDisableVertexAttribArray(0);

    mdl_sweep->Deactivate();

    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    fbo->detach(GL_COLOR_ATTACHMENT1);
    fbo->detach(GL_COLOR_ATTACHMENT0);
    fbo->unbind(GL_FRAMEBUFFER);


    fbo->bind(GL_FRAMEBUFFER);
    fbo->attach(GL_COLOR_ATTACHMENT0,my_cTex,0);
    ncImageBuffer<ncc::v4ub>    mdl_color(w,h);
    glReadPixels(0,0,w,h,GL_RGBA,GL_UNSIGNED_BYTE ,&mdl_color[0].x);
    fbo->detach(GL_COLOR_ATTACHMENT0);
    fbo->unbind(GL_FRAMEBUFFER);

    fbo->bind(GL_FRAMEBUFFER);
    fbo->attach(GL_COLOR_ATTACHMENT0,my_zTex,0);
    ncImageBuffer<ncc::v2f>    mdl_depth(w,h);
    glReadPixels(0,0,w,h,GL_RG,GL_FLOAT ,&mdl_depth[0].x);

    //ncImageBuffer<float>    mdl_depthx(w,h);
    //glReadPixels(0,0,w,h,GL_RED,GL_FLOAT ,&mdl_depthx[0]);
    //sprintf(mystr,"/tmp/%02d.%09d.pgm",texinfo[ref].cid,texinfo[ref].vid);
    //ncc::writeImage(mystr,mdl_depthx);

    fbo->detach(GL_COLOR_ATTACHMENT0);
    fbo->unbind(GL_FRAMEBUFFER);

    delete my_cTex;
    delete my_zTex;

    ncc::m4x4d Ktexi = texinfo[ref].Ktex().inverse();

    int         border                              = 0;
    ncc::v4ub   overexposure_threshold              = ncc::v4ub(230,230,230,0);
    float       minimum_height_relative_to_camera   = -2.7f;
    float       minimum_window_depth   				= 0.0f;
    float       maximum_window_depth   				= 0.99f;

    ncc::RTf local_tr = texinfo[ref].MV();
    local_tr[3].x = local_tr[3].y = local_tr[3].z = 0.0;

    std::vector<ncc::v3f4ub> mdl_data;
    for ( int y = border;y<h-border;++y)
    {
        //for ( int x = border+w/2;x<w-border;++x)
        for ( int x = border    ;x<w-border;++x)
        {
            unsigned int index = y*w+x;

            bool exposure_test  = !((mdl_color[index].x>overexposure_threshold.x) && (mdl_color[index].y>overexposure_threshold.y) && (mdl_color[index].z>overexposure_threshold.z));
            bool valid_test     = (mdl_depth[index].y>0.0f);
            bool depth_test     = (mdl_depth[index].x>minimum_window_depth) && (mdl_depth[index].x<maximum_window_depth);

            if (depth_test && valid_test && exposure_test)
            {
                ncc::v4f tmp = Ktexi * ncc::v4f((x+0.5f)/w,(y+0.5f)/h,mdl_depth[index].x,1.0f);
                tmp/=tmp.w;
                bool distance_test  = (tmp.xyz().norm() < 2.0f*cfg->project.cameras.zfar[texinfo[ref].cid]);

                ncc::v4f tmp2       = local_tr*tmp;
                tmp2/=tmp2.w;
                bool height_test    = tmp2.z>minimum_height_relative_to_camera;

                if (distance_test && height_test)
                    mdl_data.push_back( ncc::v3f4ub( tmp.xyz() , mdl_color[index] ));
            }
        }
    }

    char fname[1024];
    sprintf(fname,"%s/%02d.%06d.mdl",cfg->getDirName(MODEL_DIR).c_str(),texinfo[ref].cid,texinfo[ref].vid);
    ncMDL::save(fname, texinfo[ref].MV() ,mdl_data);
}

void ncFlow::allocateStorageMDL(int cam_id) {

    cmd_list = cfg->createCMDList(cam_id);

	int imw_     = (*cfg->project.cameras.calset)[cam_id].width(0) ;
    int imh_     = (*cfg->project.cameras.calset)[cam_id].height(0);
    int winlod_  = cfg->flow.winlod;
    int imlod_   = cfg->flow.imlod;
    int wind_    = cfg->flow.winD;

    if ((imw_!=imw) || (imh_!=imh) || (imlod_!=imlod) || (winlod_!=winlod) || (wind!=wind_))
    {
        clearStorage();

        imw      = imw_;
        imh      = imh_;
        imlod    = imlod_;
        lodw     = imw>>imlod;
        lodh     = imh>>imlod;
        winlod   = winlod_;
        winw     = imw>>winlod;
        winh     = imh>>winlod;
        wind     = wind_;

        mdl_upscale = 1;

        fprintf(stderr,"allocating MDL storage %dx%d:%d:%d\n",imw,imh,imlod,winlod);

        fbo     = new ncFBO;

        cTex    = new ncTexImage2DArray(GL_RGBA8  ,winw,winh,nrim);
        cTex->setFilters(GL_LINEAR,GL_LINEAR);

        zTex    = new ncTexImage2DArray(GL_RG32F    ,winw,winh,nrim);
        zTex->setFilters(GL_LINEAR,GL_LINEAR);

    }
}

void ncFlow::loadNextImageMDL() {

    int cid    = cmd_list[cur_cmd].cid;
    int vid    = cmd_list[cur_cmd].vid;
    int index  = cmd_list[cur_cmd].index;
    ref = cmd_list[cur_cmd].ref;

    if (index != -1)
    {
        texinfo[index] = cfg->getTexInfo(cid,vid,winlod);

        cfg->loadImage<ncc::v4ub>(COLOR_CP,cTex,index,cid,vid,texinfo[index]);
        cfg->loadImage<ncc::v2f>(DEPTH_CHECK_CP,zTex,index,cid,vid,texinfo[index]);
    }
}

//! DEL

void ncFlow::runDEL(int cid) {

    fprintf(stderr,"Delaunay creation...\n");

    ncPtr<ncTexImage2D> ccc;
    ncPtr<ncTexImage2D> ddd;

    ncFBO* my_fbo = new ncFBO;

    ncIDList id_list = cfg->createIDList( cid );

    for (unsigned int i = 0 ; i<id_list.size();++i)
    {
        int cid = id_list[i].cid;
        int vid = id_list[i].vid;

        cfg->setProgress(cid,vid);

        ncTexInfo ti;
        cfg->loadImage<ncc::v2f>(DEPTH_CHECK_CP,ddd,cid,vid,ti);

        std::vector<ncc::v2d> v_in_xy;
        std::vector<ncc::v3f> v_in_3d;
        std::vector<ncc::v3i> i_out;

        ncImageBuffer<ncc::v2f>  rg_depth(ti.width(),ti.height());

        ddd->bind(0);
        glGetTexImage(ddd->target,0,GL_RG ,GL_FLOAT ,&rg_depth[0].x);

        for ( int y = 0;y<ti.width();++y)
            for ( int x = 0;x<ti.height();++x)
            {
                unsigned int index = y*ti.width()+x;
                if (rg_depth[index].y>0.0f)
                {
                    v_in_xy.push_back(ncc::v2d(x+0.5,y+0.5));
                    v_in_3d.push_back(ncc::v3d( 2.0*(x+0.5)/ti.width() - 1.0 , 2.0*(y+0.5)/ti.height() - 1.0 , rg_depth[index].x));
                }
            }

        ncc::delaunay2D(v_in_xy, i_out);

        my_fbo->bind(GL_FRAMEBUFFER);
        my_fbo->attach(GL_COLOR_ATTACHMENT0,ddd,0);
        glViewport(0,0,ti.width(),ti.height());
        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        delaunay_prog->Activate();
        glBindBuffer(GL_ARRAY_BUFFER, 0 );
        glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, &v_in_3d[0].x);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0 );
        glDrawElements(GL_TRIANGLES, i_out.size()*3, GL_UNSIGNED_INT, &i_out[0].x );
        glDisableVertexAttribArray(0);

        delaunay_prog->Deactivate();
        my_fbo->detach(GL_COLOR_ATTACHMENT0);
        my_fbo->unbind(GL_FRAMEBUFFER);

        //medianC2(ddd,1);

        cfg->saveImage<ncc::v2f>(DEPTH_CP,ddd,0,ti.cid,ti.vid,ti.lod);
        updateSignal();
    }

    delete my_fbo;
}
