/****************************************************************************
** Meta object code from reading C++ file 'ncqtconfigmanager.h'
**
** Created: Thu Feb 20 14:38:57 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/ncqtconfigmanager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ncqtconfigmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ncQtConfigManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      28,   19,   18,   18, 0x05,
      49,   19,   18,   18, 0x05,
      72,   19,   18,   18, 0x05,
     106,   96,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
     132,   18,   18,   18, 0x0a,
     162,  153,   18,   18, 0x0a,
     218,   18,   18,   18, 0x0a,
     235,   18,   18,   18, 0x0a,
     249,   18,   18,   18, 0x0a,
     261,   18,   18,   18, 0x0a,
     274,   18,   18,   18, 0x0a,
     290,   18,   18,   18, 0x0a,
     309,   18,  305,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ncQtConfigManager[] = {
    "ncQtConfigManager\0\0filename\0"
    "configAdded(QString)\0configRemoved(QString)\0"
    "configSelected(QString)\0file_list\0"
    "processBatch(QStringList)\0"
    "onNrConfigsChanged()\0cur,prev\0"
    "onCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)\0"
    "onProcessBatch()\0onSaveBatch()\0"
    "loadBatch()\0addConfigs()\0removeConfigs()\0"
    "clearConfigs()\0int\0getNrConfigs()\0"
};

void ncQtConfigManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ncQtConfigManager *_t = static_cast<ncQtConfigManager *>(_o);
        switch (_id) {
        case 0: _t->configAdded((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->configRemoved((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->configSelected((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->processBatch((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 4: _t->onNrConfigsChanged(); break;
        case 5: _t->onCurrentItemChanged((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QTreeWidgetItem*(*)>(_a[2]))); break;
        case 6: _t->onProcessBatch(); break;
        case 7: _t->onSaveBatch(); break;
        case 8: _t->loadBatch(); break;
        case 9: _t->addConfigs(); break;
        case 10: _t->removeConfigs(); break;
        case 11: _t->clearConfigs(); break;
        case 12: { int _r = _t->getNrConfigs();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ncQtConfigManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ncQtConfigManager::staticMetaObject = {
    { &ncQtDock::staticMetaObject, qt_meta_stringdata_ncQtConfigManager,
      qt_meta_data_ncQtConfigManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ncQtConfigManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ncQtConfigManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ncQtConfigManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ncQtConfigManager))
        return static_cast<void*>(const_cast< ncQtConfigManager*>(this));
    return ncQtDock::qt_metacast(_clname);
}

int ncQtConfigManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void ncQtConfigManager::configAdded(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ncQtConfigManager::configRemoved(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ncQtConfigManager::configSelected(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ncQtConfigManager::processBatch(QStringList _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
