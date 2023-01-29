#include "ncqtdoublespinbox.h"
#include <limits>

ncQtDoubleSpinBox::ncQtDoubleSpinBox(QWidget * parent) : QDoubleSpinBox(parent) {

    setMinimumSize(112,24);
    setMaximumSize(112,24);
    lineEdit()->setAlignment(Qt::AlignRight);
    setAccelerated(true);

	double minval      = - (std::numeric_limits<double>::max)();
    double maxval      = + (std::numeric_limits<double>::max)();
	setRange(minval , maxval);
	setValue(0.0);
	setDecimals(323);

}

ncQtDoubleSpinBox::~ncQtDoubleSpinBox() {
}

QString ncQtDoubleSpinBox::textFromValue( double value ) const {
    return QString::number(value, 'f', 5);
}

double ncQtDoubleSpinBox::valueFromText ( const QString & text ) const {
    return text.toDouble();
}
