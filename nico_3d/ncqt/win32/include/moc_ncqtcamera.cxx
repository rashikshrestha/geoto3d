/****************************************************************************
** Meta object code from reading C++ file 'ncqtcamera.h'
**
** Created: Thu Feb 20 14:38:57 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/ncqtcamera.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ncqtcamera.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ncSkyDome[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      28,   26,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ncSkyDome[] = {
    "ncSkyDome\0\0indexChanged()\0i\0setIndex(int)\0"
};

void ncSkyDome::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ncSkyDome *_t = static_cast<ncSkyDome *>(_o);
        switch (_id) {
        case 0: _t->indexChanged(); break;
        case 1: _t->setIndex((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ncSkyDome::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ncSkyDome::staticMetaObject = {
    { &QComboBox::staticMetaObject, qt_meta_stringdata_ncSkyDome,
      qt_meta_data_ncSkyDome, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ncSkyDome::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ncSkyDome::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ncSkyDome::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ncSkyDome))
        return static_cast<void*>(const_cast< ncSkyDome*>(this));
    return QComboBox::qt_metacast(_clname);
}

int ncSkyDome::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QComboBox::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void ncSkyDome::indexChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
static const uint qt_meta_data_ncQtCamera[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      42,   28,   11,   11, 0x0a,
      81,   11,   11,   11, 0x0a,
     107,  101,   11,   11, 0x0a,
     134,   11,   11,   11, 0x0a,
     151,  145,   11,   11, 0x0a,
     175,  170,   11,   11, 0x0a,
     197,  190,   11,   11, 0x0a,
     220,  214,   11,   11, 0x0a,
     243,  236,   11,   11, 0x0a,
     266,  264,   11,   11, 0x0a,
     284,  282,   11,   11, 0x0a,
     303,  301,   11,   11, 0x0a,
     320,  318,   11,   11, 0x0a,
     333,  301,   11,   11, 0x0a,
     348,  346,   11,   11, 0x0a,
     365,  361,   11,   11, 0x0a,
     390,  385,   11,   11, 0x0a,
     412,  408,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ncQtCamera[] = {
    "ncQtCamera\0\0valuesChanged()\0center,radius\0"
    "boundingSphereChanged(ncc::v3d,double)\0"
    "sendValuesChanged()\0event\0"
    "processEvent(QMouseEvent*)\0setColor()\0"
    "KGL__\0setKGL(ncc::m4x4d)\0fov_\0"
    "setFOV(double)\0znear_\0setZnear(double)\0"
    "zfar_\0setZfar(double)\0msens_\0"
    "setMouseSens(double)\0r\0setRoll(double)\0"
    "p\0setPitch(double)\0y\0setYaw(double)\0"
    "x\0setX(double)\0setY(double)\0z\0"
    "setZ(double)\0col\0setColor(ncc::v4ub)\0"
    "size\0setSize(ncc::v2i)\0mat\0setMV(ncc::RTd)\0"
};

void ncQtCamera::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ncQtCamera *_t = static_cast<ncQtCamera *>(_o);
        switch (_id) {
        case 0: _t->valuesChanged(); break;
        case 1: _t->boundingSphereChanged((*reinterpret_cast< ncc::v3d(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 2: _t->sendValuesChanged(); break;
        case 3: _t->processEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 4: _t->setColor(); break;
        case 5: _t->setKGL((*reinterpret_cast< ncc::m4x4d(*)>(_a[1]))); break;
        case 6: _t->setFOV((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 7: _t->setZnear((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 8: _t->setZfar((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 9: _t->setMouseSens((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 10: _t->setRoll((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 11: _t->setPitch((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 12: _t->setYaw((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 13: _t->setX((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 14: _t->setY((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 15: _t->setZ((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 16: _t->setColor((*reinterpret_cast< ncc::v4ub(*)>(_a[1]))); break;
        case 17: _t->setSize((*reinterpret_cast< ncc::v2i(*)>(_a[1]))); break;
        case 18: _t->setMV((*reinterpret_cast< ncc::RTd(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ncQtCamera::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ncQtCamera::staticMetaObject = {
    { &ncQtDock::staticMetaObject, qt_meta_stringdata_ncQtCamera,
      qt_meta_data_ncQtCamera, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ncQtCamera::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ncQtCamera::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ncQtCamera::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ncQtCamera))
        return static_cast<void*>(const_cast< ncQtCamera*>(this));
    return ncQtDock::qt_metacast(_clname);
}

int ncQtCamera::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ncQtDock::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    }
    return _id;
}

// SIGNAL 0
void ncQtCamera::valuesChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
