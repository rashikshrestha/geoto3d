#include "ncapp.h"

#include "resource.h"

using namespace std;

#include "ncqtprogram.h"

#include "ncfbo.h"
#include "ncundistort.h"

const int magic_width   = 160;
const int magic_height  = 1;
const unsigned char magicdata[640] = {0 , 0 , 255 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 255 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 255 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 255 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 255 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 255 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 255 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 255 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 255 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 255 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 255 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 255 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 255 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 255 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 255 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 255 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 255 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 255 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 255 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 255 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 255 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 255 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 255 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 };

ncApp::ncApp(QString title, ncQtMainWindow* mainw) : ncQtGLWidget(title, mainw, 0, Qt::Window) {

	//fprintf(stderr,"+ncApp\n");

	recording = false;

	pose_index = 0xFFFFFFFF;

	point_select = ncc::v4f(0.0f,0.0f,0.0f,0.0f);
	point_transform.setIdentity();

	//! params
	stereo_mode		= STEREO_OFF;
    ctrl			= false;
	update_fbo		= false;
	framenr			= 0;

	//! controls
	qt_cam          = new ncQtCamera("Camera Control"	        , mainw,Qt::RightDockWidgetArea  , Qt::LeftDockWidgetArea  |Qt::RightDockWidgetArea );
	qt_scenix       = new ncQtScenix("Scene Graph"	            , mainw,Qt::LeftDockWidgetArea   , Qt::LeftDockWidgetArea  |Qt::RightDockWidgetArea );
	qt_xymess       = new ncQtXYMessenger("XY Messenger"        , mainw,Qt::LeftDockWidgetArea   , Qt::LeftDockWidgetArea  |Qt::RightDockWidgetArea );
	qt_xymess->setVisible(false);
	qt_video        = new ncQtVideo("Video Control"		        , mainw,Qt::RightDockWidgetArea  , Qt::LeftDockWidgetArea  |Qt::RightDockWidgetArea );
	qt_logger       = new ncQtLogger("Logger"			        , mainw,Qt::BottomDockWidgetArea , Qt::BottomDockWidgetArea|Qt::TopDockWidgetArea   );

	//!+xy_mess
	point_label_x = new QLabel(  "X:  " + QString::number(0.0 , 'f' , 4));
	point_label_y = new QLabel(  "Y: "  + QString::number(0.0 , 'f' , 4));
	point_label_z = new QLabel(  "Z: "  + QString::number(0.0 , 'f' , 4));
	qt_xymess->grid->addWidget(point_label_x,0,0);
	qt_xymess->grid->addWidget(point_label_y,0,1);
	qt_xymess->grid->addWidget(point_label_z,0,2);

	QPushButton* xy_dp = new QPushButton("Send Data Point");
	qt_xymess->grid->addWidget(xy_dp,1,0,1,3);
	connect(xy_dp		, SIGNAL(clicked()) , this , SLOT(sendDataPoint()		));

	QPushButton* xy_dpcam = new QPushButton("Send Data Point With Camera");
	qt_xymess->grid->addWidget(xy_dpcam,2,0,1,3);
	connect(xy_dpcam	, SIGNAL(clicked()) , this , SLOT(sendDataPointCamera()	));
	//!-xy_mess

    connect(qt_video    , SIGNAL(poseChanged(ncc::RTd))            , this	, SLOT(setPose(ncc::RTd))                  );
    connect(qt_video    , SIGNAL(startRecord())                    , this	, SLOT(startRecord())                       );

	connect(qt_cam		, SIGNAL(log(QString)) , qt_logger , SLOT(log(QString)));
	connect(qt_xymess	, SIGNAL(log(QString)) , qt_logger , SLOT(log(QString)));
	connect(qt_video	, SIGNAL(log(QString)) , qt_logger , SLOT(log(QString)));
	connect(this		, SIGNAL(log(QString)) , qt_logger , SLOT(log(QString)));

	connect(qt_cam , SIGNAL(valuesChanged()) , this , SLOT(updateFBO()));
	connect(qt_scenix , SIGNAL(valuesChanged()) , this , SLOT(updateFBO()));
	connect(qt_scenix , SIGNAL(boundingSphereChanged(ncc::v3d,double)) , qt_cam , SLOT(boundingSphereChanged(ncc::v3d,double)));
}

