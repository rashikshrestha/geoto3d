#include "ncqtintspinbox.h"
#include <limits>

ncQtIntSpinBox::ncQtIntSpinBox(QWidget * parent) : QAbstractSpinBox(parent) {

    setMinimumSize(96,16);
    lineEdit()->setAlignment(Qt::AlignRight);
    setAccelerated(true);

    QRegExp rx("[0123456789]*");
    validator = new QRegExpValidator(rx, this);
    lineEdit()->setValidator(validator);

    minval      = - (std::numeric_limits<int>::max)();
    maxval      = + (std::numeric_limits<int>::max)();
    stepsize    = 0.1;

    value = 0.0;
    lineEdit()->setText("0");

    connect(this    , SIGNAL(editingFinished())   , this , SLOT( finishEdit()));
}

ncQtIntSpinBox::~ncQtIntSpinBox() {
}

void ncQtIntSpinBox::setRange(int minvalue, int maxvalue) {
    minval = (std::min)(minvalue,maxvalue);
    maxval = (std::max)(minvalue,maxvalue);
    setValue(value);
}

int ncQtIntSpinBox::clampValue(int val) {

    return (std::min)((std::max)(val,minval),maxval);
}

void ncQtIntSpinBox::setSingleStep(int step) {
    stepsize = step;
}

bool ncQtIntSpinBox::setValue(int val) {
    int oldval = value;
    value = clampValue(val);
    if (oldval!=value)
    {
        lineEdit()->setText(QString::number(val));
		lineEdit()->repaint();
        return true;
    }
    else
        return false;
}

void ncQtIntSpinBox::finishEdit() {
    int tmp = lineEdit()->text().toInt();
    if (setValue(tmp))
        valueChanged(value);
}

void ncQtIntSpinBox::stepBy(int steps) {
    if (setValue(value+steps*stepsize))
        valueChanged(value);
}

QAbstractSpinBox::StepEnabled ncQtIntSpinBox::stepEnabled() const {

    return StepUpEnabled | StepDownEnabled;
}

int ncQtIntSpinBox::Value() {
    return value;
}
