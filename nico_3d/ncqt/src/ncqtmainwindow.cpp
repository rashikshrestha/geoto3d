#include "ncqtmainwindow.h"

#include <QDesktopWidget>

ncQtMainWindow::ncQtMainWindow(QString app_name, QString app_version ,  Qt::WindowFlags flags): QMainWindow(0 , flags) {

	file_menu       = menuBar()->addMenu("&File");
	windows_menu    = menuBar()->addMenu("&Windows");
	help_menu       = menuBar()->addMenu("&Help");

	setContentsMargins(5,5,5,5);

    about = new ncQtAbout(this);
	about->setText(app_name, app_version);

	setCorner(Qt::TopLeftCorner ,Qt::LeftDockWidgetArea );
	setCorner(Qt::TopRightCorner,Qt::RightDockWidgetArea);

	central_widget = new QWidget(this);
	central_widget->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
	setCentralWidget(central_widget);

	setAnimated(false);

    connect(qApp			, SIGNAL(aboutToQuit())				, this            , SLOT(onQuit())				);
}

ncQtMainWindow::~ncQtMainWindow() {
}

void ncQtMainWindow::onQuit() {
}

void ncQtMainWindow::postInit() {

    file_menu_quit = new QAction("&Quit", this);
    file_menu_quit->setIconVisibleInMenu(true);
    file_menu_quit->setShortcut(tr("CTRL+Q"));
    file_menu->addAction(file_menu_quit);

    connect(file_menu_quit  , SIGNAL(triggered())   , qApp, SLOT(quit())    );

	help_menu->addAction(about->menu_show);
}

void ncQtMainWindow::closeEvent(QCloseEvent* /*ce*/) {
	qApp->quit();
}

void ncQtMainWindow::keyPressEvent(QKeyEvent* /*ke*/) {

	//fprintf(stderr,"intercepted key event...\n");
}
