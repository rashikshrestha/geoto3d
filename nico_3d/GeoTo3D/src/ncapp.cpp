#include "ncapp.h"

#include <stdio.h>
#include <stdlib.h>

#if 0
#include <cuda.h>
#include <cudaGL.h>
#include <cuda_gl_interop.h>
#include <cuda_runtime_api.h>
#include <cutil_inline.h>

#include "cuda_gvm.h"

cudaGraphicsResource*   fbo_resource    = 0;
cudaArray*              fbo_array       = 0;
ncVoxTree*           vox_tree        = 0;
cudaGVM                 gvm;
extern "C" void runTest(cudaArray* my_array, unsigned int w, unsigned int h , cudaGVM gvm_);
#endif

ncApp::ncApp(QString title, ncQtMainWindow* mainw) : ncQtGLWidget(title, mainw, 0, Qt::Window)
{
    fixed = false;

    cudaSetDevice(0);
    cudaGLSetGLDevice(0);

    Q_INIT_RESOURCE(GeoTo3D_shaders);

    //fprintf(stderr,"+ncApp\n");

    counter         = 0;
	cfg             = new ncQtConfig3D("Config File", mainw,Qt::RightDockWidgetArea  , Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea );

  	mainw->file_menu->addAction(cfg->action_new);
  	mainw->file_menu->addAction(cfg->action_load);

	qt_logger       = new ncQtLogger("Logger"   , mainw,Qt::BottomDockWidgetArea , Qt::BottomDockWidgetArea|Qt::TopDockWidgetArea   );

	qt_cam          = new ncQtCamera("Camera Control"       , mainw,Qt::LeftDockWidgetArea  , Qt::LeftDockWidgetArea  |Qt::RightDockWidgetArea );
	connect(qt_cam  , SIGNAL(log(QString))      , qt_logger , SLOT(log(QString)));
    connect(qt_cam  , SIGNAL(valuesChanged())   , this      , SLOT(updateGL()));

	qt_video        = new ncQtVideo("Video Control"	        , mainw,Qt::LeftDockWidgetArea  , Qt::LeftDockWidgetArea  |Qt::RightDockWidgetArea );
	connect(qt_video    , SIGNAL(log(QString))              , qt_logger , SLOT(log(QString))    );
    connect(qt_video    , SIGNAL(poseChanged(ncc::RTd))    , qt_cam	, SLOT(setMV(ncc::RTd)) );

	connect(cfg->flow_button    , SIGNAL(clicked()      ), this , SLOT(flow_command())    );
	connect(cfg->clobber_button , SIGNAL(clicked()      ), this , SLOT(clobber_command()) );
	connect(cfg->carve_button   , SIGNAL(clicked()      ), this , SLOT(carve_command())   );
	connect(cfg->recarve_button , SIGNAL(clicked()      ), this , SLOT(recarve_command()) );
	connect(cfg->remesh_button  , SIGNAL(clicked()      ), this , SLOT(remesh_command())  );
	connect(cfg                 , SIGNAL(valuesChanged()), this , SLOT(setVideoConfig())  );

    //fprintf(stderr,"-ncApp\n");

};

void ncApp::flow_command()
{
    cfg->save(cfg->getFileName(XML_FILE));
    ncFlow* flower = new ncFlow(cfg);
   	connect(flower , SIGNAL(updateSignal()) , this , SLOT(updateSignal()));
    flower->run();
   	disconnect(flower , SIGNAL(updateSignal()) , this , SLOT(updateSignal()));
    delete flower;

    //carve_command();
}

