#include "mainwindow.h"

#include <QDesktopWidget>

#include "ncmath.h"

MainWindow::MainWindow() : ncQtMainWindow(QString(APP_NAME),QString(APP_VERSION)) {

    //! app
	glapp	= new ncApp("GeoToView: Render Window",this);
	postInit();
}

