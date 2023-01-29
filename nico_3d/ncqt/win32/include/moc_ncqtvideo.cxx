/****************************************************************************
** Meta object code from reading C++ file 'ncqtvideo.h'
**
** Created: Thu Feb 20 14:38:53 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/ncqtvideo.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ncqtvideo.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ncQtVideo[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      18,   11,   10,   10, 0x05,
      53,   10,   10,   10, 0x05,
      75,   10,   10,   10, 0x05,
      89,   10,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
     102,   10,   10,   10, 0x0a,
     109,   10,   10,   10, 0x0a,
     117,   10,   10,   10, 0x0a,
     130,   10,   10,   10, 0x0a,
     138,   10,   10,   10, 0x0a,
     146,   10,   10,   10, 0x0a,
     152,   10,   10,   10, 0x0a,
     161,   10,   10,   10, 0x0a,
     176,   10,   10,   10, 0x0a,
     190,   10,   10,   10, 0x0a,
     204,   10,   10,   10, 0x0a,
     223,  217,   10,   10, 0x0a,
     239,  217,   10,   10, 0x0a,
     257,   10,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ncQtVideo[] = {
    "ncQtVideo\0\0VP,KGL\0cameraChanged(ncc::v4i,ncc::m4x4d)\0"
    "poseChanged(ncc::RTd)\0startRecord()\0"
    "stopRecord()\0play()\0pause()\0togglePlay()\0"
    "reset()\0start()\0end()\0record()\0"
    "loadPoseFile()\0loadCamFile()\0showCamFile()\0"
    "showSlider()\0index\0changePose(int)\0"
    "changeCamera(int)\0next()\0"
};

void ncQtVideo::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ncQtVideo *_t = static_cast<ncQtVideo *>(_o);
        switch (_id) {
        case 0: _t->cameraChanged((*reinterpret_cast< ncc::v4i(*)>(_a[1])),(*reinterpret_cast< ncc::m4x4d(*)>(_a[2]))); break;
        case 1: _t->poseChanged((*reinterpret_cast< ncc::RTd(*)>(_a[1]))); break;
        case 2: _t->startRecord(); break;
        case 3: _t->stopRecord(); break;
        case 4: _t->play(); break;
        case 5: _t->pause(); break;
        case 6: _t->togglePlay(); break;
        case 7: _t->reset(); break;
        case 8: _t->start(); break;
        case 9: _t->end(); break;
        case 10: _t->record(); break;
        case 11: _t->loadPoseFile(); break;
        case 12: _t->loadCamFile(); break;
        case 13: _t->showCamFile(); break;
        case 14: _t->showSlider(); break;
        case 15: _t->changePose((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->changeCamera((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->next(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ncQtVideo::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ncQtVideo::staticMetaObject = {
    { &ncQtDock::staticMetaObject, qt_meta_stringdata_ncQtVideo,
      qt_meta_data_ncQtVideo, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ncQtVideo::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ncQtVideo::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ncQtVideo::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ncQtVideo))
        return static_cast<void*>(const_cast< ncQtVideo*>(this));
    return ncQtDock::qt_metacast(_clname);
}

int ncQtVideo::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ncQtDock::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void ncQtVideo::cameraChanged(ncc::v4i _t1, ncc::m4x4d _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ncQtVideo::poseChanged(ncc::RTd _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ncQtVideo::startRecord()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void ncQtVideo::stopRecord()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
