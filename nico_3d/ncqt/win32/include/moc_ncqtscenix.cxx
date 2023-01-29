/****************************************************************************
** Meta object code from reading C++ file 'ncqtscenix.h'
**
** Created: Thu Feb 20 14:38:53 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/ncqtscenix.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ncqtscenix.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ncQtScenix[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,
      42,   28,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      93,   81,   11,   11, 0x0a,
     139,  129,   11,   11, 0x0a,
     174,   11,   11,   11, 0x0a,
     186,   11,   11,   11, 0x0a,
     201,   11,   11,   11, 0x0a,
     214,   11,   11,   11, 0x0a,
     230,  227,   11,   11, 0x0a,
     255,   11,  251,   11, 0x0a,
     269,   11,   11,   11, 0x0a,
     289,   11,   11,   11, 0x0a,
     312,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ncQtScenix[] = {
    "ncQtScenix\0\0valuesChanged()\0center,radius\0"
    "boundingSphereChanged(ncc::v3d,double)\0"
    "item,column\0onItemChanged(QTreeWidgetItem*,int)\0"
    "KGL,MV,VP\0draw(ncc::m4x4d,ncc::RTd,ncc::v4i)\0"
    "addModels()\0removeModels()\0showModels()\0"
    "hideModels()\0ps\0setPointSize(double)\0"
    "int\0getNrModels()\0sendValuesChanged()\0"
    "resetCameraToVisible()\0resetCameraToSelected()\0"
};

void ncQtScenix::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
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
    }
}

const QMetaObjectExtraData ncQtScenix::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ncQtScenix::staticMetaObject = {
    { &ncQtDock::staticMetaObject, qt_meta_stringdata_ncQtScenix,
      qt_meta_data_ncQtScenix, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ncQtScenix::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ncQtScenix::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ncQtScenix::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ncQtScenix))
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
