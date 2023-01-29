#include "ncqtlogger.h"


ncQtLogger::ncQtLogger(QString title, ncQtMainWindow* parent , Qt::DockWidgetArea init_area , Qt::DockWidgetAreas allowed_areas) : ncQtDock(title,parent,init_area,allowed_areas) {
	textbrowser = new QTextBrowser;
	grid->addWidget(textbrowser,0,0);
}

ncQtLogger::~ncQtLogger() {

}

void ncQtLogger::log(QString text) {
    textbrowser->append(text);
	textbrowser->update();
}
