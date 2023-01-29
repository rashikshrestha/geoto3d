#ifndef __NC_QT_SIGNALHANDLER_H__
#define __NC_QT_SIGNALHANDLER_H__

#include <QObject>

class ncQtSignalHandler : public QObject
{
	Q_OBJECT

public:
	ncQtSignalHandler();
	~ncQtSignalHandler();

	void				operator()();
	ncQtSignalHandler&	operator++();
	void				operator++(int);
	ncQtSignalHandler&	operator--();
	void				operator--(int);

signals:
	void output();

public slots:
	void input();

private:
	int block_count;

};

#endif //__NC_QT_SIGNALHANDLER_H__