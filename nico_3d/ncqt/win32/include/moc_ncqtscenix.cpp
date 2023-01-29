/****************************************************************************
** Meta object code from reading C++ file 'ncqtscenix.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.0.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/ncqtscenix.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ncqtscenix.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.0.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ncQtScenix_t {
    QByteArrayData data[28];
    char stringdata[293];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_ncQtScenix_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_ncQtScenix_t qt_meta_stringdata_ncQtScenix = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 13),
QT_MOC_LITERAL(2, 25, 0),
QT_MOC_LITERAL(3, 26, 21),
QT_MOC_LITERAL(4, 48, 8),
QT_MOC_LITERAL(5, 57, 6),
QT_MOC_LITERAL(6, 64, 6),
QT_MOC_LITERAL(7, 71, 13),
QT_MOC_LITERAL(8, 85, 16),
QT_MOC_LITERAL(9, 102, 4),
QT_MOC_LITERAL(10, 107, 6),
QT_MOC_LITERAL(11, 114, 4),
QT_MOC_LITERAL(12, 119, 10),
QT_MOC_LITERAL(13, 130, 3),
QT_MOC_LITERAL(14, 134, 8),
QT_MOC_LITERAL(15, 143, 2),
QT_MOC_LITERAL(16, 146, 8),
QT_MOC_LITERAL(17, 155, 2),
QT_MOC_LITERAL(18, 158, 9),
QT_MOC_LITERAL(19, 168, 12),
QT_MOC_LITERAL(20, 181, 10),
QT_MOC_LITERAL(21, 192, 10),
QT_MOC_LITERAL(22, 203, 12),
QT_MOC_LITERAL(23, 216, 2),
QT_MOC_LITERAL(24, 219, 11),
QT_MOC_LITERAL(25, 231, 17),
QT_MOC_LITERAL(26, 249, 20),
QT_MOC_LITERAL(27, 270, 21)
    },
    "ncQtScenix\0valuesChanged\0\0"
    "boundingSphereChanged\0ncc::v3d\0center\0"
    "radius\0onItemChanged\0QTreeWidgetItem*\0"
    "item\0column\0draw\0ncc::m4x4d\0KGL\0"
    "ncc::RTd\0MV\0ncc::v4i\0VP\0addModels\0"
    "removeModels\0showModels\0hideModels\0"
    "setPointSize\0ps\0getNrModels\0"
    "sendValuesChanged\0resetCameraToVisible\0"
    "resetCameraToSelected\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ncQtScenix[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x05,
       3,    2,   80,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       7,    2,   85,    2, 0x0a,
      11,    3,   90,    2, 0x0a,
      18,    0,   97,    2, 0x0a,
      19,    0,   98,    2, 0x0a,
      20,    0,   99,    2, 0x0a,
      21,    0,  100,    2, 0x0a,
      22,    1,  101,    2, 0x0a,
      24,    0,  104,    2, 0x0a,
      25,    0,  105,    2, 0x0a,
      26,    0,  106,    2, 0x0a,
      27,    0,  107,    2, 0x0a,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4, QMetaType::Double,    5,    6,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 8, QMetaType::Int,    9,   10,
    QMetaType::Void, 0x80000000 | 12, 0x80000000 | 14, 0x80000000 | 16,   13,   15,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,   23,
    QMetaType::Int,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ncQtScenix::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ncQtScenix *_t = static_cast<ncQtScenix *>(_o);
        switch (_id) {
        case 0: _t->valuesChanged(); break;
        case 1: _t->boundingSphereChanged((*reinterpret_cast< ncc::v3d(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 2: _t->onItemChanged((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->draw((*reinterpret_cast< ncc::m4x4d(*)>(_a[1])),(*reinterpret_cast< ncc::RTd(*)>(_a[2])),(*reinterpret_cast< ncc::v4i(*)>(_a[3]))); break;
        case 4: _t->addModels(); break;
        case 5: _t->removeModels(); break;
        case 6: _t->showModels(); break;
        case 7: _t->hideModels(); break;
        case 8: _t->setPointSize((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 9: { int _r = _t->getNrModels();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 10: _t->sendValuesChanged(); break;
        case 11: _t->resetCameraToVisible(); break;
        case 12: _t->resetCameraToSelected(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ncQtScenix::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ncQtScenix::valuesChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (ncQtScenix::*_t)(ncc::v3d , double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ncQtScenix::boundingSphereChanged)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject ncQtScenix::staticMetaObject = {
    { &ncQtDock::staticMetaObject, qt_meta_stringdata_ncQtScenix.data,
      qt_meta_data_ncQtScenix,  qt_static_metacall, 0, 0}
};


const QMetaObject *ncQtScenix::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ncQtScenix::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ncQtScenix.stringdata))
        return static_cast<void*>(const_cast< ncQtScenix*>(this));
    return ncQtDock::qt_metacast(_clname);
}

int ncQtScenix::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ncQtDock::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void ncQtScenix::valuesChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void ncQtScenix::boundingSphereChanged(ncc::v3d _t1, double _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
