/****************************************************************************
** Meta object code from reading C++ file 'componenteditor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../scada/componenteditor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'componenteditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ComponentEditor_t {
    QByteArrayData data[7];
    char stringdata0[85];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ComponentEditor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ComponentEditor_t qt_meta_stringdata_ComponentEditor = {
    {
QT_MOC_LITERAL(0, 0, 15), // "ComponentEditor"
QT_MOC_LITERAL(1, 16, 20), // "onColorButtonClicked"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 13), // "onSizeChanged"
QT_MOC_LITERAL(4, 52, 5), // "value"
QT_MOC_LITERAL(5, 58, 17), // "onRotationChanged"
QT_MOC_LITERAL(6, 76, 8) // "onAccept"

    },
    "ComponentEditor\0onColorButtonClicked\0"
    "\0onSizeChanged\0value\0onRotationChanged\0"
    "onAccept"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ComponentEditor[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x08 /* Private */,
       3,    1,   35,    2, 0x08 /* Private */,
       5,    1,   38,    2, 0x08 /* Private */,
       6,    0,   41,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,

       0        // eod
};

void ComponentEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ComponentEditor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onColorButtonClicked(); break;
        case 1: _t->onSizeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->onRotationChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->onAccept(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ComponentEditor::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_ComponentEditor.data,
    qt_meta_data_ComponentEditor,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ComponentEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ComponentEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ComponentEditor.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int ComponentEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