void ncApp::carve_command()
{

    int maxcarve_   = cfg->vox.max_carve;
    int minlevel_   = cfg->vox.min_level;

    ncc::v3d pmin_,pmax_;
    cfg->getBoundingBox(pmin_,pmax_);
    pmin_=1000.0*pmin_;
    pmax_=1000.0*pmax_;

    ncc::v3ll imin_(0,0,0);
    int maxlevel_   = -1;
    bool found = false;
    while (!found)
    {
        maxlevel_++;
        long long maxedge_ = (1<<maxlevel_);
        imin_.x = ((long long)floor(pmin_.x/maxedge_))*maxedge_;
        imin_.y = ((long long)floor(pmin_.y/maxedge_))*maxedge_;
        imin_.z = ((long long)floor(pmin_.z/maxedge_))*maxedge_;

        found = (imin_.x+2*maxedge_ >= pmax_.x) && (imin_.y+2*maxedge_ >= pmax_.y) && (imin_.z+2*maxedge_ >= pmax_.z);
    }
    if ((imin_.x+(1<<maxlevel_) < pmax_.x) || (imin_.y+(1<<maxlevel_) < pmax_.y) || (imin_.z+(1<<maxlevel_) < pmax_.z))
        maxlevel_++;

    int sublevel_ = std::min(maxlevel_,14);

    cfg->save(cfg->getFileName(XML_FILE));

    //! carving
    {
        //fprintf(stderr,"cfg1 = %p\n",cfg.get());
        ncVoxTree* carve_tree = new ncVoxTree(cfg->getFileName(GVM_RAW_FILE),true,true,NC_CHILD|NC_CARVE,imin_,maxlevel_,sublevel_,minlevel_,maxcarve_);

        ncCarve* carver = new ncCarve(carve_tree,cfg);
        connect(carver , SIGNAL(updateSignal()) , this , SLOT(updateSignal()));
        carver->run();
        disconnect(carver , SIGNAL(updateSignal()) , this , SLOT(updateSignal()));
        delete carver;

        carve_tree->save();
        delete carve_tree;
    }

    recarve_command();

}

void ncApp::recarve_command()
{
    //fprintf(stderr,"cfg2 = %p\n",cfg.get());

    //! optimizing
    fprintf(stderr,"optimizing....\n");
    ncVoxTree::copy(cfg->getFileName(GVM_OPT_FILE),cfg->getFileName(GVM_RAW_FILE),NC_CHILD|NC_CARVE|NC_RGBM);
    {
        ncVoxTree* opt = new ncVoxTree(cfg->getFileName(GVM_OPT_FILE),false,true);
        opt->optimizeCarve(cfg->vox.max_carve , cfg->vox.min_level);
        opt->save();
        delete opt;
    }

#if 1
    //! subdividing
    fprintf(stderr,"subdividing....\n");
    ncVoxTree::copy(cfg->getFileName(GVM_SUBDIV_FILE),cfg->getFileName(GVM_OPT_FILE),NC_CHILD|NC_RGBM);
    {
        ncVoxTree* subdiv = new ncVoxTree(cfg->getFileName(GVM_SUBDIV_FILE),false,true);
        subdiv->subDivide();
        subdiv->save();
        delete subdiv;
    }

    //! coloring
    fprintf(stderr,"coloring....\n");
    ncVoxTree::copy(cfg->getFileName(GVM_COLOR_FILE),cfg->getFileName(GVM_SUBDIV_FILE),NC_CHILD|NC_RGBM|NC_COLOR);
#else
    ncVoxTree::copy(cfg->getFileName(GVM_COLOR_FILE),cfg->getFileName(GVM_FILE_OPT),NC_CHILD|NC_RGBM|NC_COLOR);
#endif
    {
        ncGVM* color_tree = new ncGVM(cfg->getFileName(GVM_COLOR_FILE));
        color_tree->save_to_disk = true;

        fprintf(stderr,"color test =  %d\n",((color_tree->data_mask)&NC_COLOR ? 1 : 0));


        ncCarve* carver = new ncCarve(color_tree,cfg);
        carver->createColors();
        delete carver;
        color_tree->save();
        delete color_tree;
    }

    //ncVoxTree::copy(cfg->getFileName(GVM_FILE),cfg->getFileName(GVM_COLOR_FILE),NC_CHILD|NC_RGBM);
    {
        ncVoxTree* gvm_tree = new ncVoxTree(cfg->getFileName(GVM_COLOR_FILE),false,true);
        gvm_tree->createColors();
        gvm_tree->save();
        delete gvm_tree;
    }

    fprintf(stderr,"saving....\n");
    ncVoxTree::copy(cfg->getFileName(GVM_FILE),cfg->getFileName(GVM_COLOR_FILE),NC_CHILD|NC_RGBM);
    fprintf(stderr,"done....\n");

    loadGVM(cfg->getFileName(GVM_FILE));
}

