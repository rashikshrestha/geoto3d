#include "mainwindow.h"

MainWindow::MainWindow() : ncQtMainWindow(QString(APP_NAME),QString(APP_VERSION))  {

	//! app
	glapp	= new ncApp("GeoToDepthMap: Render Window",this);

	postInit();

}

MainWindow::~MainWindow() {

}
