#include "ncqtglwidget.h"

#include "ncgl.h"

//! ncQtGLWidget

ncQtGLWidget::ncQtGLWidget(QString title, ncQtMainWindow * mainw_ , QGLWidget * shareWidget, Qt::WindowFlags f) : QGLWidget(0,shareWidget,f) {

    instance = this;

	mainw			= mainw_;
    initialize_gl	= true;
	resize_gl		= false;

	setWindowFlags(f);

	dummy_widget = new QWidget(0,Qt::FramelessWindowHint);
	dummy_layout = new QGridLayout;
	dummy_layout->setContentsMargins(0,0,0,0);
	dummy_layout->addWidget(this,0,0);
	dummy_widget->setLayout(dummy_layout);

	central_layout = new QGridLayout;
	central_layout->setContentsMargins(0,0,0,0);
	central_layout->addWidget(dummy_widget,0,0);

	mainw->central_widget->setLayout(central_layout);

	setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    setMinimumSize(768,576);

	setWindowTitle(title);

    //!global
	show_window = new QAction ("GL Window", this);
	show_window->setCheckable(true);
	show_window->setChecked(true);
	mainw->windows_menu->addAction(show_window);

	connect(show_window, SIGNAL(toggled(bool)) , this , SLOT(	setVisible(bool)));

}

ncQtGLWidget::~ncQtGLWidget() {
}

ncQtGLWidget* ncQtGLWidget::instance = 0;

bool ncQtGLWidget::Initialized() {
	return !initialize_gl;
}

void ncQtGLWidget::toggleFullScreen() {
    if (dummy_widget->isFullScreen())
    {
        dummy_widget->showNormal();
        central_layout->addWidget(dummy_widget,0,0);
        this->setFocus(Qt::OtherFocusReason);
    }
    else
    {
        central_layout->removeWidget(dummy_widget);
        dummy_widget->setParent(0);
        dummy_widget->showFullScreen();
    }
}

bool ncQtGLWidget::isFullScreen() {
    return dummy_widget->isFullScreen();
}

void ncQtGLWidget::mousePressEvent(QMouseEvent *event) {
    onMouseEvent(event);
}

void ncQtGLWidget::mouseReleaseEvent(QMouseEvent *event) {
    onMouseEvent(event);
}

void ncQtGLWidget::mouseMoveEvent(QMouseEvent *event) {
    onMouseEvent(event);
}

void ncQtGLWidget::keyPressEvent(QKeyEvent * event) {

    if (event->type()  ==  6/*QEvent::KeyPress*/)
        switch (event->key())
        {
        case Qt::Key_F11 :
			toggleFullScreen();
			break;
		}

    onKeyEvent(event);
}

void ncQtGLWidget::keyReleaseEvent(QKeyEvent * event) {
    onKeyEvent(event);
}

void ncQtGLWidget::closeEvent(QCloseEvent* /*ce*/) {
	show_window->setChecked(false);
}

void ncQtGLWidget::paintGL() {
    //fprintf(stderr,"+ncQtGLWidget::paintGL()\n");

	if (initialize_gl)
	{
		ncgl::init();
		onInitialize();
		initialize_gl = false;
	}

	if (resize_gl)
	{
		onResize();
		resize_gl = false;
	}

	onPaint();
	checkGL();
    //fprintf(stderr,"-ncQtGLWidget::paintGL()\n");

};

void ncQtGLWidget::resizeGL(int w, int h) {
    //fprintf(stderr,"+ncQtGLWidget::resizeGL()\n");
	resize_gl = true;
	glw = w;
	glh = h;
	resized(w,h);
    //fprintf(stderr,"-ncQtGLWidget::resizeGL()\n");
};

void ncQtGLWidget::checkGL() {
	GLenum error;
    do {
		error = glGetError();
        if (error != GL_NO_ERROR)
			log(QString("<font color=red>GL Error (%1): %2 </font>").arg(QString::number(error, 16).toUpper()).arg(QString((char *) gluErrorString(error))));
	} while (error != GL_NO_ERROR);
}
