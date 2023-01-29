/****************************************************************************
** Meta object code from reading C++ file 'ncqtmultibar.h'
**
** Created: Thu Feb 20 14:38:54 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/ncqtmultibar.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ncqtmultibar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ncQtMultiBar[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      25,   14,   13,   13, 0x0a,
      68,   61,   13,   13, 0x2a,
     110,   99,   13,   13, 0x0a,
     151,  131,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ncQtMultiBar[] = {
    "ncQtMultiBar\0\0bspecs,pad\0"
    "setSpecs(std::vector<barSpec>,bool)\0"
    "bspecs\0setSpecs(std::vector<barSpec>)\0"
    "index,val_\0setValue(int,double)\0"
    "min_range,max_range\0setRange(double,double)\0"
};

void ncQtMultiBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ncQtMultiBar *_t = static_cast<ncQtMultiBar *>(_o);
        switch (_id) {
        case 0: _t->setSpecs((*reinterpret_cast< std::vector<barSpec>(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 1: _t->setSpecs((*reinterpret_cast< std::vector<barSpec>(*)>(_a[1]))); break;
        case 2: _t->setValue((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 3: _t->setRange((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ncQtMultiBar::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ncQtMultiBar::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ncQtMultiBar,
      qt_meta_data_ncQtMultiBar, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ncQtMultiBar::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ncQtMultiBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ncQtMultiBar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ncQtMultiBar))
        return static_cast<void*>(const_cast< ncQtMultiBar*>(this));
    return QWidget::qt_metacast(_clname);
}

int ncQtMultiBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
