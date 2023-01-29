#ifndef __NCQT_DOUBLESLIDERCOMBO_H__
#define __NCQT_DOUBLESLIDERCOMBO_H__

#include "ncqt.h"
#include "ncqtdoublespinbox.h"
#include "ncqtsignalhandler.h"

class ncQtDoubleSliderCombo : public QObject {

    Q_OBJECT

public:
    ncQtDoubleSliderCombo(QString text, double minval_, double maxval_, double step, QWidget *parent = 0);
    ~ncQtDoubleSliderCombo();

    double value();

    double minimum();
    double maximum();


signals:
    void valueChanged(double);

public slots:
    void setValue(double value);

private slots:
    void setValue(int value);
	void sendValueChanged();

public:
    ncQtDoubleSpinBox*  spinbox;
    QSlider*			slider;
	QLabel*				label;

private:
    double				minval;
    double				maxval;
    double				stepsize;
    int					nrsteps;
	ncQtSignalHandler	value_changed;
};

#endif
