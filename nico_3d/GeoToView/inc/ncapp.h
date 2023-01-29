#ifndef NCAPP_H
#define NCAPP_H

#include "ncgl.h"
#include "ncqtglwidget.h"
#include "ncqtscenix.h"
#include "ncqtcamera.h"
#include "ncqtvideo.h"
#include "ncqtxymessenger.h"
#include "ncqtlogger.h"
#include "nccodec.h"
#include "ncprogram.h"
#include "ncconfig.h"

#ifndef WIN32
#include "ncvoxel.h"
#endif

enum model_ID {CAM_ID , POSE_ID};
enum stereo_ID { STEREO_OFF , STEREO_ACTIVE , STEREO_PASSIVE };

class ncApp : public ncQtGLWidget {

    Q_OBJECT

public:
	ncApp(QString title, ncQtMainWindow* mainw);
    ~ncApp();

public slots:
	void setPose(ncc::RTd mat);
	void updateFBO();
	void startRecord();
	void sendDataPoint();
	void sendDataPointCamera();

protected:
	void onInitialize();
    void onResize();
    void onPaint();
    void onMouseEvent(QMouseEvent *event);
    void onKeyEvent(QKeyEvent* event);

	void selectPoint(int x, int y);
	void drawScene(ncc::m4x4d KGL , ncc::RTd MV , ncc::v4i VP);
	void drawFSrect();
	void drawSelection(ncc::m4x4d KGL , ncc::RTd MV);
	void toggleStereo();
	void drawPoses(ncc::m4x4d KGL , ncc::RTd MV);
	void drawCameras(ncc::m4x4d KGL , ncc::RTd MV);


public:

    uint8_t*            video_bgra_buf;
    bool                recording;

	int					stereo_mode;
	float				stereo_znear;
	float				stereo_zfar;

    ncTexImageRect*     magicTex;

	bool				ctrl;
	bool                update_fbo;

	ncFBO*				fWin;
	ncTexImageRect*     cWin;
    ncTexImageRect*     dWin;

	ncFBO*				fWin1;
	ncTexImageRect*     cWin1;
	ncTexImageRect*     iWin1;
    ncTexImageRect*     dWin1;

	ncFBO*				iRead;

	int					pose_index;
	ncc::v4f			point_select;
	ncc::RTd			point_transform;
	QLabel*				point_label_x;
	QLabel*				point_label_y;
	QLabel*				point_label_z;

    ncQtVideo*          qt_video;
	ncQtXYMessenger*	qt_xymess;
	ncQtLogger*			qt_logger;
    ncQtCamera*         qt_cam;
    ncQtScenix*         qt_scenix;

    ncProgram*		    ball_prog;
    ncProgram*		    cylinder_prog;
    ncProgram*		    stereo_prog;
    ncProgram*		    blit_prog;

	int					framenr;
};

#endif // NCAPP_H
