#ifndef NCAPP_H
#define NCAPP_H

#include "ncgl.h"
#include "ncqtglwidget.h"
#include "ncqtcamera.h"
#include "ncqtvideo.h"
#include "ncqtlogger.h"
#include "ncqtconfig3D.h"

#include "ncflow.h"
#include "nccarve.h"
#include "ncconfig.h"


class ncApp : public ncQtGLWidget
{
    Q_OBJECT

public:
	ncApp(QString title, ncQtMainWindow* mainw);

public slots:
    void updateSignal();
    void flow_command();
    void carve_command();
    void recarve_command();
    void remesh_command();
    void clobber_command();
    void setVideoConfig();
    void loadGVM(std::string fname);
    void loadMDLs();

protected:
    void onPaint();
    void onInitialize();
    void onResize();
    void onKeyEvent(QKeyEvent* event);
    void onMouseEvent(QMouseEvent *event);

public:

    ncPtr<ncQtCamera>   qt_cam;
    ncPtr<ncQtVideo>    qt_video;
	ncPtr<ncQtLogger>   qt_logger;
	ncPtr<ncQtConfig3D>   cfg;

private:

    ncPtr<ncTexImageRect>     cWin;
    ncPtr<ncTexImageRect>     dWin;

    int                 counter;
    char                mystr[1024];
    ncPtr<ncGVM>        gvm;

    ncc::RTd            MV_fixed;
    bool                fixed;

    std::vector<ncMDL* > mdls;
};

#endif
