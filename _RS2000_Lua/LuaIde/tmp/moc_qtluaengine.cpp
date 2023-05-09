/****************************************************************************
** Meta object code from reading C++ file 'qtluaengine.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../qtluaengine.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qtluaengine.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QtLuaEngine_t {
    QByteArrayData data[30];
    char stringdata0[292];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QtLuaEngine_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QtLuaEngine_t qt_meta_stringdata_QtLuaEngine = {
    {
QT_MOC_LITERAL(0, 0, 11), // "QtLuaEngine"
QT_MOC_LITERAL(1, 12, 12), // "stateChanged"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 5), // "state"
QT_MOC_LITERAL(4, 32, 12), // "errorMessage"
QT_MOC_LITERAL(5, 45, 7), // "message"
QT_MOC_LITERAL(6, 53, 15), // "setPrintResults"
QT_MOC_LITERAL(7, 69, 14), // "setPrintErrors"
QT_MOC_LITERAL(8, 84, 15), // "setPauseOnError"
QT_MOC_LITERAL(9, 100, 4), // "stop"
QT_MOC_LITERAL(10, 105, 7), // "nopause"
QT_MOC_LITERAL(11, 113, 6), // "resume"
QT_MOC_LITERAL(12, 120, 10), // "nocontinue"
QT_MOC_LITERAL(13, 131, 4), // "eval"
QT_MOC_LITERAL(14, 136, 1), // "s"
QT_MOC_LITERAL(15, 138, 5), // "async"
QT_MOC_LITERAL(16, 144, 8), // "evaluate"
QT_MOC_LITERAL(17, 153, 16), // "lastErrorMessage"
QT_MOC_LITERAL(18, 170, 17), // "lastErrorLocation"
QT_MOC_LITERAL(19, 188, 12), // "printResults"
QT_MOC_LITERAL(20, 201, 11), // "printErrors"
QT_MOC_LITERAL(21, 213, 12), // "pauseOnError"
QT_MOC_LITERAL(22, 226, 17), // "runSignalHandlers"
QT_MOC_LITERAL(23, 244, 5), // "State"
QT_MOC_LITERAL(24, 250, 5), // "ready"
QT_MOC_LITERAL(25, 256, 7), // "running"
QT_MOC_LITERAL(26, 264, 6), // "paused"
QT_MOC_LITERAL(27, 271, 5), // "Ready"
QT_MOC_LITERAL(28, 277, 7), // "Running"
QT_MOC_LITERAL(29, 285, 6) // "Paused"

    },
    "QtLuaEngine\0stateChanged\0\0state\0"
    "errorMessage\0message\0setPrintResults\0"
    "setPrintErrors\0setPauseOnError\0stop\0"
    "nopause\0resume\0nocontinue\0eval\0s\0async\0"
    "evaluate\0lastErrorMessage\0lastErrorLocation\0"
    "printResults\0printErrors\0pauseOnError\0"
    "runSignalHandlers\0State\0ready\0running\0"
    "paused\0Ready\0Running\0Paused"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QtLuaEngine[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
      10,  134, // properties
       1,  164, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   89,    2, 0x06 /* Public */,
       4,    1,   92,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   95,    2, 0x0a /* Public */,
       7,    1,   98,    2, 0x0a /* Public */,
       8,    1,  101,    2, 0x0a /* Public */,
       9,    1,  104,    2, 0x0a /* Public */,
       9,    0,  107,    2, 0x2a /* Public | MethodCloned */,
      11,    1,  108,    2, 0x0a /* Public */,
      11,    0,  111,    2, 0x2a /* Public | MethodCloned */,
      13,    2,  112,    2, 0x0a /* Public */,
      13,    1,  117,    2, 0x2a /* Public | MethodCloned */,
      13,    2,  120,    2, 0x0a /* Public */,
      13,    1,  125,    2, 0x2a /* Public | MethodCloned */,
      16,    1,  128,    2, 0x0a /* Public */,
      16,    1,  131,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::QByteArray,    5,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Bool, QMetaType::Bool,   10,
    QMetaType::Bool,
    QMetaType::Bool, QMetaType::Bool,   12,
    QMetaType::Bool,
    QMetaType::Bool, QMetaType::QByteArray, QMetaType::Bool,   14,   15,
    QMetaType::Bool, QMetaType::QByteArray,   14,
    QMetaType::Bool, QMetaType::QString, QMetaType::Bool,   14,   15,
    QMetaType::Bool, QMetaType::QString,   14,
    QMetaType::QVariantList, QMetaType::QByteArray,   14,
    QMetaType::QVariantList, QMetaType::QString,   14,

 // properties: name, type, flags
      17, QMetaType::QByteArray, 0x00095001,
      18, QMetaType::QStringList, 0x00095001,
      19, QMetaType::Bool, 0x00095103,
      20, QMetaType::Bool, 0x00095103,
      21, QMetaType::Bool, 0x00095103,
      22, QMetaType::Bool, 0x00095001,
       3, 0x80000000 | 23, 0x00095009,
      24, QMetaType::Bool, 0x00095001,
      25, QMetaType::Bool, 0x00095001,
      26, QMetaType::Bool, 0x00095001,

 // enums: name, alias, flags, count, data
      23,   23, 0x0,    3,  169,

 // enum data: key, value
      27, uint(QtLuaEngine::Ready),
      28, uint(QtLuaEngine::Running),
      29, uint(QtLuaEngine::Paused),

       0        // eod
};

