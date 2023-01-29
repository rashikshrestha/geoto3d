#include "ncqtintslidercombo.h"

ncQtIntSliderCombo::ncQtIntSliderCombo(QString text, int minval_, int maxval_, int step, QWidget *parent) : QObject(parent)  {

	minval      = minval_;
	maxval      = maxval_;
	stepsize    = step;
	nrsteps     = int((maxval-minval)/stepsize);

	label	= new QLabel(text,parent);

	spinbox = new ncQtIntSpinBox(parent);
	spinbox->setRange(minval,maxval);
	spinbox->setSingleStep(stepsize);

	slider  = new QSlider(parent);
	slider->setOrientation(Qt::Horizontal);
	slider->setRange(0,nrsteps);

	connect(slider ,SIGNAL(valueChanged(int   )),this,SLOT(setSpinBoxValue(int   )));
	connect(spinbox,SIGNAL(valueChanged(int)),this,SLOT(setSliderValue (int)));
};

ncQtIntSliderCombo::~ncQtIntSliderCombo() {
};

void ncQtIntSliderCombo::setValue(int value) {
	setSliderValue(value);
}

int ncQtIntSliderCombo::getValue() {
    return spinbox->Value();
}

void ncQtIntSliderCombo::setSliderValue(int value) {
	int newvalue = floor(nrsteps*(value-minval)/(maxval-minval) + 0.5);
	if (newvalue!=slider->value())
		slider->setValue(newvalue);
}

void ncQtIntSliderCombo::setSpinBoxValue(int value) {
	int newvalue = minval+value*(maxval-minval)/nrsteps;
	spinbox->setValue(newvalue);
	valueChanged(newvalue);
}
