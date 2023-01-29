#ifndef __NCQT_GLWIDGET_H__
#define __NCQT_GLWIDGET_H__

#include "ncgl.h"
#include "ncqtmainwindow.h"

class ncQtGLWidget : public QGLWidget {

    Q_OBJECT

public:
    ncQtGLWidget(QString title, ncQtMainWindow * mainw_ , QGLWidget * shareWidget, Qt::WindowFlags f);
    ~ncQtGLWidget();
    void toggleFullScreen();
    bool isFullScreen();
    bool Initialized();
    static ncQtGLWidget* instance;

signals:
	void log(QString text);
	void resized(int w, int h);

protected:

	bool hasHeightForWidth();

	virtual void onMouseEvent(QMouseEvent*) {};
    virtual void onKeyEvent(QKeyEvent*) {};
    virtual void onPaint() {};
    virtual void onResize() {};
    virtual void onInitialize() {};

	void checkGL();

    int glw;
    int glh;

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent * event);
	void closeEvent(QCloseEvent*);
	virtual void paintGL();
	virtual void resizeGL(int w, int h);

	QAction*			show_window;
	QWidget*			dummy_widget;
	QGridLayout*		dummy_layout;
	QGridLayout*		central_layout;
	ncQtMainWindow *	mainw;
	bool				initialize_gl;
	bool                resize_gl;

};

#endif //__NCQT_GLWIDGET_H__
