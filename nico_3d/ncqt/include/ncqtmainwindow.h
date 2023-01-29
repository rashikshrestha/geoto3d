#ifndef __NCQT_MAINWINDOW_H__
#define __NCQT_MAINWINDOW_H__

//#include <QtPlugin>
//Q_IMPORT_PLUGIN (QWindowsIntegrationPlugin);

#include "ncqtabout.h"

class ncQtMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    ncQtMainWindow(QString app_name, QString app_version , Qt::WindowFlags flags = 0);
    ~ncQtMainWindow();

protected:
    void postInit();

	virtual void closeEvent(QCloseEvent* ce);
	virtual void keyPressEvent(QKeyEvent* ke);
	virtual void leaveEvent ( QEvent * event ) {};

public slots:
	virtual void onQuit();


public:
    QMenu*		file_menu;
    QAction*	file_menu_quit;
    QMenu*		windows_menu;
    QMenu*		help_menu;
    ncQtAbout*	about;

	QWidget*	central_widget;

};

#endif
