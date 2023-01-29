#include "ncgvm.h"
#include "ncqtprogram.h"

int                 ncGVM::refcount  = 0;
ncPtr<ncProgram>    ncGVM::render_prog;


ncGVM::ncGVM() {
    init();
}

ncGVM::ncGVM(std::string fname) {
    init();
    ncModel::load(fname);
}

ncGVM::~ncGVM() {
    vpbo.reset();
    mesh.reset();
    fbo.reset();
    dtex.reset();

    refcount--;
    if (refcount == 0)
        render_prog.reset();
}

void ncGVM::init() {

    type = NC_GVM;

    if (refcount == 0)
    {
        Q_INIT_RESOURCE(ncmodel_shaders);
        render_prog = new ncQtProgram(":/shaders/gvm.vert"		, ":/shaders/gvm.geom"		, ":/shaders/gvmd.frag"	);
    }
    refcount++;

    fbo     = new ncFBO;
    dtex    = new ncTexImage2D(GL_DEPTH_COMPONENT32F,64,64);

    vpbo = new ncPBO;

    mesh = new ncMesh;
    mesh->bindAttribI(0,vpbo,4,GL_INT,sizeof(ncc::v4i),0);

}

void ncGVM::clear() {
    glDeleteQueries(subs.size(),&occ[0]);
    ncModel::clear();
}

bool ncGVM::v_load(std::string fname) {

    ncVoxTree::load(fname , false , false);

    std::vector<ncc::v4i> subpos;
    for (unsigned int i=0;i<subs.size();++i)
        subpos.push_back(subs[i].pos);

    vpbo->setup(subpos.size(),&subpos[0]);

    mesh->bindAttribI(0,vpbo,4,GL_INT,sizeof(ncc::v4i),0);

    occ.resize(subs.size());
    glGenQueries(subs.size(),&occ[0]);

    vis.resize(subs.size());

    return true;
}

void ncGVM::v_draw(ncc::m4x4d KGL , ncc::RTd MVi) {
    drawCCV(KGL , MVi.inverse() , GL_CCW);
}

struct ncCamDist {

    ncCamDist(unsigned int sub_id , double dist) : sub_id(sub_id) , dist(dist) {};

    bool operator<(const ncCamDist& cd) const {
        return dist<cd.dist;
    }

    unsigned int    sub_id;
    double          dist;
};

void ncGVM::setCameraPosition(ncc::m4x4d KGL , ncc::RTd MV , int w, int h , GLenum cw_dir) {

//fprintf(stderr,"+ncGVM::setCameraPosition\n");

    GLint bleh;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING,&bleh);
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    dtex->resize(w,h);


    fbo->bind(GL_FRAMEBUFFER);
    glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
    fbo->attach(GL_DEPTH_ATTACHMENT,dtex);

    ncc::RTd MV_ = getTransform().inverse()*MV;
    ncc::RTd MVi = MV_.inverse();

    for (unsigned int i=0;i<subs.size();++i)
        vis[i] = false;

    unsigned int nrvis = 0;
    bool vis_changed = true;


#if 1
    while ((nrvis<max_resident) && vis_changed)
    {

        vis_changed = false;

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_DEPTH_CLAMP);

        glClearDepth(1.0f);
        glClear(GL_DEPTH_BUFFER_BIT);

        for (unsigned int i=0;i<vis.size();++i)
            if (vis[i])
                ((ncSubGVM*)voxels.getValue<NC_PTR>(subs[i].id).value)->draw(KGL,MV,cw_dir);

        render_prog->Activate();
        render_prog->setUniform("MVP"		,KGL*MVi);
        for (unsigned int i=0;i<vis.size();++i)
            if (!vis[i])
            {
                glBeginQuery(GL_ANY_SAMPLES_PASSED,occ[i]);
                mesh->draw(GL_POINTS,1,i);
                glEndQuery(GL_ANY_SAMPLES_PASSED);
            }

        render_prog->Deactivate();

        glDisable(GL_DEPTH_CLAMP);
        glDisable(GL_DEPTH_TEST);

        std::vector<ncCamDist> camdist;
        for (unsigned int i=0; i<subs.size() ;++i)
            if (!vis[i])
            {
                int tmp;
                glGetQueryObjectiv(occ[i], GL_QUERY_RESULT, &tmp);
                if (tmp)
                    camdist.push_back(ncCamDist(i,getCameraDistance(MV[3].xyz(),subs[i].pos)));
                //else
                //    ncVoxTree::saveSubLevel(subs[i].xyz());
            }

        std::sort(camdist.begin(),camdist.end());

        for (unsigned int i=0; (nrvis<max_resident) && (i<camdist.size());++i)
        {
            ncVoxTree::loadSubLevel(camdist[i].sub_id);
            vis[camdist[i].sub_id] = true;
            nrvis++;
            vis_changed = true;
        }
    }

