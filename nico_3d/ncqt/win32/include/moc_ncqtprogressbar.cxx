/****************************************************************************
** Meta object code from reading C++ file 'ncqtprogressbar.h'
**
** Created: Thu Feb 20 14:38:53 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/ncqtprogressbar.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ncqtprogressbar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ncQtProgressBar[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      28,   17,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ncQtProgressBar[] = {
    "ncQtProgressBar\0\0text,value\0"
    "setProgress(std::string,int)\0"
};

void ncQtProgressBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ncQtProgressBar *_t = static_cast<ncQtProgressBar *>(_o);
        switch (_id) {
        case 0: _t->setProgress((*reinterpret_cast< std::string(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ncQtProgressBar::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ncQtProgressBar::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_ncQtProgressBar,
      qt_meta_data_ncQtProgressBar, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ncQtProgressBar::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ncQtProgressBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ncQtProgressBar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ncQtProgressBar))
        return static_cast<void*>(const_cast< ncQtProgressBar*>(this));
    return QFrame::qt_metacast(_clname);
}

int ncQtProgressBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
