/****************************************************************************
** Meta object code from reading C++ file 'tcpclienthandler.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../tcpclienthandler.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tcpclienthandler.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSTcpClientHandlerENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSTcpClientHandlerENDCLASS = QtMocHelpers::stringData(
    "TcpClientHandler",
    "isConnectedChanged",
    "",
    "statusChanged",
    "progressChanged",
    "currentFileChanged",
    "fileSentSuccessfully",
    "fileName",
    "errorOccurred",
    "errorMessage",
    "onConnected",
    "onDisconnected",
    "onBytesWritten",
    "bytes",
    "onErrorOccurred",
    "QAbstractSocket::SocketError",
    "error",
    "connectToServer",
    "address",
    "port",
    "disconnectFromServer",
    "sendFile",
    "filePath",
    "isConnected",
    "status",
    "progress",
    "currentFile"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSTcpClientHandlerENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       4,  119, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   92,    2, 0x06,    5 /* Public */,
       3,    0,   93,    2, 0x06,    6 /* Public */,
       4,    0,   94,    2, 0x06,    7 /* Public */,
       5,    0,   95,    2, 0x06,    8 /* Public */,
       6,    1,   96,    2, 0x06,    9 /* Public */,
       8,    1,   99,    2, 0x06,   11 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      10,    0,  102,    2, 0x08,   13 /* Private */,
      11,    0,  103,    2, 0x08,   14 /* Private */,
      12,    1,  104,    2, 0x08,   15 /* Private */,
      14,    1,  107,    2, 0x08,   17 /* Private */,

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
      17,    2,  110,    2, 0x02,   19 /* Public */,
      20,    0,  115,    2, 0x02,   22 /* Public */,
      21,    1,  116,    2, 0x02,   23 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QString,    9,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::LongLong,   13,
    QMetaType::Void, 0x80000000 | 15,   16,

 // methods: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::UShort,   18,   19,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   22,

 // properties: name, type, flags
      23, QMetaType::Bool, 0x00015001, uint(0), 0,
      24, QMetaType::QString, 0x00015001, uint(1), 0,
      25, QMetaType::Int, 0x00015001, uint(2), 0,
      26, QMetaType::QString, 0x00015001, uint(3), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject TcpClientHandler::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSTcpClientHandlerENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSTcpClientHandlerENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSTcpClientHandlerENDCLASS_t,
        // property 'isConnected'
        QtPrivate::TypeAndForceComplete<bool, std::true_type>,
        // property 'status'
        QtPrivate::TypeAndForceComplete<QString, std::true_type>,
        // property 'progress'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // property 'currentFile'
        QtPrivate::TypeAndForceComplete<QString, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<TcpClientHandler, std::true_type>,
        // method 'isConnectedChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'statusChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'progressChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'currentFileChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'fileSentSuccessfully'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'errorOccurred'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onConnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onDisconnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onBytesWritten'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<qint64, std::false_type>,
        // method 'onErrorOccurred'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QAbstractSocket::SocketError, std::false_type>,
        // method 'connectToServer'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<quint16, std::false_type>,
        // method 'disconnectFromServer'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'sendFile'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
    >,
    nullptr
} };

void TcpClientHandler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TcpClientHandler *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->isConnectedChanged(); break;
        case 1: _t->statusChanged(); break;
        case 2: _t->progressChanged(); break;
        case 3: _t->currentFileChanged(); break;
        case 4: _t->fileSentSuccessfully((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: _t->errorOccurred((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->onConnected(); break;
        case 7: _t->onDisconnected(); break;
        case 8: _t->onBytesWritten((*reinterpret_cast< std::add_pointer_t<qint64>>(_a[1]))); break;
        case 9: _t->onErrorOccurred((*reinterpret_cast< std::add_pointer_t<QAbstractSocket::SocketError>>(_a[1]))); break;
        case 10: _t->connectToServer((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<quint16>>(_a[2]))); break;
        case 11: _t->disconnectFromServer(); break;
        case 12: _t->sendFile((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TcpClientHandler::*)();
            if (_t _q_method = &TcpClientHandler::isConnectedChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (TcpClientHandler::*)();
            if (_t _q_method = &TcpClientHandler::statusChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (TcpClientHandler::*)();
            if (_t _q_method = &TcpClientHandler::progressChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (TcpClientHandler::*)();
            if (_t _q_method = &TcpClientHandler::currentFileChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (TcpClientHandler::*)(const QString & );
            if (_t _q_method = &TcpClientHandler::fileSentSuccessfully; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (TcpClientHandler::*)(const QString & );
            if (_t _q_method = &TcpClientHandler::errorOccurred; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
    } else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<TcpClientHandler *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = _t->isConnected(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->status(); break;
        case 2: *reinterpret_cast< int*>(_v) = _t->progress(); break;
        case 3: *reinterpret_cast< QString*>(_v) = _t->currentFile(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
}

const QMetaObject *TcpClientHandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TcpClientHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSTcpClientHandlerENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int TcpClientHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void TcpClientHandler::isConnectedChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void TcpClientHandler::statusChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void TcpClientHandler::progressChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void TcpClientHandler::currentFileChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void TcpClientHandler::fileSentSuccessfully(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void TcpClientHandler::errorOccurred(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
