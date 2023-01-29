
#ifndef NCQTMULTIBAR_H
#define NCQTMULTIBAR_H

#include <QWidget>
#include <QPainter>


struct barSpec {

    barSpec(QString txt = "" , double val = 0.0 , QColor col = Qt::black, int prec = 0) : col(col) , txt(txt) , val(val) , prec(prec){};

    QColor  col;
    QString txt;
    double  val;
    int     prec;
};


class ncQtMultiBar : public QWidget
{
    Q_OBJECT


public:

    ncQtMultiBar(std::vector<barSpec> bspecs , double min_range , double max_range , bool pad = 0, QWidget *parent = 0);
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

//signals:
//    void valueChanged(double);


public slots:
    void setSpecs(std::vector<barSpec> bspecs , bool pad = 0);
    void setValue(int index , double val_);
    void setRange(double min_range , double max_range);

protected:

    //void mousePressEvent(QMouseEvent *);
    //void mouseMoveEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
    void paintBorder();
    void paintBar();
    void paintValue();

private:

    std::vector<barSpec> bspecs;

    QRect   mywindow;
    QRect   mybar;
    QRect   mytext;

    double  min_range;
    double  max_range;
    int     border;
    int     barw;
    int     barh;
    int     texth;
    int     pad;

};

#endif
