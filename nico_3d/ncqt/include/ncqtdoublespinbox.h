#ifndef __NC_QT_DOUBLESPINBOX_H__
#define __NC_QT_DOUBLESPINBOX_H__

#include "ncqt.h"

class ncQtDoubleSpinBox : public QDoubleSpinBox {

    Q_OBJECT

public:
    ncQtDoubleSpinBox(QWidget * parent = 0);
    ~ncQtDoubleSpinBox();

    QString textFromValue( double value ) const;
    double valueFromText ( const QString & text ) const;

};

#endif //! __NC_QT_SPINBOX_H__
