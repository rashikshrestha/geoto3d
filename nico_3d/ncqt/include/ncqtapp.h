#ifndef __NC_QT_APP_H__
#define __NC_QT_APP_H__

#include "ncqt.h"
#include "ncgl.h"
#include "ncqtcamera.h"
#include "ncqtdock.h"
#include "ncqtlogger.h"
#include "ncqtmodel.h"
#include "ncqtprogressbar.h"
#include "ncqtvideo.h"
#include "ncqtmainwindow.h"

class ncQtGLWidget : public QGLWidget {

    Q_OBJECT

public:
    ncQtGLWidget(QString title, ncQtMainWindow * mainw , int major, int minor, QGL::FormatOptions options, QGLFormat::OpenGLContextProfile profile , const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0 );
    ~ncQtGLWidget();

	void log(char* fmt , ...) {
		va_list argptr;
		va_start(argptr,fmt);
		char logtxt[1024];
		vsprintf(logtxt,fmt,argptr);
        qt_logger->log(logtxt);
		va_end(argptr);
	}

	void postInit();

	void setProgress(std::string text,int value);

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent * event);

    virtual void mouseEvent(QMouseEvent*) {};
    virtual void keyEvent(QKeyEvent*) {};

	void closeEvent(QCloseEvent*);

public:
    ncQtProgressBar*	qt_pbar;
    ncQtLogger*		    qt_logger;
    ncQtMainWindow*     main_window;
	QAction*			show_window;

private:
	QWidget*			dummy_widget;
	QGridLayout*		dummy_layout;
	QGridLayout*		central_layout;
	ncQtMainWindow *	mainw;
};

#endif
