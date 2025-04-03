/****************************************************************************
** Meta object code from reading C++ file 'mqttcomm.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../runtime/mqttcomm.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mqttcomm.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MqttComm_t {
    QByteArrayData data[15];
    char stringdata0[173];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MqttComm_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MqttComm_t qt_meta_stringdata_MqttComm = {
    {
QT_MOC_LITERAL(0, 0, 8), // "MqttComm"
QT_MOC_LITERAL(1, 9, 12), // "valueChanged"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 7), // "address"
QT_MOC_LITERAL(4, 31, 5), // "value"
QT_MOC_LITERAL(5, 37, 13), // "handleMessage"
QT_MOC_LITERAL(6, 51, 7), // "message"
QT_MOC_LITERAL(7, 59, 14), // "QMqttTopicName"
QT_MOC_LITERAL(8, 74, 5), // "topic"
QT_MOC_LITERAL(9, 80, 18), // "handleStateChanged"
QT_MOC_LITERAL(10, 99, 24), // "QMqttClient::ClientState"
QT_MOC_LITERAL(11, 124, 5), // "state"
QT_MOC_LITERAL(12, 130, 11), // "handleError"
QT_MOC_LITERAL(13, 142, 24), // "QMqttClient::ClientError"
QT_MOC_LITERAL(14, 167, 5) // "error"

    },
    "MqttComm\0valueChanged\0\0address\0value\0"
    "handleMessage\0message\0QMqttTopicName\0"
    "topic\0handleStateChanged\0"
    "QMqttClient::ClientState\0state\0"
    "handleError\0QMqttClient::ClientError\0"
    "error"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MqttComm[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    2,   39,    2, 0x08 /* Private */,
       9,    1,   44,    2, 0x08 /* Private */,
      12,    1,   47,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Double,    3,    4,

 // slots: parameters
    QMetaType::Void, QMetaType::QByteArray, 0x80000000 | 7,    6,    8,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, 0x80000000 | 13,   14,

       0        // eod
};

void MqttComm::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MqttComm *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->valueChanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 1: _t->handleMessage((*reinterpret_cast< const QByteArray(*)>(_a[1])),(*reinterpret_cast< const QMqttTopicName(*)>(_a[2]))); break;
        case 2: _t->handleStateChanged((*reinterpret_cast< QMqttClient::ClientState(*)>(_a[1]))); break;
        case 3: _t->handleError((*reinterpret_cast< QMqttClient::ClientError(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QMqttTopicName >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QMqttClient::ClientState >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QMqttClient::ClientError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MqttComm::*)(const QString & , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MqttComm::valueChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MqttComm::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_MqttComm.data,
    qt_meta_data_MqttComm,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MqttComm::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MqttComm::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MqttComm.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MqttComm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void MqttComm::valueChanged(const QString & _t1, double _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