void ncApp::remesh_command()
{
    /*
    cfg->save(cfg->getFileName(XML_FILE));
    ncCarve* carver = new ncCarve(cfg->getFileName(GVM_FILE),cfg);
	connect(carver , SIGNAL(updateSignal()) , this , SLOT(updateSignal()));
    carver->load(cfg->getFileName(GVM_FILE));
    carver->getMesh(cfg->vox.min_level);
	disconnect(carver , SIGNAL(updateSignal()) , this , SLOT(updateSignal()));
    delete carver;
    */
}

void ncApp::clobber_command()
{
    if(QMessageBox::question(this, tr("WARNING"), tr("This action will erase all flow data.\nAre you sure?"), QMessageBox::No, QMessageBox::Yes, QMessageBox::NoButton) == QMessageBox::Yes)
        cfg->clobber();
}

void ncApp::setVideoConfig()
{
    qt_video->loadPoseFile(cfg->project.poses.getPath());
    qt_video->loadCamFile(cfg->project.cameras.getPath());
    qt_video->begin_slider->setValue(cfg->project.poses.start_pose);
    qt_video->end_slider->setValue(cfg->project.poses.end_pose);
}

void ncApp::loadGVM(std::string fname)
{
    if (gvm)
        gvm.reset();
    gvm = new ncGVM(fname);
}

void ncApp::onKeyEvent(QKeyEvent* event)
{

    if (event->type()  ==  6/*QEvent::KeyPress*/)
        switch (event->key())
        {
        case Qt::Key_F :
            fixed = !fixed;
            if (fixed)
                MV_fixed  = qt_cam->MV();
            else
                qt_cam->setMV(MV_fixed);
            break;
        case Qt::Key_L :
            loadMDLs();
            updateGL();
            break;

        case Qt::Key_Plus :
            //fprintf(stderr,"TEST_INDEX = %d\n",++ncConfig::TEST_INDEX);
            cfg->my_tv_enum = (cfg->my_tv_enum+1)%nr_tv_enum;
            fprintf(stderr,"Setting TV mode to %s\n",tv_enum_name[cfg->my_tv_enum].c_str());
            updateGL();
            break;

        case Qt::Key_Minus :
            //fprintf(stderr,"TEST_INDEX = %d\n",--ncConfig::TEST_INDEX);
            cfg->my_tv_enum = (cfg->my_tv_enum+nr_tv_enum-1)%nr_tv_enum;
            fprintf(stderr,"Setting TV mode to %s\n",tv_enum_name[cfg->my_tv_enum].c_str());
            updateGL();
            break;

        /*
        case Qt::Key_D :
            flower->checkDepths();
            flower->writeModel();
            break;

        case Qt::Key_Plus :
            flower->incDval();
            flower->testSweep();
            updateGL();
            break;

        case Qt::Key_Minus :
            flower->decDval();
            flower->testSweep();
            updateGL();
            break;
        */
        }
}

void ncApp::onMouseEvent(QMouseEvent *event)
{

	qt_cam->processEvent(event);
}

