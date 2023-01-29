/****************************************************************************
** Meta object code from reading C++ file 'ncqtcamera.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.0.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../include/ncqtcamera.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ncqtcamera.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.0.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ncSkyDome_t {
    QByteArrayData data[5];
    char stringdata[36];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_ncSkyDome_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_ncSkyDome_t qt_meta_stringdata_ncSkyDome = {
    {
QT_MOC_LITERAL(0, 0, 9),
QT_MOC_LITERAL(1, 10, 12),
QT_MOC_LITERAL(2, 23, 0),
QT_MOC_LITERAL(3, 24, 8),
QT_MOC_LITERAL(4, 33, 1)
    },
    "ncSkyDome\0indexChanged\0\0setIndex\0i\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ncSkyDome[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       3,    1,   25,    2, 0x0a,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    4,

       0        // eod
};

void ncSkyDome::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ncSkyDome *_t = static_cast<ncSkyDome *>(_o);
        switch (_id) {
        case 0: _t->indexChanged(); break;
        case 1: _t->setIndex((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ncSkyDome::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ncSkyDome::indexChanged)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject ncSkyDome::staticMetaObject = {
    { &QComboBox::staticMetaObject, qt_meta_stringdata_ncSkyDome.data,
      qt_meta_data_ncSkyDome,  qt_static_metacall, 0, 0}
};


const QMetaObject *ncSkyDome::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ncSkyDome::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ncSkyDome.stringdata))
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
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void ncSkyDome::indexChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
struct qt_meta_stringdata_ncQtCamera_t {
    QByteArrayData data[42];
    char stringdata[321];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_ncQtCamera_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_ncQtCamera_t qt_meta_stringdata_ncQtCamera = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 13),
QT_MOC_LITERAL(2, 25, 0),
QT_MOC_LITERAL(3, 26, 21),
QT_MOC_LITERAL(4, 48, 8),
QT_MOC_LITERAL(5, 57, 6),
QT_MOC_LITERAL(6, 64, 6),
QT_MOC_LITERAL(7, 71, 17),
QT_MOC_LITERAL(8, 89, 12),
QT_MOC_LITERAL(9, 102, 12),
QT_MOC_LITERAL(10, 115, 5),
QT_MOC_LITERAL(11, 121, 8),
QT_MOC_LITERAL(12, 130, 6),
QT_MOC_LITERAL(13, 137, 10),
QT_MOC_LITERAL(14, 148, 5),
QT_MOC_LITERAL(15, 154, 6),
QT_MOC_LITERAL(16, 161, 4),
QT_MOC_LITERAL(17, 166, 8),
QT_MOC_LITERAL(18, 175, 6),
QT_MOC_LITERAL(19, 182, 7),
QT_MOC_LITERAL(20, 190, 5),
QT_MOC_LITERAL(21, 196, 12),
QT_MOC_LITERAL(22, 209, 6),
QT_MOC_LITERAL(23, 216, 7),
QT_MOC_LITERAL(24, 224, 1),
QT_MOC_LITERAL(25, 226, 8),
QT_MOC_LITERAL(26, 235, 1),
QT_MOC_LITERAL(27, 237, 6),
QT_MOC_LITERAL(28, 244, 1),
QT_MOC_LITERAL(29, 246, 4),
QT_MOC_LITERAL(30, 251, 1),
QT_MOC_LITERAL(31, 253, 4),
QT_MOC_LITERAL(32, 258, 4),
QT_MOC_LITERAL(33, 263, 1),
QT_MOC_LITERAL(34, 265, 9),
QT_MOC_LITERAL(35, 275, 3),
QT_MOC_LITERAL(36, 279, 7),
QT_MOC_LITERAL(37, 287, 8),
QT_MOC_LITERAL(38, 296, 4),
QT_MOC_LITERAL(39, 301, 5),
QT_MOC_LITERAL(40, 307, 8),
QT_MOC_LITERAL(41, 316, 3)
    },
    "ncQtCamera\0valuesChanged\0\0"
    "boundingSphereChanged\0ncc::v3d\0center\0"
    "radius\0sendValuesChanged\0processEvent\0"
    "QMouseEvent*\0event\0setColor\0setKGL\0"
    "ncc::m4x4d\0KGL__\0setFOV\0fov_\0setZnear\0"
    "znear_\0setZfar\0zfar_\0setMouseSens\0"
    "msens_\0setRoll\0r\0setPitch\0p\0setYaw\0y\0"
    "setX\0x\0setY\0setZ\0z\0ncc::v4ub\0col\0"
    "setSize\0ncc::v2i\0size\0setMV\0ncc::RTd\0"
    "mat\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ncQtCamera[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  109,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       3,    2,  110,    2, 0x0a,
       7,    0,  115,    2, 0x0a,
       8,    1,  116,    2, 0x0a,
      11,    0,  119,    2, 0x0a,
      12,    1,  120,    2, 0x0a,
      15,    1,  123,    2, 0x0a,
      17,    1,  126,    2, 0x0a,
      19,    1,  129,    2, 0x0a,
      21,    1,  132,    2, 0x0a,
      23,    1,  135,    2, 0x0a,
      25,    1,  138,    2, 0x0a,
      27,    1,  141,    2, 0x0a,
      29,    1,  144,    2, 0x0a,
      31,    1,  147,    2, 0x0a,
      32,    1,  150,    2, 0x0a,
      11,    1,  153,    2, 0x0a,
      36,    1,  156,    2, 0x0a,
      39,    1,  159,    2, 0x0a,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 4, QMetaType::Double,    5,    6,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void, QMetaType::Double,   16,
    QMetaType::Void, QMetaType::Double,   18,
    QMetaType::Void, QMetaType::Double,   20,
    QMetaType::Void, QMetaType::Double,   22,
    QMetaType::Void, QMetaType::Double,   24,
    QMetaType::Void, QMetaType::Double,   26,
    QMetaType::Void, QMetaType::Double,   28,
    QMetaType::Void, QMetaType::Double,   30,
    QMetaType::Void, QMetaType::Double,   28,
    QMetaType::Void, QMetaType::Double,   33,
    QMetaType::Void, 0x80000000 | 34,   35,
    QMetaType::Void, 0x80000000 | 37,   38,
    QMetaType::Void, 0x80000000 | 40,   41,

       0        // eod
};

void ncQtCamera::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
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
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ncQtCamera::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ncQtCamera::valuesChanged)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject ncQtCamera::staticMetaObject = {
    { &ncQtDock::staticMetaObject, qt_meta_stringdata_ncQtCamera.data,
      qt_meta_data_ncQtCamera,  qt_static_metacall, 0, 0}
};


const QMetaObject *ncQtCamera::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ncQtCamera::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ncQtCamera.stringdata))
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
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 19)
            *reinterpret_cast<int*>(_a[0]) = -1;
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
