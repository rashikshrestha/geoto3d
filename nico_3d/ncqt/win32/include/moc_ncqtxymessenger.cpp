/****************************************************************************
** Meta object code from reading C++ file 'ncqtxymessenger.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.0.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/ncqtxymessenger.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ncqtxymessenger.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.0.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ncQtXYMessenger_t {
    QByteArrayData data[11];
    char stringdata[113];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_ncQtXYMessenger_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_ncQtXYMessenger_t qt_meta_stringdata_ncQtXYMessenger = {
    {
QT_MOC_LITERAL(0, 0, 15),
QT_MOC_LITERAL(1, 16, 11),
QT_MOC_LITERAL(2, 28, 0),
QT_MOC_LITERAL(3, 29, 5),
QT_MOC_LITERAL(4, 35, 7),
QT_MOC_LITERAL(5, 43, 13),
QT_MOC_LITERAL(6, 57, 7),
QT_MOC_LITERAL(7, 65, 10),
QT_MOC_LITERAL(8, 76, 12),
QT_MOC_LITERAL(9, 89, 15),
QT_MOC_LITERAL(10, 105, 6)
    },
    "ncQtXYMessenger\0sendMessage\0\0title\0"
    "message\0toggleConnect\0connect\0disconnect\0"
    "setConnected\0setDisconnected\0readme\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ncQtXYMessenger[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   49,    2, 0x0a,
       5,    0,   54,    2, 0x08,
       6,    0,   55,    2, 0x08,
       7,    0,   56,    2, 0x08,
       8,    0,   57,    2, 0x08,
       9,    0,   58,    2, 0x08,
      10,    0,   59,    2, 0x08,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ncQtXYMessenger::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ncQtXYMessenger *_t = static_cast<ncQtXYMessenger *>(_o);
        switch (_id) {
        case 0: _t->sendMessage((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->toggleConnect(); break;
        case 2: _t->connect(); break;
        case 3: _t->disconnect(); break;
        case 4: _t->setConnected(); break;
        case 5: _t->setDisconnected(); break;
        case 6: _t->readme(); break;
        default: ;
        }
    }
}

const QMetaObject ncQtXYMessenger::staticMetaObject = {
    { &ncQtDock::staticMetaObject, qt_meta_stringdata_ncQtXYMessenger.data,
      qt_meta_data_ncQtXYMessenger,  qt_static_metacall, 0, 0}
};


const QMetaObject *ncQtXYMessenger::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ncQtXYMessenger::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ncQtXYMessenger.stringdata))
        return static_cast<void*>(const_cast< ncQtXYMessenger*>(this));
    return ncQtDock::qt_metacast(_clname);
}

int ncQtXYMessenger::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ncQtDock::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
