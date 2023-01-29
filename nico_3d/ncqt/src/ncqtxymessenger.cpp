#include "ncqtxymessenger.h"
#include <QInputDialog>

static const int XYMsgLogin			= -33792000;
static const int XYMsgNewUser		= -33793000;
static const int XYMsgUserList		= -33794000;
static const int XYMsgRemoveUser	= -33795000;
static const int XYMsgText			= -33796000;
static const int XYMsgTextBroadcast = -33797000;
static const int XYMsgParent		= -33806000;

ncQtXYMessenger::ncQtXYMessenger(QString title, ncQtMainWindow* parent , Qt::DockWidgetArea init_area , Qt::DockWidgetAreas allowed_areas ) : ncQtDock(title,parent,init_area,allowed_areas) {

	nrmessages		= 0;
	server_username_= "";

	xyhost		= new QLineEdit(this);
	xyhost->setText("127.0.0.1");
	xyport		= new QSpinBox(this);
	xyport->setRange(0,10000);
	xyport->setValue(5001);
	xyconnect	= new QPushButton("Connect",this);

	socket		= new QTcpSocket(this);

	ncQtDock::grid->addWidget(new QLabel("Host",this),0,0);
	ncQtDock::grid->addWidget(xyhost,0,1);
	ncQtDock::grid->addWidget(new QLabel("Port",this),0,2);
	ncQtDock::grid->addWidget(xyport,0,3);
	ncQtDock::grid->addWidget(xyconnect,0,4);

	button_widget = new QWidget(this);
	grid = new QGridLayout;
	button_widget->setLayout(grid);
	button_widget->setEnabled(false);
	ncQtDock::grid->addWidget(button_widget,1,0,1,5);

	QObject::connect(xyconnect	, SIGNAL(clicked()		) , this , SLOT(toggleConnect()		));
	QObject::connect(socket		, SIGNAL(connected()	) , this , SLOT(setConnected()		));
	QObject::connect(socket		, SIGNAL(disconnected()	) , this , SLOT(setDisconnected()	));
	QObject::connect(socket		, SIGNAL(readyRead()	) , this , SLOT(readme()			));
}

ncQtXYMessenger::~ncQtXYMessenger() {
}

void ncQtXYMessenger::toggleConnect() {
	if (xyconnect->text() == "Connect")
		connect();
	else
		disconnect();
}

void ncQtXYMessenger::connect()
{
	socket->connectToHost(xyhost->text(),xyport->value());
}

void ncQtXYMessenger::disconnect()
{
	socket->disconnectFromHost();
}

void ncQtXYMessenger::setConnected() {
	login	= QString::number(rand());
	pwd		= QString::number(rand());
	xyconnect->setText("Disconnect");
	xyhost->setDisabled(true);
	xyport->setDisabled(true);
	button_widget->setEnabled(true);
	writeInt(XYMsgLogin);
	writeData(QString("<XYConnect><XYUser>")+login+"</XYUser><XYPassword>"+pwd+"</XYPassword></XYConnect>");
}

void ncQtXYMessenger::setDisconnected() {
	xyconnect->setText("Connect");
	xyhost->setDisabled(false);
	xyport->setDisabled(false);
	button_widget->setEnabled(false);

}

void ncQtXYMessenger::readme() {

	int type;
	while ((type=readInt())!=0)
	{
		char data[10000];
		if (type == XYMsgNewUser)
		{
			readData(data, 10000);
		}
		if (type == XYMsgParent)
		{
			if (readData(data, 10000))
			{
				server_username_ = QString(data);
			}
		}
		if (type == XYMsgUserList)
		{
			int count=readInt();
			for (int i=0; i<count; ++i)
			{
				readData(data, 10000);
				if ((QString(data).left(QString(data).indexOf("@"))) == login)
				{
					QMessageBox::information(0, "Connected", "Connected to " + xyhost->text() + " with login " + login);
					sendMessage("version","##0#");
				}
			}
		}
	}
}

int ncQtXYMessenger::readInt()
{
	char data[5];
	if (!socket->read(data, 4))
		return 0;
	return (*(int*)data);
}

int ncQtXYMessenger::readData(char* data, int max)
{
	int length = readInt();
	if (length>0)
	{
		int res = socket->read(data, (std::min)(max-1, length));
		if (!res)
			return 0;
		data[res]='\0';
		return res;
	}
	return length;
}

void ncQtXYMessenger::writeInt(int a)
{
	char data[4];
	data[0] = 0x000000ff&a;
	data[1] = 0x000000ff&(a>>8);
	data[2] = 0x000000ff&(a>>16);
	data[3] = 0x000000ff&(a>>24);
	socket->write(data, 4);
}

void ncQtXYMessenger::writeData(QString s)
{
	int l = s.length();
	writeInt(l);
	char* d = new char[l];
	for (int i=0; i<l; i++)
		d[i] = s.data()[i].unicode();
	socket->write(d, l);
	delete [] d;
}

void ncQtXYMessenger::sendMessage(QString title, QString msg)
{
	/*
	fprintf(stderr,"XYM Sending...\n");
	fprintf(stderr,"title   = %s\n",title.toStdString().c_str());
	fprintf(stderr,"message = %s\n",msg.toStdString().c_str());
	*/

	sendMessageToDestination(server_username_, title, msg);
}

void ncQtXYMessenger::sendMessageToDestination(QString destination, QString title, QString msg)
{
	writeInt(XYMsgText);
	writeInt(nrmessages++);
	writeData(login);
	writeData(destination);
	writeData(title);
	writeData(msg);
	writeInt(0);
}