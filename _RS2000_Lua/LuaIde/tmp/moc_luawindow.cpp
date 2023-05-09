/****************************************************************************
** Meta object code from reading C++ file 'luawindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../luawindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'luawindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LuaWindow_t {
    QByteArrayData data[11];
    char stringdata0[206];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LuaWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LuaWindow_t qt_meta_stringdata_LuaWindow = {
    {
QT_MOC_LITERAL(0, 0, 9), // "LuaWindow"
QT_MOC_LITERAL(1, 10, 14), // "OnErrorMessage"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 7), // "message"
QT_MOC_LITERAL(4, 34, 27), // "on_actionStartLua_triggered"
QT_MOC_LITERAL(5, 62, 26), // "on_actionStopLua_triggered"
QT_MOC_LITERAL(6, 89, 29), // "on_actionRestartLua_triggered"
QT_MOC_LITERAL(7, 119, 19), // "on_brnNext1_clicked"
QT_MOC_LITERAL(8, 139, 19), // "on_btnNext2_clicked"
QT_MOC_LITERAL(9, 159, 21), // "on_btnReplace_clicked"
QT_MOC_LITERAL(10, 181, 24) // "on_btnReplaceAll_clicked"

    },
    "LuaWindow\0OnErrorMessage\0\0message\0"
    "on_actionStartLua_triggered\0"
    "on_actionStopLua_triggered\0"
    "on_actionRestartLua_triggered\0"
    "on_brnNext1_clicked\0on_btnNext2_clicked\0"
    "on_btnReplace_clicked\0on_btnReplaceAll_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LuaWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x08 /* Private */,
       4,    0,   57,    2, 0x08 /* Private */,
       5,    0,   58,    2, 0x08 /* Private */,
       6,    0,   59,    2, 0x08 /* Private */,
       7,    0,   60,    2, 0x08 /* Private */,
       8,    0,   61,    2, 0x08 /* Private */,
       9,    0,   62,    2, 0x08 /* Private */,
      10,    0,   63,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QByteArray,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void LuaWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LuaWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->OnErrorMessage((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 1: _t->on_actionStartLua_triggered(); break;
        case 2: _t->on_actionStopLua_triggered(); break;
        case 3: _t->on_actionRestartLua_triggered(); break;
        case 4: _t->on_brnNext1_clicked(); break;
        case 5: _t->on_btnNext2_clicked(); break;
        case 6: _t->on_btnReplace_clicked(); break;
        case 7: _t->on_btnReplaceAll_clicked(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject LuaWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_LuaWindow.data,
    qt_meta_data_LuaWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *LuaWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LuaWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LuaWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int LuaWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