ncApp::~ncApp() {

	delete	fWin;
	delete  cWin;
    delete  dWin;

	delete	fWin1;
	delete  cWin1;
	delete  iWin1;
    delete  dWin1;

	delete	iRead;

	delete  magicTex;

	delete	ball_prog;
	delete	cylinder_prog;
	delete	stereo_prog;
	delete	blit_prog;
}

void ncApp::updateFBO() {
	update_fbo = true;
    updateGL();
}

void ncApp::drawScene(ncc::m4x4d KGL , ncc::RTd MV , ncc::v4i VP) {

	//fprintf(stderr,"drawing scene...\n");

    qt_cam->drawBackGround();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    //glEnable(GL_BLEND);
    //glBlendEquation(GL_FUNC_ADD);
    //glBlendFunc(GL_CONSTANT_COLOR,GL_ONE);
    //glBlendColor(0.005,0.005,0.005,0.005);

    qt_scenix->draw(KGL,MV,VP);

    //glDisable(GL_BLEND);

    glDisable(GL_DEPTH_TEST);
}

void ncApp::drawFSrect() {

    ncc::v4f tmp[4];
    tmp[0] = ncc::v4f(-1.0f,-1.0f,0.0f,1.0f);
    tmp[1] = ncc::v4f(+1.0f,-1.0f,0.0f,1.0f);
    tmp[2] = ncc::v4f(-1.0f,+1.0f,0.0f,1.0f);
    tmp[3] = ncc::v4f(+1.0f,+1.0f,0.0f,1.0f);

    glBindBuffer(GL_ARRAY_BUFFER,0);
	glVertexAttribPointer((GLuint)0, 4, GL_FLOAT, GL_FALSE, 0, &tmp[0].x);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    glDisableVertexAttribArray(0);

}

void ncApp::drawSelection(ncc::m4x4d KGL , ncc::RTd MV) {

	if (point_select != ncc::v4f(0.0f,0.0f,0.0f,0.0f))
	{
		glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquation(GL_FUNC_ADD);

		glBindBuffer(GL_ARRAY_BUFFER,0);
        glVertexAttribPointer((GLuint)0, 4, GL_FLOAT, GL_FALSE, 0, &point_select.x);
		glEnableVertexAttribArray(0);

		ncc::RTd matMV = MV.inverse()*point_transform;

		ball_prog->Activate();
		ball_prog->setUniform("matMV",matMV);
		ball_prog->setUniform("matP",KGL);

		glDepthMask(GL_FALSE);
		ball_prog->setUniform("mColor",ncc::v4f(1.0f,0.0f,0.0f,0.75f));
		ball_prog->setUniform("mRadius",0.1f);
		glDrawArrays(GL_POINTS,0,1);
		glDepthMask(GL_TRUE);

		ball_prog->setUniform("mColor",ncc::v4f(0.0f,1.0f,0.0f,1.0f));
		ball_prog->setUniform("mRadius",0.01f);
		glDrawArrays(GL_POINTS,0,1);

		ball_prog->Deactivate();

		glDisableVertexAttribArray(0);

		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);

    }
}

void ncApp::drawPoses(ncc::m4x4d KGL , ncc::RTd MV) {
	if (qt_video->poses)
	{
		glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		ncc::RTd matMV = MV.inverse()*qt_video->poses->getTransform();

		cylinder_prog->Activate();
        cylinder_prog->setUniform("matMV",matMV);
		cylinder_prog->setUniform("matP",KGL);
		cylinder_prog->setUniform("mColor",ncc::v4f(0.0f,1.0f,0.0f,1.0f));
		cylinder_prog->setUniform("mRadius",0.0125f);
		cylinder_prog->setUniform("model_ID",POSE_ID);
		qt_video->drawPoses(GL_LINE_STRIP);
		cylinder_prog->Deactivate();

		ball_prog->Activate();
        ball_prog->setUniform("matMV",matMV);
		ball_prog->setUniform("matP",KGL);
		ball_prog->setUniform("mColor",ncc::v4f(0.0f,0.0f,1.0f,1.0f));
		ball_prog->setUniform("mRadius",0.0175f);
		ball_prog->setUniform("model_ID",POSE_ID);
		qt_video->drawPoses(GL_POINTS);
		ball_prog->Deactivate();

		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
	}
}