#endif

    glClear(GL_DEPTH_BUFFER_BIT);

    for (unsigned int i=0;i<vis.size();++i)
        if (vis[i])
            ((ncSubGVM*)voxels.getValue<NC_PTR>(subs[i].id).value)->draw(KGL,MV,cw_dir);


    glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
    fbo->detach(GL_DEPTH_ATTACHMENT);
    fbo->unbind();

    glBindFramebuffer(GL_FRAMEBUFFER,bleh);
    glPopAttrib();

//fprintf(stderr,"-ncGVM::setCameraPosition\n");

}

void ncGVM::drawCCV(ncc::m4x4d KGL , ncc::RTd MV , GLenum cw_dir) {

//fprintf(stderr,"+ncGVM::draw\n");
        ncc::v4i vp;
        glGetIntegerv(GL_VIEWPORT,&vp[0]);
        setCameraPosition(KGL,MV,vp[2],vp[3],cw_dir);

        glClear(GL_DEPTH_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT);

        for ( std::list<unsigned int>::iterator it = LRU.begin() ;  it!=LRU.end() ; ++it)
        {
            ncVoxTreeIterator tmp = subs[*it];
            ((ncSubGVM*)voxels.getValue<NC_PTR>(tmp.id).value)->draw(KGL,MV,cw_dir);
        }

//fprintf(stderr,"-ncGVM::draw\n");
}

void ncGVM::loadSubLevel(ncVoxTreeIterator it) {
    //fprintf(stderr,"ncGVM::loadSubLevel -> %d\n",(data_mask&NC_COLOR ? 1 : 0));
    voxels.getValue<NC_PTR>(it.id).value = new ncSubGVM(getFilename(it.pos.xyz()).c_str());
}

void ncGVM::loadSubLevel(ncVoxTreeIterator it , unsigned int data_mask , ncc::v3ll pmin , int maxlevel , int minlevel , unsigned int maxcarve) {
    voxels.getValue<NC_PTR>(it.id).value = new ncSubGVM(getFilename(it.pos.xyz()).c_str() , data_mask , pmin , maxlevel , minlevel , maxcarve);
}

void ncGVM::saveSubLevel(ncVoxTreeIterator it) {
    //fprintf(stderr,"ncGVM::saveSubLevel -> %d\n",(data_mask&NC_COLOR ? 1 : 0));

    ncSubGVM*& vt = (ncSubGVM*&)voxels.getValue<NC_PTR>(it.id).value;

    if (data_mask&NC_CHILD) {
        vt->child_pbo->bind(GL_ARRAY_BUFFER);
        glGetBufferSubData(GL_ARRAY_BUFFER,0,vt->voxels.size()*sizeof(ncAttribute<NC_CHILD>),&vt->voxels.getValue<NC_CHILD>(0));
        vt->child_pbo->unbind(GL_ARRAY_BUFFER);
    }

    if (data_mask&NC_RGBM) {
        vt->rgbm_pbo->bind(GL_ARRAY_BUFFER);
        glGetBufferSubData(GL_ARRAY_BUFFER,0,vt->voxels.size()*sizeof(ncAttribute<NC_RGBM>),&vt->voxels.getValue<NC_RGBM>(0));
        vt->rgbm_pbo->unbind(GL_ARRAY_BUFFER);
    }

    if (data_mask&NC_COLOR) {
        vt->color_pbo->bind(GL_ARRAY_BUFFER);
        glGetBufferSubData(GL_ARRAY_BUFFER,0,vt->voxels.size()*sizeof(ncAttribute<NC_COLOR>),&vt->voxels.getValue<NC_COLOR>(0));
        vt->color_pbo->unbind(GL_ARRAY_BUFFER);

        //for (unsigned int i=0;i<vt->voxels.size();++i)
        //    vt->voxels.setValue<NC_COLOR>(i,255,0,0,1);
    }

    if (save_to_disk)
        vt->save();
    delete vt;
    vt = 0;
}






int                 ncSubGVM::refcount  = 0;
int                 ncSubGVM::lod       = 0;
ncPtr<ncProgram>    ncSubGVM::render_prog;

