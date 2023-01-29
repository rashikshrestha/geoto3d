#include "ncQtGLWidget.h"

ncQtGLWidget::ncQtGLWidget(QString title, ncQtMainWindow * mainw_ ,int major, int minor, QGL::FormatOptions options, QGLFormat::OpenGLContextProfile profile , const QGLWidget * shareWidget, Qt::WindowFlags f) : QGLWidget(0,shareWidget,f) {

	mainw = mainw_;
	
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

    QGLFormat form(options);
	form.setVersion(major,minor);
	form.setProfile(profile);
	setFormat(form);

	//setAutoFillBackground(false);

	main_window = mainw;

    //! logger
	qt_logger	= new ncQtLogger(this);

    //! progressbar
    qt_pbar	    = new ncQtProgressBar(this);

    //!global
	show_window = new QAction ("GL Window", this);
	show_window->setCheckable(true);
	show_window->setChecked(true);
	mainw->windows_menu->addAction(show_window);

	connect(show_window, SIGNAL(toggled(bool)) , this , SLOT(	setVisible(bool)));
}

ncQtGLWidget::~ncQtGLWidget() {
}

void ncQtGLWidget::postInit() {
    show();
}

void ncQtGLWidget::setProgress(std::string text,int value) {
    qt_pbar->setProgress(text,value);
}

void ncQtGLWidget::mousePressEvent(QMouseEvent *event) {
    mouseEvent(event);
}

void ncQtGLWidget::mouseReleaseEvent(QMouseEvent *event) {
    mouseEvent(event);
}

void ncQtGLWidget::mouseMoveEvent(QMouseEvent *event) {
    mouseEvent(event);
}

void ncQtGLWidget::keyPressEvent(QKeyEvent * event) {

	//fprintf(stderr,"Going fullscreen!!!\n");

    if (event->type()  ==  QEvent::KeyPress)
        switch (event->key())
        {
        case Qt::Key_F11 :
			if (dummy_widget->isFullScreen())
			{
				//dummy_widget->hide();
				dummy_widget->showNormal();
				central_layout->addWidget(dummy_widget,0,0);
				//dummy_widget->show();
				this->setFocus(Qt::OtherFocusReason);
			}
			else
			{
				//dummy_widget->hide();
				central_layout->removeWidget(dummy_widget);
				dummy_widget->setParent(0);
				dummy_widget->showFullScreen();
				//dummy_widget->show();
			}
			break;
		}

    keyEvent(event);
}

void ncQtGLWidget::keyReleaseEvent(QKeyEvent * event) {
    keyEvent(event);
}

void ncQtGLWidget::closeEvent(QCloseEvent* /*ce*/) {
	show_window->setChecked(false);
}
