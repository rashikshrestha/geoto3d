/****************************************************************************
** Meta object code from reading C++ file 'ncqtconfig.h'
**
** Created: Thu Feb 20 14:38:57 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/ncqtconfig.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ncqtconfig.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_cam_cfg[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x05,
      28,   26,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
      52,    8,    8,    8, 0x0a,
      64,    8,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_cam_cfg[] = {
    "cam_cfg\0\0edit_signal(int)\0,\0"
    "toggle_signal(int,bool)\0edit_slot()\0"
    "toggle_slot(bool)\0"
};

void cam_cfg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        cam_cfg *_t = static_cast<cam_cfg *>(_o);
        switch (_id) {
        case 0: _t->edit_signal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->toggle_signal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 2: _t->edit_slot(); break;
        case 3: _t->toggle_slot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData cam_cfg::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject cam_cfg::staticMetaObject = {
    { &QRadioButton::staticMetaObject, qt_meta_stringdata_cam_cfg,
      qt_meta_data_cam_cfg, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &cam_cfg::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *cam_cfg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *cam_cfg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_cam_cfg))
        return static_cast<void*>(const_cast< cam_cfg*>(this));
    return QRadioButton::qt_metacast(_clname);
}

int cam_cfg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QRadioButton::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void cam_cfg::edit_signal(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void cam_cfg::toggle_signal(int _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
static const uint qt_meta_data_ncQtConfig[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,
      37,   28,   11,   11, 0x05,
      59,   28,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      81,   11,   11,   11, 0x0a,
      93,   11,   11,   11, 0x0a,
     106,   11,   11,   11, 0x0a,
     124,  122,   11,   11, 0x0a,
     153,  147,   11,   11, 0x0a,
     175,  170,   11,   11, 0x0a,
     198,  191,   11,   11, 0x0a,
     222,  216,   11,   11, 0x0a,
     245,  239,   11,   11, 0x0a,
     262,   11,   11,   11, 0x0a,
     281,  270,   11,   11, 0x0a,
     308,  299,   11,   11, 0x0a,
     334,  324,   11,   11, 0x0a,
     363,  354,   11,   11, 0x0a,
     396,  386,   11,   11, 0x0a,
     417,   11,   11,   11, 0x0a,
     438,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ncQtConfig[] = {
    "ncQtConfig\0\0valuesChanged()\0filename\0"
    "configLoaded(QString)\0configCopied(QString)\0"
    "newConfig()\0loadConfig()\0editCamera(int)\0"
    ",\0toggleCamera(int,bool)\0znear\0"
    "setZnear(double)\0zfar\0setZfar(double)\0"
    "vignet\0setVignet(double)\0gamma\0"
    "setGamma(double)\0scale\0setScale(double)\0"
    "setWB()\0start_pose\0setStartPose(int)\0"
    "end_pose\0setEndPose(int)\0step_pose\0"
    "setStepPose(double)\0cam_file\0"
    "setCameraFile(QString)\0pose_file\0"
    "setPoseFile(QString)\0updateActiveCamIDs()\0"
    "copyConfigFile()\0"
};

void ncQtConfig::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ncQtConfig *_t = static_cast<ncQtConfig *>(_o);
        switch (_id) {
        case 0: _t->valuesChanged(); break;
        case 1: _t->configLoaded((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->configCopied((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->newConfig(); break;
        case 4: _t->loadConfig(); break;
        case 5: _t->editCamera((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->toggleCamera((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 7: _t->setZnear((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 8: _t->setZfar((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 9: _t->setVignet((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 10: _t->setGamma((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 11: _t->setScale((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 12: _t->setWB(); break;
        case 13: _t->setStartPose((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->setEndPose((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->setStepPose((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 16: _t->setCameraFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 17: _t->setPoseFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 18: _t->updateActiveCamIDs(); break;
        case 19: _t->copyConfigFile(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ncQtConfig::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ncQtConfig::staticMetaObject = {
    { &ncQtDock::staticMetaObject, qt_meta_stringdata_ncQtConfig,
      qt_meta_data_ncQtConfig, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ncQtConfig::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ncQtConfig::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ncQtConfig::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ncQtConfig))
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
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    }
    return _id;
}

// SIGNAL 0
void ncQtConfig::valuesChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void ncQtConfig::configLoaded(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ncQtConfig::configCopied(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
