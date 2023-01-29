#include "ncqtabout.h"


ncQtAbout::ncQtAbout(QWidget* parent) : QMessageBox(parent) {

	hide();
	menu_show = new QAction ("&About", this);
    menu_show->setIconVisibleInMenu(true);
   	connect(menu_show	, SIGNAL(triggered())	, this			, SLOT(showAbout()));
	setIcon(QMessageBox::Information);
	setWindowTitle("About");
}

ncQtAbout::~ncQtAbout() {
}

void ncQtAbout::setText(QString app_name, QString app_version) {

    info = "<FONT COLOR=blue SIZE=40><b>" + app_name + "</b></FONT> <br> <br> <b>Version " + app_version + "</b> <br> <b>Build</b> " + __DATE__ + " " + __TIME__ + "<br> <b>Author</b> Nico Cornelis <br> <br> \xA9 GeoAutomation 2014";
	QMessageBox::setText(info);
}

void ncQtAbout::showAbout() {

	show();
}



