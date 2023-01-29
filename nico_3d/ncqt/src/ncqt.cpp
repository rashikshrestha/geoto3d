#include "ncqt.h"
//#include "arthurwidgets.h"

int		ncqt::frame_style	= QFrame::Panel | QFrame::Sunken;
QString ncqt::browsepath	= QDir::homePath();
//QStyle* ncqt::my_style      = new ArthurStyle;

bool ncqt::readImage(QString fname, ncImageBuffer<ncc::v3ub> & imbuf) {
	QImage im(fname);
	imbuf.resize(im.width(),im.height());
	for (unsigned int y=0;y<im.height();++y)
		for (unsigned int x=0;x<im.width();++x)
		{
			QRgb pix = im.pixel(x,y);
			imbuf[3*(y*im.width()+x)+0] = qRed(pix);
			imbuf[3*(y*im.width()+x)+1] = qGreen(pix);
			imbuf[3*(y*im.width()+x)+2] = qBlue(pix);
		}
	return true;
}

bool ncqt::readImage(QString fname, ncImageBuffer<ncc::v4ub> & imbuf) {

	QImage im(fname);
	imbuf.resize(im.width(),im.height());
	for (unsigned int y=0;y<im.height();++y)
		for (unsigned int x=0;x<im.width();++x)
		{
			QRgb pix = im.pixel(x,y);
			imbuf[y*im.width()+x].x = qRed(pix);
			imbuf[y*im.width()+x].y = qGreen(pix);
			imbuf[y*im.width()+x].z = qBlue(pix);
			imbuf[y*im.width()+x].w = qAlpha(pix);
		}
	return true;
}
