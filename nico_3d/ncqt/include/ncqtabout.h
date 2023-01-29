#ifndef __NC_QT_ABOUT_H__
#define __NC_QT_ABOUT_H__

#include "ncqt.h"

class ncQtAbout : public QMessageBox {

    Q_OBJECT

public:
    ncQtAbout(QWidget* parent = 0);
    ~ncQtAbout();

    void setText(QString app_name, QString app_version);

public slots:
	void showAbout();

public:
	QAction *   menu_show;
	QString		info;

};

#endif
