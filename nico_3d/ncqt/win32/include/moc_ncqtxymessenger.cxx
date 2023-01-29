/****************************************************************************
** Meta object code from reading C++ file 'ncqtxymessenger.h'
**
** Created: Thu Feb 20 14:38:52 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/ncqtxymessenger.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ncqtxymessenger.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ncQtXYMessenger[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      31,   17,   16,   16, 0x0a,
      60,   16,   16,   16, 0x08,
      76,   16,   16,   16, 0x08,
      86,   16,   16,   16, 0x08,
      99,   16,   16,   16, 0x08,
     114,   16,   16,   16, 0x08,
     132,   16,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ncQtXYMessenger[] = {
    "ncQtXYMessenger\0\0title,message\0"
    "sendMessage(QString,QString)\0"
    "toggleConnect()\0connect()\0disconnect()\0"
    "setConnected()\0setDisconnected()\0"
    "readme()\0"
};

void ncQtXYMessenger::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
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

const QMetaObjectExtraData ncQtXYMessenger::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ncQtXYMessenger::staticMetaObject = {
    { &ncQtDock::staticMetaObject, qt_meta_stringdata_ncQtXYMessenger,
      qt_meta_data_ncQtXYMessenger, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ncQtXYMessenger::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ncQtXYMessenger::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ncQtXYMessenger::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ncQtXYMessenger))
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
    }
    return _id;
}
QT_END_MOC_NAMESPACE
