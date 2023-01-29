#include "ncqtprogressbar.h"

ncQtProgressBar::ncQtProgressBar(QWidget* parent) : QFrame(parent) {
	label	= new QLabel;
	pbar	= new QProgressBar;
	pbar->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
	pbar->setRange(0,100);

	vbox = new QVBoxLayout;
	vbox->addWidget(label);
	vbox->addWidget(pbar);

	setLayout(vbox);

	setFrameStyle(ncqt::frame_style);

	hide();
}

ncQtProgressBar::~ncQtProgressBar() {
}

void ncQtProgressBar::setProgress(std::string text,int value) {
	
	if (value ==100)
		hide();
	else
	{
		label->setText(QString::fromStdString(text));
		pbar->setValue(value);
		show();
	}
	qApp->processEvents();

}