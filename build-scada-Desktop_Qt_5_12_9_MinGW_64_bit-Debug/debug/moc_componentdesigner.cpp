/****************************************************************************
** Meta object code from reading C++ file 'componentdesigner.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../scada/componentdesigner.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'componentdesigner.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ComponentDesigner_t {
    QByteArrayData data[11];
    char stringdata0[145];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ComponentDesigner_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ComponentDesigner_t qt_meta_stringdata_ComponentDesigner = {
    {
QT_MOC_LITERAL(0, 0, 17), // "ComponentDesigner"
QT_MOC_LITERAL(1, 18, 23), // "componentLibraryChanged"
QT_MOC_LITERAL(2, 42, 0), // ""
QT_MOC_LITERAL(3, 43, 17), // "propertiesChanged"
QT_MOC_LITERAL(4, 61, 7), // "addText"
QT_MOC_LITERAL(5, 69, 13), // "saveComponent"
QT_MOC_LITERAL(6, 83, 13), // "loadComponent"
QT_MOC_LITERAL(7, 97, 8), // "filename"
QT_MOC_LITERAL(8, 106, 14), // "deleteSelected"
QT_MOC_LITERAL(9, 121, 17), // "onCategoryChanged"
QT_MOC_LITERAL(10, 139, 5) // "index"

    },
    "ComponentDesigner\0componentLibraryChanged\0"
    "\0propertiesChanged\0addText\0saveComponent\0"
    "loadComponent\0filename\0deleteSelected\0"
    "onCategoryChanged\0index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ComponentDesigner[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x06 /* Public */,
       3,    0,   55,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   56,    2, 0x08 /* Private */,
       5,    0,   57,    2, 0x08 /* Private */,
       6,    0,   58,    2, 0x08 /* Private */,
       6,    1,   59,    2, 0x08 /* Private */,
       8,    0,   62,    2, 0x08 /* Private */,
       9,    1,   63,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,

       0        // eod
};

void ComponentDesigner::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ComponentDesigner *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->componentLibraryChanged(); break;
        case 1: _t->propertiesChanged(); break;
        case 2: _t->addText(); break;
        case 3: _t->saveComponent(); break;
        case 4: _t->loadComponent(); break;
        case 5: _t->loadComponent((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->deleteSelected(); break;
        case 7: _t->onCategoryChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ComponentDesigner::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ComponentDesigner::componentLibraryChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ComponentDesigner::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ComponentDesigner::propertiesChanged)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ComponentDesigner::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_ComponentDesigner.data,
    qt_meta_data_ComponentDesigner,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ComponentDesigner::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ComponentDesigner::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ComponentDesigner.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int ComponentDesigner::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

// SIGNAL 0
void ComponentDesigner::componentLibraryChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ComponentDesigner::propertiesChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
