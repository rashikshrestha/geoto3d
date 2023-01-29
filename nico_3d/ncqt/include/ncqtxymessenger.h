#ifndef __NC_QT_XYMESSENGER_H__
#define __NC_QT_XYMESSENGER_H__

#include "ncqtdock.h"

class ncQtXYMessenger : public ncQtDock {

    Q_OBJECT

public:
    ncQtXYMessenger(QString title, ncQtMainWindow* parent , Qt::DockWidgetArea init_area , Qt::DockWidgetAreas allowed_areas );
    ~ncQtXYMessenger();

public slots:
	void sendMessage(QString title, QString message);

private slots:
	void toggleConnect();
	void connect();
	void disconnect();
	void setConnected();
	void setDisconnected();
	void readme();

private:
	int	 readInt();
	int	 readData(char* data, int max);
	void writeInt(int a);
	void writeData(QString s);
	void sendMessageToDestination(QString destination, QString title, QString message);

public:
	QGridLayout*					grid;

private:
	QLineEdit*						xyhost;
	QSpinBox*						xyport;
	QPushButton*					xyconnect;
	QTcpSocket*						socket;
	QString							login;
	QString							pwd;
	QString							server_username_;
	int								nrmessages;
	QWidget*						button_widget;
};

#endif //__NC_QT_XYMESSENGER_H__
