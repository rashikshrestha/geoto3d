/****************************************************************************
** Meta object code from reading C++ file 'ncqtvideo.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.0.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/ncqtvideo.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ncqtvideo.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.0.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ncQtVideo_t {
    QByteArrayData data[26];
    char stringdata[225];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_ncQtVideo_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_ncQtVideo_t qt_meta_stringdata_ncQtVideo = {
    {
QT_MOC_LITERAL(0, 0, 9),
QT_MOC_LITERAL(1, 10, 13),
QT_MOC_LITERAL(2, 24, 0),
QT_MOC_LITERAL(3, 25, 8),
QT_MOC_LITERAL(4, 34, 2),
QT_MOC_LITERAL(5, 37, 10),
QT_MOC_LITERAL(6, 48, 3),
QT_MOC_LITERAL(7, 52, 11),
QT_MOC_LITERAL(8, 64, 8),
QT_MOC_LITERAL(9, 73, 11),
QT_MOC_LITERAL(10, 85, 10),
QT_MOC_LITERAL(11, 96, 4),
QT_MOC_LITERAL(12, 101, 5),
QT_MOC_LITERAL(13, 107, 10),
QT_MOC_LITERAL(14, 118, 5),
QT_MOC_LITERAL(15, 124, 5),
QT_MOC_LITERAL(16, 130, 3),
QT_MOC_LITERAL(17, 134, 6),
QT_MOC_LITERAL(18, 141, 12),
QT_MOC_LITERAL(19, 154, 11),
QT_MOC_LITERAL(20, 166, 11),
QT_MOC_LITERAL(21, 178, 10),
QT_MOC_LITERAL(22, 189, 10),
QT_MOC_LITERAL(23, 200, 5),
QT_MOC_LITERAL(24, 206, 12),
QT_MOC_LITERAL(25, 219, 4)
    },
    "ncQtVideo\0cameraChanged\0\0ncc::v4i\0VP\0"
    "ncc::m4x4d\0KGL\0poseChanged\0ncc::RTd\0"
    "startRecord\0stopRecord\0play\0pause\0"
    "togglePlay\0reset\0start\0end\0record\0"
    "loadPoseFile\0loadCamFile\0showCamFile\0"
    "showSlider\0changePose\0index\0changeCamera\0"
    "next\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ncQtVideo[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,  104,    2, 0x05,
       7,    1,  109,    2, 0x05,
       9,    0,  112,    2, 0x05,
      10,    0,  113,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
      11,    0,  114,    2, 0x0a,
      12,    0,  115,    2, 0x0a,
      13,    0,  116,    2, 0x0a,
      14,    0,  117,    2, 0x0a,
      15,    0,  118,    2, 0x0a,
      16,    0,  119,    2, 0x0a,
      17,    0,  120,    2, 0x0a,
      18,    0,  121,    2, 0x0a,
      19,    0,  122,    2, 0x0a,
      20,    0,  123,    2, 0x0a,
      21,    0,  124,    2, 0x0a,
      22,    1,  125,    2, 0x0a,
      24,    1,  128,    2, 0x0a,
      25,    0,  131,    2, 0x0a,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,
    QMetaType::Void, 0x80000000 | 8,    2,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   23,
    QMetaType::Void, QMetaType::Int,   23,
    QMetaType::Void,

       0        // eod
};

void ncQtVideo::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
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
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ncQtVideo::*_t)(ncc::v4i , ncc::m4x4d );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ncQtVideo::cameraChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (ncQtVideo::*_t)(ncc::RTd );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ncQtVideo::poseChanged)) {
                *result = 1;
            }
        }
        {
            typedef void (ncQtVideo::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ncQtVideo::startRecord)) {
                *result = 2;
            }
        }
        {
            typedef void (ncQtVideo::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ncQtVideo::stopRecord)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject ncQtVideo::staticMetaObject = {
    { &ncQtDock::staticMetaObject, qt_meta_stringdata_ncQtVideo.data,
      qt_meta_data_ncQtVideo,  qt_static_metacall, 0, 0}
};


const QMetaObject *ncQtVideo::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ncQtVideo::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ncQtVideo.stringdata))
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
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            *reinterpret_cast<int*>(_a[0]) = -1;
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
