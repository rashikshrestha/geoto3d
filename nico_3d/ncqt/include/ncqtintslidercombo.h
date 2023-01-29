#ifndef __NCQT_INTSLIDERCOMBO_H__
#define __NCQT_INTSLIDERCOMBO_H__

#include "ncqt.h"
#include "ncqtintspinbox.h"

class ncQtIntSliderCombo : public QObject {

    Q_OBJECT

public:
    ncQtIntSliderCombo(QString text, int minval_, int maxval_, int step, QWidget *parent = 0);
    ~ncQtIntSliderCombo();

    int getValue();

signals:
    void valueChanged(int);

public slots:
    void setValue(int value);

private slots:
    void setSliderValue(int value);
    void setSpinBoxValue(int value);


public:
    ncQtIntSpinBox* spinbox;
    QSlider*        slider;
	QLabel*			label;

private:
    int          minval;
    int          maxval;
    int          stepsize;
    int          nrsteps;
};

#endif