void QtLuaEngine::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<QtLuaEngine *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->errorMessage((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 2: _t->setPrintResults((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->setPrintErrors((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->setPauseOnError((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: { bool _r = _t->stop((*reinterpret_cast< bool(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 6: { bool _r = _t->stop();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 7: { bool _r = _t->resume((*reinterpret_cast< bool(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 8: { bool _r = _t->resume();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 9: { bool _r = _t->eval((*reinterpret_cast< QByteArray(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 10: { bool _r = _t->eval((*reinterpret_cast< QByteArray(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 11: { bool _r = _t->eval((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 12: { bool _r = _t->eval((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 13: { QVariantList _r = _t->evaluate((*reinterpret_cast< QByteArray(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 14: { QVariantList _r = _t->evaluate((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (QtLuaEngine::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QtLuaEngine::stateChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (QtLuaEngine::*)(QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QtLuaEngine::errorMessage)) {
                *result = 1;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<QtLuaEngine *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QByteArray*>(_v) = _t->lastErrorMessage(); break;
        case 1: *reinterpret_cast< QStringList*>(_v) = _t->lastErrorLocation(); break;
        case 2: *reinterpret_cast< bool*>(_v) = _t->printResults(); break;
        case 3: *reinterpret_cast< bool*>(_v) = _t->printErrors(); break;
        case 4: *reinterpret_cast< bool*>(_v) = _t->pauseOnError(); break;
        case 5: *reinterpret_cast< bool*>(_v) = _t->runSignalHandlers(); break;
        case 6: *reinterpret_cast< State*>(_v) = _t->state(); break;
        case 7: *reinterpret_cast< bool*>(_v) = _t->isReady(); break;
        case 8: *reinterpret_cast< bool*>(_v) = _t->isRunning(); break;
        case 9: *reinterpret_cast< bool*>(_v) = _t->isPaused(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<QtLuaEngine *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 2: _t->setPrintResults(*reinterpret_cast< bool*>(_v)); break;
        case 3: _t->setPrintErrors(*reinterpret_cast< bool*>(_v)); break;
        case 4: _t->setPauseOnError(*reinterpret_cast< bool*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject QtLuaEngine::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_QtLuaEngine.data,
    qt_meta_data_QtLuaEngine,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QtLuaEngine::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QtLuaEngine::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QtLuaEngine.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int QtLuaEngine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 10;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 10;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 10;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 10;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 10;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void QtLuaEngine::stateChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QtLuaEngine::errorMessage(QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
