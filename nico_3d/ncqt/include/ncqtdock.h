#ifndef __NC_QT_DOCK_H__
#define __NC_QT_DOCK_H__

#include "ncqt.h"
#include "ncqtmainwindow.h"

class ncQtDock : public QDockWidget {

    Q_OBJECT

public:
    ncQtDock(const QString & title, ncQtMainWindow* mainw , Qt::DockWidgetArea init_area , Qt::DockWidgetAreas allowed_areas);
    ~ncQtDock();

signals:
	void log(QString text);

public:
	QGridLayout*    grid;
	
protected:
	QWidget*		dockwidget;
	QGridLayout*    dock_grid;
	QGroupBox*      groupwidget;


};

#endif