void ncApp::onInitialize()
{
    cudaGLSetGLDevice(0);

    cWin    = new ncTexImageRect(GL_RGBA8,512,512);
    dWin    = new ncTexImageRect(GL_DEPTH_COMPONENT32F,512,512);

    if (ncc::checkFile(cfg->getFileName(XML_FILE).c_str()))
        cfg->load(cfg->getFileName(XML_FILE));

#if 0
    cutilSafeCall(cudaGraphicsGLRegisterImage (&fbo_resource, cWin->id, cWin->target, cudaGraphicsRegisterFlagsSurfaceLoadStore));
    vox_tree = new ncSubVoxTree("/usr/data/tmp/ncornelis/GeoToVox/gvm/testje.gvm");

    gvm.bpos.x     = vox_tree->bbox.pos[0];
    gvm.bpos.y     = vox_tree->bbox.pos[1];
    gvm.bpos.z     = vox_tree->bbox.pos[2];
    gvm.bsize.x    = vox_tree->bbox.size[0];
    gvm.bsize.y    = vox_tree->bbox.size[1];
    gvm.bsize.z    = vox_tree->bbox.size[2];
    gvm.maxlevel   = vox_tree->maxlevel;
    gvm.lod        = vox_tree->maxlevel;

    unsigned int data_size = vox_tree->voxels.size()*sizeof(cudaVox);
    cudaMalloc((void**)&gvm.voxels,data_size);
    cudaMemcpy(gvm.voxels,&(vox_tree->voxels[0]),data_size,cudaMemcpyHostToDevice);

    vox_tree->bbox.pos.print("bbox.pos");
    ncc::RTd tmp = qt_cam->MV();
    tmp[3][0] = vox_tree->bbox.pos[0] + vox_tree->transform[3][0];
    tmp[3][1] = vox_tree->bbox.pos[1] + vox_tree->transform[3][1];
    tmp[3][2] = vox_tree->bbox.pos[2] + vox_tree->transform[3][2];
    qt_cam->blockSignals(true);
    qt_cam->setMV(tmp);
    qt_cam->blockSignals(false);
#endif
}

void ncApp::onResize()
{
#if 0
    if (fbo_resource)
        cutilSafeCall(cudaGraphicsUnregisterResource(fbo_resource));
#endif

    cWin->resize(glw,glh);
    dWin->resize(glw,glh);

    qt_cam->blockSignals(true);
	qt_cam->setSize(ncc::v2i(glw,glh));
    qt_cam->blockSignals(false);

#if 0
    cutilSafeCall(cudaGraphicsGLRegisterImage (&fbo_resource, cWin->id, cWin->target, cudaGraphicsRegisterFlagsSurfaceLoadStore));
#endif
}

void  ncApp::loadMDLs()
{
#if 1

    for (unsigned int i = 0;i<mdls.size() ; ++i)
        delete mdls[i];
    mdls.clear();

    std::vector<int> cids = cfg->getActiveCids();

    for (unsigned int cc = 0 ; cc<cids.size();++cc)
    {
        ncIDList id_list = cfg->createIDList( cids[cc]);

        for (unsigned int i = 0 ; i<id_list.size();++i)
        {
            int cid = id_list[i].cid;
            int vid = id_list[i].vid;

            char fname[1024];
            sprintf(fname,"%s/%02d.%06d.mdl",cfg->getDirName(MODEL_DIR).c_str(),cid,vid);
            fprintf(stderr,"\rLoading %s",fname);
            if (ncc::checkFile(fname))
                mdls.push_back(new ncMDL(fname));
        }
    }
    fprintf(stderr,"\n");

#else

    for (unsigned int i = 0;i<mdls.size() ; ++i)
        delete mdls[i];
    mdls.clear();
    mdls.push_back(new ncMDL("/home/ncornelis/Desktop/test.mdl"));

#endif

}

