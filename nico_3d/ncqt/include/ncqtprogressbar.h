#ifndef __NCQTPROGRESSBAR_H__
#define __NCQTPROGRESSBAR_H__

#include "ncqt.h"
#include <QProgressBar>

class ncQtProgressBar : public QFrame
{
	Q_OBJECT

public:
	ncQtProgressBar(QWidget* parent = 0);
	~ncQtProgressBar();

public slots:
	void setProgress(std::string text,int value);

private:
	QProgressBar*	pbar;
	QLabel*			label;
	QVBoxLayout*	vbox;

};

#endif // NCPROGRESSBAR_H