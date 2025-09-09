/****************************************************************************
** Meta object code from reading C++ file 'tcpserverhandler.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../tcpserverhandler.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tcpserverhandler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSTcpServerHandlerENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSTcpServerHandlerENDCLASS = QtMocHelpers::stringData(
    "TcpServerHandler",
    "serverStatusChanged",
    "",
    "serverIpPortChanged",
    "receivedFileNameChanged",
    "fileSaved",
    "success",
    "path",
    "onNewConnection",
    "onReadyRead",
    "onClientDisconnected",
    "startServer",
    "stopServer",
    "serverStatus",
    "serverIpPort",
    "receivedFileName"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSTcpServerHandlerENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       3,   81, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   68,    2, 0x06,    4 /* Public */,
       3,    0,   69,    2, 0x06,    5 /* Public */,
       4,    0,   70,    2, 0x06,    6 /* Public */,
       5,    2,   71,    2, 0x06,    7 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       8,    0,   76,    2, 0x08,   10 /* Private */,
       9,    0,   77,    2, 0x08,   11 /* Private */,
      10,    0,   78,    2, 0x08,   12 /* Private */,

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
      11,    0,   79,    2, 0x02,   13 /* Public */,
      12,    0,   80,    2, 0x02,   14 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    6,    7,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // methods: parameters
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
      13, QMetaType::QString, 0x00015001, uint(0), 0,
      14, QMetaType::QString, 0x00015001, uint(1), 0,
      15, QMetaType::QString, 0x00015001, uint(2), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject TcpServerHandler::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSTcpServerHandlerENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSTcpServerHandlerENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSTcpServerHandlerENDCLASS_t,
        // property 'serverStatus'
        QtPrivate::TypeAndForceComplete<QString, std::true_type>,
        // property 'serverIpPort'
        QtPrivate::TypeAndForceComplete<QString, std::true_type>,
        // property 'receivedFileName'
        QtPrivate::TypeAndForceComplete<QString, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<TcpServerHandler, std::true_type>,
        // method 'serverStatusChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'serverIpPortChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'receivedFileNameChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'fileSaved'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onNewConnection'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onReadyRead'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onClientDisconnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'startServer'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'stopServer'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void TcpServerHandler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TcpServerHandler *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->serverStatusChanged(); break;
        case 1: _t->serverIpPortChanged(); break;
        case 2: _t->receivedFileNameChanged(); break;
        case 3: _t->fileSaved((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 4: _t->onNewConnection(); break;
        case 5: _t->onReadyRead(); break;
        case 6: _t->onClientDisconnected(); break;
        case 7: _t->startServer(); break;
        case 8: _t->stopServer(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TcpServerHandler::*)();
            if (_t _q_method = &TcpServerHandler::serverStatusChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (TcpServerHandler::*)();
            if (_t _q_method = &TcpServerHandler::serverIpPortChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (TcpServerHandler::*)();
            if (_t _q_method = &TcpServerHandler::receivedFileNameChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (TcpServerHandler::*)(bool , const QString & );
            if (_t _q_method = &TcpServerHandler::fileSaved; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
    } else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<TcpServerHandler *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->serverStatus(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->serverIpPort(); break;
        case 2: *reinterpret_cast< QString*>(_v) = _t->receivedFileName(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
}

const QMetaObject *TcpServerHandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TcpServerHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSTcpServerHandlerENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int TcpServerHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void TcpServerHandler::serverStatusChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void TcpServerHandler::serverIpPortChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void TcpServerHandler::receivedFileNameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void TcpServerHandler::fileSaved(bool _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