ncSubGVM::ncSubGVM(std::string fname) {

    init();
    load(fname);
}

ncSubGVM::ncSubGVM(std::string filename_ , unsigned int data_mask_ , ncc::v3ll pmin_ , int maxlevel_ , int minlevel_ , unsigned int maxcarve_) {

    init();
    ncSubVoxTree(filename_ , data_mask_ , pmin_ , maxlevel_ , minlevel_ , maxcarve_);
    cpu2gpu();
}

ncSubGVM::~ncSubGVM() {

    clear();

    refcount--;
    if (refcount == 0)
    {
        render_prog.reset();
    }
}

void ncSubGVM::init() {

    if (refcount == 0)
    {
        Q_INIT_RESOURCE(ncmodel_shaders);
        render_prog = new ncQtProgram(":/shaders/gvm.vert"		, ":/shaders/gvm.geom"		, ":/shaders/gvm.glsl,:/shaders/gvm.frag"		);
    }
    refcount++;
}

void ncSubGVM::clear() {

    child_pbo.reset();
    rgbm_pbo.reset();
    color_pbo.reset();
    mesh.reset();
    vpbo.reset();

    ncSubVoxTree::clear();
}

bool ncSubGVM::load(std::string fname) {

    filename = fname;
    if (!filename.empty())
    {
        clear();
        ncSubVoxTree::load(filename);
        //fprintf(stderr,"ncSubGVM::load() -> %d\n", (voxels.data_mask&NC_COLOR? 1 : 0));
        cpu2gpu();
        return true;
    }
    else
        return false;
}

void ncSubGVM::cpu2gpu() {


    ncc::v4i tmp(0,0,0,maxlevel);

    if (!vpbo)
        vpbo = new ncPBO;
    vpbo->setup(1,&tmp);

    if (!mesh)
        mesh = new ncMesh;
    mesh->bindAttribI(0,vpbo,4,GL_INT,sizeof(ncc::v4i),0);

    if (!child_pbo)
        child_pbo = new ncPBO;
    child_pbo->setup(voxels.size(),&voxels.getValue<NC_CHILD>(0));

    if (voxels.data_mask&NC_RGBM)
    {
        if (!rgbm_pbo)
            rgbm_pbo = new ncPBO;
        rgbm_pbo->setup(voxels.size(),&voxels.getValue<NC_RGBM>(0));
    }

    if (voxels.data_mask&NC_COLOR)
    {
        //fprintf(stderr,"allocating color storage...\n");
        if (!color_pbo)
            color_pbo = new ncPBO;
        color_pbo->setup(voxels.size(),&voxels.getValue<NC_COLOR>(0));
    }
}

void ncSubGVM::draw(ncc::m4x4d KGL , ncc::RTd MV , GLenum cw_dir) {

        ncc::RTd MV_ = getTransform().inverse()*MV;
        ncc::RTd MVi = MV_.inverse();
        ncc::v3d campos = MV_[3].xyz();

		render_prog->Activate();
		render_prog->setUniform("campos"    ,campos);
		render_prog->setUniform("MVP"		,KGL*MVi);
		render_prog->setUniform("maxlevel"	,(int)maxlevel);
		render_prog->setUniform("lod"	    ,(int)lod);
        if (voxels.data_mask&NC_CHILD)
            render_prog->setUniform("child"  	,child_pbo  , GL_READ_ONLY);
        if (voxels.data_mask&NC_RGBM)
            render_prog->setUniform("rgbm" 	    ,rgbm_pbo   , GL_READ_ONLY);
        if (voxels.data_mask&NC_COLOR)
            render_prog->setUniform("color" 	,color_pbo  , GL_READ_ONLY);

        drawMesh(KGL,MV,cw_dir);

		render_prog->Deactivate();

}

void ncSubGVM::drawMesh(ncc::m4x4d KGL , ncc::RTd MV , GLenum cw_dir) {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_CLAMP);
    glEnable(GL_CULL_FACE);
    glFrontFace(cw_dir);
    ncc::v3d cp2 = 1000.0*MV[3].xyz();
    int vtreesize = (1<<maxlevel);
    if ((cp2.x>0) && (cp2.x<vtreesize) && (cp2.y>0) && (cp2.y<vtreesize) && (cp2.z>0) && (cp2.z<vtreesize))
        glCullFace(GL_BACK);
    else
        glCullFace(GL_FRONT);
    mesh->draw(GL_POINTS,1);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_CLAMP);
    glDisable(GL_DEPTH_TEST);
}