void ncApp::drawCameras(ncc::m4x4d KGL , ncc::RTd MV) {
	if (qt_video->poses && qt_video->calset && (pose_index != 0xFFFFFFFF))
	{
		glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		ncc::RTd matMV = MV.inverse()*(*(qt_video->poses))[pose_index];

		cylinder_prog->Activate();
        cylinder_prog->setUniform("matMV",matMV);
		cylinder_prog->setUniform("matP",KGL);
		cylinder_prog->setUniform("mColor",ncc::v4f(1.0f,0.0f,0.0f,1.0f));
		cylinder_prog->setUniform("mRadius",0.0125f);
		cylinder_prog->setUniform("model_ID",CAM_ID);
		qt_video->drawCameras(GL_LINES);
		cylinder_prog->Deactivate();

		ball_prog->Activate();
        ball_prog->setUniform("matMV",matMV);
		ball_prog->setUniform("matP",KGL);
		ball_prog->setUniform("mColor",ncc::v4f(1.0f,0.0f,0.0f,1.0f));
		ball_prog->setUniform("mRadius",0.025f);
		ball_prog->setUniform("model_ID",CAM_ID);
		qt_video->drawCameras(GL_POINTS);
		ball_prog->Deactivate();

		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
	}
}

void ncApp::onResize() {

	//fprintf(stderr,"+onResize %dx%d\n",glw,glh);

	iRead->bind(GL_FRAMEBUFFER);
	iRead->detach(GL_COLOR_ATTACHMENT0);
	iRead->unbind(GL_FRAMEBUFFER);

	fWin1->bind(GL_FRAMEBUFFER);
	fWin1->detach(GL_COLOR_ATTACHMENT0);
	fWin1->detach(GL_COLOR_ATTACHMENT1);
	fWin1->detach(GL_DEPTH_ATTACHMENT );
	cWin1->resize(glw,glh);
	iWin1->resize(glw,glh);
	dWin1->resize(glw,glh);
	fWin1->attach(GL_COLOR_ATTACHMENT0,cWin1);
	fWin1->attach(GL_COLOR_ATTACHMENT1,iWin1);
	fWin1->attach(GL_DEPTH_ATTACHMENT ,dWin1);
	fWin1->unbind(GL_FRAMEBUFFER);

	iRead->bind(GL_FRAMEBUFFER);
	iRead->attach(GL_COLOR_ATTACHMENT0,iWin1);
	iRead->unbind(GL_FRAMEBUFFER);

	fWin->bind(GL_FRAMEBUFFER);
	fWin->detach(GL_COLOR_ATTACHMENT0);
	fWin->detach(GL_DEPTH_ATTACHMENT);
	cWin->resize(glw,glh);
	dWin->resize(glw,glh);
	fWin->attach(GL_COLOR_ATTACHMENT0,cWin);
	fWin->attach(GL_DEPTH_ATTACHMENT ,dWin);
	fWin->unbind(GL_FRAMEBUFFER);

    qt_cam->blockSignals(true);
	qt_cam->setSize(ncc::v2i(glw,glh));
    qt_cam->blockSignals(false);
    update_fbo = true;

	//fprintf(stderr,"-onResize %dx%d\n",glw,glh);

}