void ncApp::onPaint()
{

    glViewport(0,0,glw,glh);
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    if (cfg->my_tv_enum == TV_3D)
    {

//! mdl
#if 1

    ncc::m4x4d  KGL = qt_cam->KGL();
    ncc::RTd    MVi = (fixed ? MV_fixed.inverse() : qt_cam->MV().inverse() );

#if 1
    glPointSize(1.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    for (unsigned int i = 0; i<mdls.size() ;++i)
        mdls[i]->draw(KGL,MVi);

    glDisable(GL_DEPTH_TEST);

    glPointSize(1.0f);
#endif

#if 0

    glPointSize(2.0f);

    glColorMask(GL_FALSE , GL_FALSE , GL_FALSE , GL_FALSE);
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_POINT_SMOOTH);
    for (unsigned int i = 0; i<mdls.size() ;++i)
        mdls[i]->draw(KGL,MVi,1.1f);
    glDisable(GL_POINT_SMOOTH);
    glDisable(GL_BLEND);

    glColorMask(GL_TRUE , GL_TRUE , GL_TRUE , GL_TRUE);
    glDepthMask(GL_FALSE);

    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_POINT_SMOOTH);
    for (unsigned int i = 0; i<mdls.size() ;++i)
        mdls[i]->draw(KGL,MVi);
    glDisable(GL_POINT_SMOOTH);
    glDisable(GL_BLEND);

    glDepthMask(GL_TRUE);

    glPointSize(1.0f);
#endif

#if 0
    int cid = qt_video->cam_spinbox->value();
    int vid = qt_video->pose_spinbox->value();
    char fname[1024];
    sprintf(fname,"%s/%02d.%06d.mdl",cfg->getDirName(MODEL_DIR).c_str(),cid,vid);
    ncPtr<ncMDL>  mdl = new ncMDL(fname);
    //glPointSize(2.0f);
    mdl->draw(KGL,MVi);
    //glPointSize(1.0f);
#endif

#endif

//! warp
#if 0
        ncc::m4x4d  KGL;
        ncc::RTd    MV;

        if (fixed)
        {
            KGL = KGL_fixed;
            MV  = MV_fixed;
        }
        else
        {
            KGL = qt_cam->KGL();
            MV  = qt_cam->MV();
        }

        int cid = qt_video->cam_spinbox->value();
        int vid = qt_video->pose_spinbox->value();
        int winlod = cfg->flow.winlod;


        ncPtr<ncTexImage2D> ccc,ddd;
        cfg->loadColorImage(ccc,cid,vid);
        cfg->loadDepthMap(ddd,cid,vid);

        if (ccc && ddd)
        {
            ncIndexArray2D* index2D = new ncIndexArray2D;
            index2D->setSize(ddd->width,ddd->height);

            ncCalibration cal = cfg->getCalibration(cid , vid);

            glViewport(0,0,glw,glh);

            glClearColor(0.0f,0.0f,0.0f,0.0f);
            glClearDepth(1.0f);
            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

            ncQtProgram* warp_prog       = new ncQtProgram(":/shaders/flow/warp.vert"        ,0,":/shaders/flow/warp.frag"       );

            warp_prog->Activate();
            warp_prog->setUniform("MVP"     , KGL * MV.inverse() * cal.MV() * cal.Ktex(winlod).inverse());
            warp_prog->setSampler("cTex"    , ccc,0);
            warp_prog->setSampler("dTex"    , ddd,1);

#if 1
            glDrawArrays(GL_POINTS,0,ddd->width*ddd->height);
#else
            glEnable(GL_DEPTH_CLAMP);
            glEnable(GL_PRIMITIVE_RESTART);
            glPrimitiveRestartIndex(0xFFFFFFFF);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index2D->getID());
            glDrawElements(GL_TRIANGLE_STRIP, index2D->getNrIndices(), GL_UNSIGNED_INT, 0 );
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0 );
            glDisable(GL_PRIMITIVE_RESTART);
            glDisable(GL_DEPTH_CLAMP);
#endif
            warp_prog->Deactivate();

            delete warp_prog;
            delete index2D;
        }
        ccc.reset();
        ddd.reset();
#endif

