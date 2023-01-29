#ifndef __NC_QT_INTSPINBOX_H__
#define __NC_QT_INTSPINBOX_H__

#include "ncqt.h"

class ncQtIntSpinBox : public QAbstractSpinBox {

    Q_OBJECT

public slots:
    void finishEdit();

signals:
    void valueChanged(int);

public:
    ncQtIntSpinBox(QWidget * parent = 0);
    ~ncQtIntSpinBox();

    void		setRange(int minvalue, int maxvalue);
    void		setSingleStep(int step);
    bool		setValue(int val);
    void		stepBy(int steps);
    StepEnabled stepEnabled() const;

    int  Value();

private:

    int  clampValue(int val);

    int  minval;
    int  maxval;
    int  value;
    int  stepsize;
    
    QValidator *validator;
};

#endif //! __NC_QT_SPINBOX_H__
