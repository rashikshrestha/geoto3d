/****************************************************************************
** Meta object code from reading C++ file 'ncqtconfig.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.0.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/ncqtconfig.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ncqtconfig.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.0.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_cam_cfg_t {
    QByteArrayData data[4];
    char stringdata[32];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_cam_cfg_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_cam_cfg_t qt_meta_stringdata_cam_cfg = {
    {
QT_MOC_LITERAL(0, 0, 7),
QT_MOC_LITERAL(1, 8, 11),
QT_MOC_LITERAL(2, 20, 0),
QT_MOC_LITERAL(3, 21, 9)
    },
    "cam_cfg\0edit_signal\0\0edit_slot\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_cam_cfg[] = {

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
       1,    1,   24,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       3,    0,   27,    2, 0x0a,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void cam_cfg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        cam_cfg *_t = static_cast<cam_cfg *>(_o);
        switch (_id) {
        case 0: _t->edit_signal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->edit_slot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (cam_cfg::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&cam_cfg::edit_signal)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject cam_cfg::staticMetaObject = {
    { &QRadioButton::staticMetaObject, qt_meta_stringdata_cam_cfg.data,
      qt_meta_data_cam_cfg,  qt_static_metacall, 0, 0}
};


const QMetaObject *cam_cfg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *cam_cfg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_cam_cfg.stringdata))
        return static_cast<void*>(const_cast< cam_cfg*>(this));
    return QRadioButton::qt_metacast(_clname);
}

int cam_cfg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QRadioButton::qt_metacall(_c, _id, _a);
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
void cam_cfg::edit_signal(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_ncQtConfig_t {
    QByteArrayData data[23];
    char stringdata[245];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_ncQtConfig_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_ncQtConfig_t qt_meta_stringdata_ncQtConfig = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 13),
QT_MOC_LITERAL(2, 25, 0),
QT_MOC_LITERAL(3, 26, 12),
QT_MOC_LITERAL(4, 39, 9),
QT_MOC_LITERAL(5, 49, 10),
QT_MOC_LITERAL(6, 60, 10),
QT_MOC_LITERAL(7, 71, 8),
QT_MOC_LITERAL(8, 80, 5),
QT_MOC_LITERAL(9, 86, 7),
QT_MOC_LITERAL(10, 94, 4),
QT_MOC_LITERAL(11, 99, 12),
QT_MOC_LITERAL(12, 112, 10),
QT_MOC_LITERAL(13, 123, 10),
QT_MOC_LITERAL(14, 134, 8),
QT_MOC_LITERAL(15, 143, 11),
QT_MOC_LITERAL(16, 155, 9),
QT_MOC_LITERAL(17, 165, 13),
QT_MOC_LITERAL(18, 179, 8),
QT_MOC_LITERAL(19, 188, 11),
QT_MOC_LITERAL(20, 200, 9),
QT_MOC_LITERAL(21, 210, 18),
QT_MOC_LITERAL(22, 229, 14)
    },
    "ncQtConfig\0valuesChanged\0\0configLoaded\0"
    "newConfig\0loadConfig\0editCamera\0"
    "setZnear\0znear\0setZfar\0zfar\0setStartPose\0"
    "start_pose\0setEndPose\0end_pose\0"
    "setSkipPose\0skip_pose\0setCameraFile\0"
    "cam_file\0setPoseFile\0pose_file\0"
    "updateActiveCamIDs\0saveConfigFile\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ncQtConfig[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x05,
       3,    0,   85,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       4,    0,   86,    2, 0x0a,
       5,    0,   87,    2, 0x0a,
       6,    1,   88,    2, 0x0a,
       7,    1,   91,    2, 0x0a,
       9,    1,   94,    2, 0x0a,
      11,    1,   97,    2, 0x0a,
      13,    1,  100,    2, 0x0a,
      15,    1,  103,    2, 0x0a,
      17,    1,  106,    2, 0x0a,
      19,    1,  109,    2, 0x0a,
      21,    0,  112,    2, 0x0a,
      22,    0,  113,    2, 0x0a,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Double,    8,
    QMetaType::Void, QMetaType::Double,   10,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::Int,   16,
    QMetaType::Void, QMetaType::QString,   18,
    QMetaType::Void, QMetaType::QString,   20,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ncQtConfig::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ncQtConfig *_t = static_cast<ncQtConfig *>(_o);
        switch (_id) {
        case 0: _t->valuesChanged(); break;
        case 1: _t->configLoaded(); break;
        case 2: _t->newConfig(); break;
        case 3: _t->loadConfig(); break;
        case 4: _t->editCamera((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->setZnear((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 6: _t->setZfar((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 7: _t->setStartPose((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->setEndPose((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->setSkipPose((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->setCameraFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: _t->setPoseFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 12: _t->updateActiveCamIDs(); break;
        case 13: _t->saveConfigFile(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ncQtConfig::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ncQtConfig::valuesChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (ncQtConfig::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ncQtConfig::configLoaded)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject ncQtConfig::staticMetaObject = {
    { &ncQtDock::staticMetaObject, qt_meta_stringdata_ncQtConfig.data,
      qt_meta_data_ncQtConfig,  qt_static_metacall, 0, 0}
};


const QMetaObject *ncQtConfig::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ncQtConfig::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ncQtConfig.stringdata))
        return static_cast<void*>(const_cast< ncQtConfig*>(this));
    if (!strcmp(_clname, "ncConfig"))
        return static_cast< ncConfig*>(const_cast< ncQtConfig*>(this));
    return ncQtDock::qt_metacast(_clname);
}

int ncQtConfig::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ncQtDock::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void ncQtConfig::valuesChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void ncQtConfig::configLoaded()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