void ncApp::onInitialize() {
	//fprintf(stderr,"+ncApp::onInitialize\n");

	log(QString("<FONT COLOR=blue><b>OpenGL Version: %1</b></FONT>").arg(QString((char*)glGetString(GL_VERSION))));

	fWin = new ncFBO;
	cWin = new ncTexImageRect(GL_RGBA8,glw,glh);
	dWin = new ncTexImageRect(GL_DEPTH_COMPONENT32,glw,glh);

	fWin1 = new ncFBO;
	cWin1 = new ncTexImageRect(GL_RGBA8,glw,glh);
	iWin1 = new ncTexImageRect(GL_R32I ,glw,glh);
	dWin1 = new ncTexImageRect(GL_DEPTH_COMPONENT32,glw,glh);

	iRead = new ncFBO;
	magicTex = new ncTexImageRect(GL_RGBA8,magic_width,magic_height);
	magicTex->setData((void*)&magicdata[0]);

	//! shaders
	ball_prog		= new ncQtProgram(":/shaders/ball.vert"		, ":/shaders/ball.geom"		, ":/shaders/ball.frag"		);
	cylinder_prog   = new ncQtProgram(":/shaders/cylinder.vert" , ":/shaders/cylinder.geom" , ":/shaders/cylinder.frag"	);
	stereo_prog		= new ncQtProgram(":/shaders/stereo.vert"	, 0							, ":/shaders/stereo.frag"	);
	blit_prog		= new ncQtProgram(":/shaders/blit.vert"		, 0							, ":/shaders/blit.frag"		);

	qt_scenix->initialize();

    //fprintf(stderr,"-ncApp::onInitialize\n");
}

