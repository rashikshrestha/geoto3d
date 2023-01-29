#ifndef __NC_QT_LOGGER_H__
#define __NC_QT_LOGGER_H__

#include "ncqt.h"
#include "ncqtdock.h"

class ncQtLogger : public ncQtDock  {

    Q_OBJECT

public:
    ncQtLogger(QString title, ncQtMainWindow* parent , Qt::DockWidgetArea init_area , Qt::DockWidgetAreas allowed_areas);
    ~ncQtLogger();

public slots:
    void    log(QString text);

private:
	QTextBrowser* textbrowser;
};

#endif
