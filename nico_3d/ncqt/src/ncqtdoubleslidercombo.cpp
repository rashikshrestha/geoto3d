#include "ncqtdoubleslidercombo.h"
#include <cmath>

ncQtDoubleSliderCombo::ncQtDoubleSliderCombo(QString text, double minval_, double maxval_, double step, QWidget *parent) : QObject(parent)  {

	minval      = minval_;
	maxval      = maxval_;
	stepsize    = step;
	nrsteps     = 1+int((maxval-minval)/stepsize);

	label	= new QLabel(text,parent);

	spinbox = new ncQtDoubleSpinBox(parent);
	spinbox->setRange(minval,maxval);
	spinbox->setValue(minval);
	spinbox->setSingleStep(stepsize);

	slider  = new QSlider(parent);
	slider->setOrientation(Qt::Horizontal);
	slider->setRange(0,nrsteps);
	slider->setValue(0);

	connect(slider			, SIGNAL(valueChanged(int   ))	, this , SLOT(setValue(int   )));
	connect(spinbox			, SIGNAL(valueChanged(double))	, this , SLOT(setValue(double)));
	connect(&value_changed	, SIGNAL(output())				, this , SLOT(sendValueChanged()));
};

ncQtDoubleSliderCombo::~ncQtDoubleSliderCombo() {
};

void ncQtDoubleSliderCombo::setValue(double value) {

	//fprintf(stderr,"receiving ... setValue(double %30.26f)\n",value);
	int index = floor(nrsteps*(value-minval)/(maxval-minval) + 0.5);

	value_changed++;
	slider->setValue(index);
	spinbox->setValue(value);
	value_changed--;
}

double ncQtDoubleSliderCombo::value() {
    return spinbox->value();
}

void ncQtDoubleSliderCombo::setValue(int value) {
	//fprintf(stderr,"receiving ... setValue(int %d)\n",value);
	double newvalue = minval+value*(maxval-minval)/nrsteps;
	setValue(newvalue);
}

void ncQtDoubleSliderCombo::sendValueChanged() {
	//fprintf(stderr,"sendValueChanged(%30.26f)\n",value());
	valueChanged(value());
}

double ncQtDoubleSliderCombo::minimum() {
    return minval;
}

double ncQtDoubleSliderCombo::maximum() {
    return maxval;
}