//! gvm
#if 0
    if (!gvm)
       loadGVM(cfg->getFileName(GVM_FILE));


    if (gvm)
    {
        glViewport(0,0,glw,glh);
        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        gvm->draw( qt_cam->KGL(),qt_cam->MV(),GL_CCW);
    }
#endif

#if 0
    float win_aspect = float(glw)/float(glh);
    float tex_aspect = float(tex->width)/float(tex->height);

    ncc::v4f tmp[4];
    if (win_aspect>tex_aspect)
    {
        float tmp = tex_aspect/win_aspect;
        tmp[0] = ncc::v4f(-tmp,-1.0f,0.0f,0.0f);
        tmp[1] = ncc::v4f(+tmp,-1.0f,1.0f,0.0f);
        tmp[2] = ncc::v4f(-tmp,+1.0f,0.0f,1.0f);
        tmp[3] = ncc::v4f(+tmp,+1.0f,1.0f,1.0f);
    }
    if (win_aspect>tex_aspect)
    {
        float tmp = win_aspect/tex_aspect;
        tmp[0] = ncc::v4f(-1.0f,-tmp,0.0f,0.0f);
        tmp[1] = ncc::v4f(+1.0f,-tmp,1.0f,0.0f);
        tmp[2] = ncc::v4f(-1.0f,+tmp,0.0f,1.0f);
        tmp[3] = ncc::v4f(+1.0f,+tmp,1.0f,1.0f);
    }


    if (flower->zTexFBO)
    {
        ncc::m4x4d cmat;
        cmat[0] = ncc::v4d(1.0,1.0,1.0,1.0);
        cmat[1] = ncc::v4d(0.0,0.0,0.0,0.0);
        cmat[2] = ncc::v4d(0.0,0.0,0.0,0.0);
        cmat[3] = ncc::v4d(0.0,0.0,0.0,0.0);
        flower->zTexFBO->draw(cmat);
    }
    else
    {
#if 0
        cudaGraphicsMapResources(1 , &fbo_resource);
        cudaGraphicsSubResourceGetMappedArray(&fbo_array, fbo_resource, 0, 0);

        ncc::RTd transform;
        transform.setIdentity();
        transform = vox_tree->transform;

        ncc::m4x4f MV   = transform.inverse()*qt_cam->MV();
        ncc::m4x4f Ki   = qt_cam->KGL().inverse();
        ncc::m4x4f MVP  = qt_cam->KGL()*(transform.inverse()*qt_cam->MV()).inverse();
        memcpy(&gvm.MV[0].x ,&MV[0] ,sizeof(ncc::m4x4f));
        memcpy(&gvm.Ki[0].x ,&Ki[0] ,sizeof(ncc::m4x4f));
        memcpy(&gvm.MVP[0].x,&MVP[0],sizeof(ncc::m4x4f));


        runTest(fbo_array,cWin->width,cWin->height,gvm);

        cudaGraphicsUnmapResources(1, &fbo_resource);

        ncgl::FBO1->bind(GL_READ_FRAMEBUFFER);
        ncgl::FBO1->attach(GL_COLOR_ATTACHMENT0,cWin,GL_READ_FRAMEBUFFER);
        glViewport(0,0,glw,glh);
        glBlitFramebuffer(0,0,cWin->width,cWin->height,0,0,glw,glh,GL_COLOR_BUFFER_BIT,GL_NEAREST);
        ncgl::FBO1->detach(GL_COLOR_ATTACHMENT0,GL_READ_FRAMEBUFFER);
        ncgl::FBO1->unbind(GL_READ_FRAMEBUFFER);

        //ncgl::checkErrors();
        //ncgl::checkFBO();
#endif
    }

#endif

    }
    else
    {
        unsigned int cid = qt_video->cam_spinbox->value();
        unsigned int vid = qt_video->pose_spinbox->value();
        glViewport(0,0,glw,glh);
        cfg->drawTexture(cfg->my_tv_enum,cid,vid);
    }

}

void ncApp::updateSignal()
{
    updateGL();
    qApp->processEvents();
}