void ncApp::onPaint() {

    //fprintf(stderr,"|");

#if 0
    fWin1->bind(GL_FRAMEBUFFER);
    glViewport(0,0,glw,glh);
    glClearColor(1.0f,0.0f,1.0f,0.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawScene(qt_cam->KGL(),qt_cam->MV());
	fWin1->unbind(GL_FRAMEBUFFER);

	glViewport(0,0,glw,glh);
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    blit_prog->Activate();
    blit_prog->setSampler("tex", cWin1 ,0);
    drawFSrect();
    blit_prog->Deactivate();

#else
	//fprintf(stderr,"+ncApp::onPaint\n");

	//fprintf(stderr,"painting %d\n",framenr++);

	int         glw2    = glw/2;
	ncc::v4i	vp0		= ncc::v4i(   0 , 0 , glw  , glh);
	ncc::v4i	vp1		= ncc::v4i(   0 , 0 , glw2 , glh);
	ncc::v4i	vp2		= ncc::v4i(glw2 , 0 , glw2 , glh);
	ncc::m4x4d	KGL1	= qt_cam->KGL();
	ncc::RTd	MV1		= qt_cam->MV();
	ncc::m4x4d	KGL2	= KGL1;
	ncc::RTd	MV2		= MV1;
	float       K22		= KGL1[2][2];
	float       K32		= KGL1[3][2];

	ncc::v4d baseline = MV2*ncc::v4d(0.5,0.0,0.0,0.0);
	MV2[3]-=baseline;

	//! draw to fWin
	if (update_fbo)
	{
		if (stereo_mode == STEREO_OFF)
		{
            fWin->bind(GL_FRAMEBUFFER);

            glViewport(0,0,glw,glh);
            ncc::v4ub bgcolor = qt_cam->getColor();
            glClearColor(bgcolor[0]/255.0f,bgcolor[1]/255.0f,bgcolor[2]/255.0f,bgcolor[3]/255.0f);
            glClearDepth(1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			drawScene(KGL1,MV1,vp0);

            fWin->unbind(GL_FRAMEBUFFER);
		}
		else if (stereo_mode == STEREO_ACTIVE)
		{
            fWin->bind(GL_FRAMEBUFFER);

            glViewport(0,0,glw,glh);
            ncc::v4ub bgcolor = qt_cam->getColor();
            glClearColor(bgcolor[0]/255.0f,bgcolor[1]/255.0f,bgcolor[2]/255.0f,bgcolor[3]/255.0f);
            glClearDepth(1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glEnable(GL_SCISSOR_TEST);
			glScissor(vp1.x,vp1.y,vp1.z,vp1.w);
			glViewport(vp1.x,vp1.y,vp1.z,vp1.w);
			drawScene(KGL1,MV1,vp1);
			glScissor(vp2.x,vp2.y,vp2.z,vp2.w);
			glViewport(vp2.x,vp2.y,vp2.z,vp2.w);
			drawScene(KGL2,MV2,vp2);
            glDisable(GL_SCISSOR_TEST);

            fWin->unbind(GL_FRAMEBUFFER);
		}
		else if (stereo_mode == STEREO_PASSIVE)
		{
            fWin->bind(GL_FRAMEBUFFER);

            glViewport(0,0,glw,glh);
            ncc::v4ub bgcolor = qt_cam->getColor();
            glClearColor(bgcolor[0]/255.0f,bgcolor[1]/255.0f,bgcolor[2]/255.0f,bgcolor[3]/255.0f);
            glClearDepth(1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glViewport(vp1.x,vp1.y,vp1.z,vp1.w);
			drawScene(KGL1,MV1,vp1);

			//depth test
			float* test = new float[glw2*glh];
			glReadPixels(0,0,glw2,glh,GL_DEPTH_COMPONENT,GL_FLOAT,test);

			float	mean		= 0.0f;
			float	variance	= 0.0f;
			int		tweight		= 0;

			for (int i=0;i<glw2*glh;++i)
			{
				if (test[i]!=1.0f)
				{
					float z  = (K32)/((2.0f*test[i]-1.0f)-K22);
					mean	+= z;
					variance+= z*z;
					tweight++;
				}
			}
			delete[] test;

			mean    /=tweight;
			variance/=tweight;

			float deviation = sqrt(variance-mean*mean);

			stereo_znear = (std::max)(0.0f,mean-2*deviation);
			stereo_zfar  = mean+2*deviation;

			//fprintf(stderr,"zcam   = [%f,%f]\n",qt_cam->getZnear(),qt_cam->getZfar());
			//fprintf(stderr,"drange = [%f,%f]\n",minz,maxz);
			//fprintf(stderr,"zrange = [%f,%f]\n",stereo_znear,stereo_zfar);

            fWin->unbind(GL_FRAMEBUFFER);
		}

		update_fbo = false;
	}

	if(ctrl && (stereo_mode == STEREO_OFF))
		selectPoint(qt_cam->getMousePos().x(),glh-1-qt_cam->getMousePos().y());

	//! draw to fWin1
    fWin->bind(GL_READ_FRAMEBUFFER);
    fWin1->bind(GL_DRAW_FRAMEBUFFER);
    glBlitFramebuffer(0,0,glw,glh,0,0,glw,glh,GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT , GL_NEAREST);
    fWin1->unbind(GL_DRAW_FRAMEBUFFER);
    fWin->unbind(GL_READ_FRAMEBUFFER);


    fWin1->bind(GL_FRAMEBUFFER);

	if (stereo_mode == STEREO_OFF)
	{
		glViewport(0,0,glw,glh);
		drawSelection(KGL1,MV1);
	}

	GLenum drawbuffers[] = {GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1};
	glDrawBuffers(2,drawbuffers);
	int id_clear = 0xFFFFFFFF;
	glViewport(0,0,glw,glh);
	glClearBufferiv(GL_COLOR, 1,&id_clear);

#if 0
	if ((stereo_mode == STEREO_OFF) || (stereo_mode == STEREO_PASSIVE))
	{
		glViewport(0,0,glw,glh);
		drawPoses(KGL1,MV1);
		drawCameras(KGL1,MV1);
	}
	if (stereo_mode == STEREO_ACTIVE)
	{
		glViewport(vp1.x,vp1.y,vp1.z,vp1.w);
		drawPoses(KGL1,MV1);
		drawCameras(KGL1,MV1);
		glViewport(vp2.x,vp2.y,vp2.z,vp2.w);
		drawPoses(KGL2,MV2);
		drawCameras(KGL2,MV2);
	}
#endif

	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	fWin1->unbind(GL_FRAMEBUFFER);



    //! blit to default FBO
	if (stereo_mode == STEREO_PASSIVE)
	{
		//fWin1->bind(GL_READ_FRAMEBUFFER);

		glViewport(0,0,glw,glh);
		glClearColor(0.0f,0.0f,0.0f,0.0f);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cWin1->setFilters(GL_LINEAR,GL_LINEAR);
		dWin1->setFilters(GL_LINEAR,GL_LINEAR);

		glDisable(GL_DEPTH_TEST);
		glDepthFunc(GL_ALWAYS);
		stereo_prog->Activate();
		stereo_prog->setUniform("K22"       , K22);
		stereo_prog->setUniform("K32"       , K32);
		stereo_prog->setUniform("Zn"        , stereo_znear);
		stereo_prog->setUniform("Zf"        , stereo_zfar);
		stereo_prog->setSampler("cTex"      , cWin1      ,0);
		stereo_prog->setSampler("dTex"      , dWin1      ,1);
		stereo_prog->setSampler("magicTex"  , magicTex   ,2);

		drawFSrect();
		glDisable(GL_DEPTH_TEST);

		//fWin1->unbind(GL_READ_FRAMEBUFFER);
	}


	if ((stereo_mode == STEREO_OFF) || (stereo_mode == STEREO_ACTIVE))
	{
		glViewport(0,0,glw,glh);
		glClearColor(0.0f,0.0f,0.0f,0.0f);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		blit_prog->Activate();
		blit_prog->setSampler("tex", cWin1 ,0);
		drawFSrect();
		blit_prog->Deactivate();
	}

	 if (recording)
        glReadPixels(0,0,glw,glh,GL_BGRA,GL_UNSIGNED_BYTE,video_bgra_buf);

	//fprintf(stderr,"-ncApp::onPaint\n");

#endif
}

void ncApp::setPose(ncc::RTd mat) {
	qt_cam->setMV(mat);
	//updateFBO();
}

void ncApp::onMouseEvent(QMouseEvent *event) {

	if (!qt_video->isPlaying())
	{
	    ctrl = ((event->modifiers() & Qt::ControlModifier) != 0);
        qt_cam->processEvent(event);
        if (ctrl && (event->type() == QEvent::MouseMove) && (event->buttons()==0))
            updateGL();
	}
}

void ncApp::onKeyEvent(QKeyEvent* event) {

	//fprintf(stderr,"mykeyevent...\n");

    //if (!qt_video->isPlaying())
	{
        if (event->type()  == 6/*QEvent::KeyPress*/ )
            switch (event->key())
            {
            case Qt::Key_F5 :
                toggleStereo();
                break;
#ifndef WIN32
			case Qt::Key_Plus :
                ncSubGVM::lod++;
                fprintf(stderr,"ncSubGVM::lod = %d\n",ncSubGVM::lod);
                updateFBO();
                break;
            case Qt::Key_Minus :
                if (ncSubGVM::lod>0)
                {
                    ncSubGVM::lod--;
                    fprintf(stderr,"ncSubGVM::lod = %d\n",ncSubGVM::lod);
                    updateFBO();
                }
				break;
#endif
            }
	}
}

void ncApp::selectPoint(int x, int y) {
	point_select = ncc::v4f(0.0f,0.0f,0.0f,0.0f);
	point_transform.setIdentity();

	if ((x>=0) && (x<glw) && (y>=0) && (y<glh))
	{
		float depth;
		fWin->bind(GL_FRAMEBUFFER);
		glReadPixels(x,y,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&depth);
		if (depth < 1.0f)
		{
			ncc::v4f tmp(2.0f*(x+0.5f)/glw -1.0f , 2.0f*(y+0.5f)/glh - 1.0f,2.0f*depth-1.0f,1.0f);
			point_select = qt_cam->KGL().inverse()*tmp;
			point_select /= point_select.w;
			point_transform = qt_cam->MV();
		}

		fWin->unbind(GL_FRAMEBUFFER);

		//!index
		if (qt_video->poses)
		{
			iRead->bind(GL_FRAMEBUFFER);
			int tmp;
			glReadPixels(x,y,1,1,GL_RED_INTEGER,GL_INT,&tmp);
			iRead->unbind(GL_FRAMEBUFFER);

			if (tmp!=0xFFFFFFFF)
			{
				int id  = (tmp>>24)&0xFF;
				int val = tmp&0x00FFFFFF;

				switch(id)
				{
				case CAM_ID:
					fprintf(stderr,"cam_index = %d\n",val);
					break;
				case POSE_ID:
					pose_index=qt_video->poses->getMinVid()+val;
					fprintf(stderr,"pose_index = %d\n",pose_index);
					break;
				}
			}
		}
	}

	ncc::v4d tmp = point_transform*point_select;
	point_label_x->setText("X: " + QString::number(tmp.x , 'f' , 4));
	point_label_y->setText("Y: " + QString::number(tmp.y , 'f' , 4));
	point_label_z->setText("Z: " + QString::number(tmp.z , 'f' , 4));
	point_label_x->repaint();
	point_label_y->repaint();
	point_label_z->repaint();
}

void ncApp::toggleStereo() {
    stereo_mode = (stereo_mode+1)%3;
	log(QString("Setting Stereo mode to: %1").arg(QString::number(stereo_mode)));
	updateFBO();
}

void ncApp::startRecord() {

    QString fileName = QFileDialog::getSaveFileName(this,tr("Save Video File\n"),0, "AVI files (*.avi)");
	qApp->processEvents();
	if (fileName.isEmpty())
        return;

    if (QFileInfo(fileName).suffix() != "avi")
        fileName += ".avi";

  	char video_file[1024];
	sprintf(video_file,"%s",fileName.toStdString().c_str());

    //! capture old state
    QSize sz        = this->size();
    QSize minsz     = this->minimumSize();
    QSize maxsz     = this->maximumSize();
    QSizePolicy szp = this->sizePolicy();
    bool wasFullScr = this->isFullScreen();

    if (!wasFullScr)
        this->toggleFullScreen();

    //! set new state
    int w       = 1024;//1280;//1920;
    int h       = 576;//720;//1080;
    int bitrate = 10000000;
    int fps     = 10;

    QSize fixedSize(w,h);
    setMinimumSize(fixedSize);
    setMaximumSize(fixedSize);
    setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    int start_pose  = qt_video->begin_slider->value();
    int stop_pose   = qt_video->end_slider->value();

    ncPoseList*         poses   = qt_video->poses;
    ncCalibrationSet*   calset  = qt_video->calset;

	std::vector<int> rec_poses;
	for (int p = start_pose;p<stop_pose;++p)
		if (qt_video->static_poses->isChecked() || poses->isValid(p))
			rec_poses.push_back(p);

    int cid         = qt_video->cam_spinbox->value();


	//! initialize progress dialog
    char bleh[1024];
    sprintf(bleh,"Recording video [%d->%d]...",start_pose,stop_pose);

	QProgressDialog progress(bleh, "Abort", 0, rec_poses.size());
    progress.setWindowModality(Qt::ApplicationModal);
    progress.show();

	ncCodec encoder;
	encoder.start(video_file,w,h,bitrate,fps);
	video_bgra_buf = encoder.getVideoBuffer();

	bool canceled = false;
    for (unsigned int i = 0; i < rec_poses.size(); i++) {

		progress.setValue(i);
        progress.update();
        progress.repaint();

        recording = true;

        ncc::RTd MVpose = (*poses)[rec_poses[i]];
        if (calset)
            setPose(MVpose*(*calset)[cid].MV());
        else
            setPose(MVpose);

fprintf(stderr,"+encoder.addFrame\n");
        encoder.addFrame(true);
fprintf(stderr,"-encoder.addFrame\n");

        if (progress.wasCanceled())
		{
			canceled = true;
            break;
		}
	}
    recording = false;
	encoder.finish();
	progress.setValue(rec_poses.size());

	video_bgra_buf = 0;

	if (canceled)
		unlink(video_file);

    //! reset old state
    if (!wasFullScr)
        this->toggleFullScreen();

    setSizePolicy(szp);
    setMinimumSize(minsz);
    setMaximumSize(maxsz);
    resize(sz);
}

void ncApp::sendDataPoint() {

	ncc::v4d p3d = point_transform*point_select;
	QString message = "##3#" + QString::number(p3d.x , 'f' , 4) + " " + QString::number(p3d.y , 'f' , 4) + " " + QString::number(p3d.z , 'f' , 4);
	qt_xymess->sendMessage("dp", message);
}

void ncApp::sendDataPointCamera() {

	ncc::v4d p3d = point_transform*point_select;
	QString message = "##3#" + QString::number(p3d.x , 'f' , 4) + " " + QString::number(p3d.y , 'f' , 4) + " " + QString::number(p3d.z , 'f' , 4);
    qt_xymess->sendMessage("dpcam", message);
}

