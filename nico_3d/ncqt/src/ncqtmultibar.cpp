/***************************************************************************
 *   Copyright (C) 2008 - Giuseppe Cigala                                  *
 *   g_cigala@virgilio.it                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include "ncqtmultibar.h"

ncQtMultiBar::ncQtMultiBar(std::vector<barSpec> bspecs_ , double min_range_ , double max_range_ , bool pad_, QWidget *parent) : QWidget(parent)
{
    border  = 2;
    barw    = 32;
    barh    = 16;
    texth   = 9;

    setMinimumSize(barw,border+barh+2*border+texth+border);
    setMinimumHeight(border+barh+2*border+texth+border);
    setRange(min_range_,max_range_);
	setSpecs(bspecs_,pad_);
}

void ncQtMultiBar::setValue(int index , double val)
{
    if ((index>=0) && (index<(int)bspecs.size()))
        bspecs[index].val = val;
	update();
}

void ncQtMultiBar::setSpecs(std::vector<barSpec> bspecs_ , bool pad_ ) {

	bspecs      = bspecs_;
    if (pad_)
    {
        pad = 1;
        bspecs.push_back(barSpec("",1.0,Qt::gray));
    }
    else
        pad = 0;
}

void ncQtMultiBar::setRange(double min_range_ , double max_range_)
{
    min_range   = min_range_;
    max_range   = max_range_;
}

void ncQtMultiBar::paintEvent(QPaintEvent *)
{
    double total_value = 0.0;
    for (unsigned int i=0;i<bspecs.size()-pad;++i)
        total_value+=bspecs[i].val;
    if (pad)
        bspecs.back().val = 1.0-total_value;
    else
        for (unsigned int i=0;i<bspecs.size();++i)
            bspecs[i].val/=total_value;

    barw        = width()-2*border;
    mywindow    = QRect(     0,                    0,width()      ,height()   );
    mybar       = QRect(border,border               ,barw         ,barh       );
    mytext      = QRect(border,border+barh+2*border ,barw         ,texth      );

    paintBar();
    paintValue();
}

void ncQtMultiBar::paintBar()
{

    QPainter painter(this);
    painter.setWindow(mywindow);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.setBrush(Qt::black);

    double startx = mybar.x();
    for (unsigned int i=0;i<bspecs.size();++i)
        if (bspecs[i].val != 0.0)
        {
            QLinearGradient linGrad1(mybar.x(),mybar.y(),mybar.x(),mybar.y()+0.5*mybar.height());
            linGrad1.setColorAt(0,bspecs[i].col.darker(200));
            linGrad1.setColorAt(1, bspecs[i].col);
            linGrad1.setSpread(QGradient::PadSpread);
            painter.setBrush(linGrad1);
            QRectF myrect = mybar;
            double stopx =startx+bspecs[i].val*mybar.width();
            myrect.setX(startx);
            myrect.setWidth(stopx-startx);
            painter.drawRect(myrect);

            startx = stopx;
        }
}

void ncQtMultiBar::paintValue()
{
    QPainter painter(this);
    painter.setWindow(mywindow);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QBrush(Qt::black));
    //painter.drawRect(mytext);

    QFont valFont("SanSerif", texth, QFont::Bold);
    valFont.setPixelSize(texth);
    painter.setFont(valFont);

    for (unsigned int i=0;i<bspecs.size()-pad;++i)
    {
        QRectF myrect = mytext;
        double new_width = double(myrect.width())/(bspecs.size()-pad);
        myrect.setX(myrect.x()+i*new_width);
        myrect.setWidth(new_width);
        painter.setPen(bspecs[i].col.darker(200));

        unsigned int align_ = Qt::AlignCenter;
        /*
        if (i==0)
            align_ = Qt::AlignLeft;
        if (i==(bspecs.size()-1))
            align_ = Qt::AlignRight;
        */
        //if (bspecs[i].val!=0.0)
            painter.drawText(myrect, align_, bspecs[i].txt.arg(min_range+bspecs[i].val*(max_range-min_range),0,'f',bspecs[i].prec));
        //else
        //    painter.drawText(myrect, align_, bspecs[i].txt);
    }

}

#if 0
void ncQtMultiBar::mousePressEvent(QMouseEvent *e)
{
    double x = (double(e->x())/width() )*mywindow.width();
    double y = (double(e->y())/height())*mywindow.height();

    double x2 = (x-mybar.x())/mybar.width();
    double y2 = (y-mybar.y())/mybar.height();

    if ((x2>=0.0) && (x2<=1.0) && (y2>=0.0) && (y2<=1.0))
    {
        bar1.val  = x2;
        update();
    }
}

void ncQtMultiBar::mouseMoveEvent(QMouseEvent *e)
{
    mousePressEvent(e);
}

#endif

QSize ncQtMultiBar::minimumSizeHint() const
{
    return QSize(barw, border+barh+2*border+texth+border);
}

QSize ncQtMultiBar::sizeHint() const
{
    return mywindow.size();
}


