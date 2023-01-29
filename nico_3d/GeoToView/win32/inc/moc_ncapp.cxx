/****************************************************************************
** Meta object code from reading C++ file 'ncapp.h'
**
** Created: Wed Dec 11 16:07:00 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../inc/ncapp.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ncapp.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ncApp[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,    7,    6,    6, 0x0a,
      29,    6,    6,    6, 0x0a,
      41,    6,    6,    6, 0x0a,
      55,    6,    6,    6, 0x0a,
      71,    6,    6,    6, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ncApp[] = {
    "ncApp\0\0mat\0setPose(ncc::RTd)\0updateFBO()\0"
    "startRecord()\0sendDataPoint()\0"
    "sendDataPointCamera()\0"
};

void ncApp::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ncApp *_t = static_cast<ncApp *>(_o);
        switch (_id) {
        case 0: _t->setPose((*reinterpret_cast< ncc::RTd(*)>(_a[1]))); break;
        case 1: _t->updateFBO(); break;
        case 2: _t->startRecord(); break;
        case 3: _t->sendDataPoint(); break;
        case 4: _t->sendDataPointCamera(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ncApp::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ncApp::staticMetaObject = {
    { &ncQtGLWidget::staticMetaObject, qt_meta_stringdata_ncApp,
      qt_meta_data_ncApp, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ncApp::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ncApp::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ncApp::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ncApp))
        return static_cast<void*>(const_cast< ncApp*>(this));
    return ncQtGLWidget::qt_metacast(_clname);
}

int ncApp::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ncQtGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
