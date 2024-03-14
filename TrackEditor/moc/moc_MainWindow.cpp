/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "..\TrackEditor.h"
#include "../MainWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CMainWindow_t {
    QByteArrayData data[21];
    char stringdata0[281];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CMainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CMainWindow_t qt_meta_stringdata_CMainWindow = {
    {
QT_MOC_LITERAL(0, 0, 11), // "CMainWindow"
QT_MOC_LITERAL(1, 12, 10), // "OnNewTrack"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 11), // "OnLoadTrack"
QT_MOC_LITERAL(4, 36, 11), // "OnSaveTrack"
QT_MOC_LITERAL(5, 48, 13), // "OnSaveTrackAs"
QT_MOC_LITERAL(6, 62, 15), // "OnImportMangled"
QT_MOC_LITERAL(7, 78, 15), // "OnExportMangled"
QT_MOC_LITERAL(8, 94, 7), // "OnDebug"
QT_MOC_LITERAL(9, 102, 7), // "OnAbout"
QT_MOC_LITERAL(10, 110, 22), // "OnSelChunksFromChanged"
QT_MOC_LITERAL(11, 133, 6), // "iValue"
QT_MOC_LITERAL(12, 140, 20), // "OnSelChunksToChanged"
QT_MOC_LITERAL(13, 161, 11), // "OnToChecked"
QT_MOC_LITERAL(14, 173, 8), // "bChecked"
QT_MOC_LITERAL(15, 182, 14), // "OnApplyClicked"
QT_MOC_LITERAL(16, 197, 15), // "OnCancelClicked"
QT_MOC_LITERAL(17, 213, 14), // "OnEditLSurface"
QT_MOC_LITERAL(18, 228, 14), // "OnEditCSurface"
QT_MOC_LITERAL(19, 243, 14), // "OnEditRSurface"
QT_MOC_LITERAL(20, 258, 22) // "UpdateGeometryEditMode"

    },
    "CMainWindow\0OnNewTrack\0\0OnLoadTrack\0"
    "OnSaveTrack\0OnSaveTrackAs\0OnImportMangled\0"
    "OnExportMangled\0OnDebug\0OnAbout\0"
    "OnSelChunksFromChanged\0iValue\0"
    "OnSelChunksToChanged\0OnToChecked\0"
    "bChecked\0OnApplyClicked\0OnCancelClicked\0"
    "OnEditLSurface\0OnEditCSurface\0"
    "OnEditRSurface\0UpdateGeometryEditMode"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CMainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   99,    2, 0x09 /* Protected */,
       3,    0,  100,    2, 0x09 /* Protected */,
       4,    0,  101,    2, 0x09 /* Protected */,
       5,    0,  102,    2, 0x09 /* Protected */,
       6,    0,  103,    2, 0x09 /* Protected */,
       7,    0,  104,    2, 0x09 /* Protected */,
       8,    0,  105,    2, 0x09 /* Protected */,
       9,    0,  106,    2, 0x09 /* Protected */,
      10,    1,  107,    2, 0x09 /* Protected */,
      12,    1,  110,    2, 0x09 /* Protected */,
      13,    1,  113,    2, 0x09 /* Protected */,
      15,    0,  116,    2, 0x09 /* Protected */,
      16,    0,  117,    2, 0x09 /* Protected */,
      17,    0,  118,    2, 0x09 /* Protected */,
      18,    0,  119,    2, 0x09 /* Protected */,
      19,    0,  120,    2, 0x09 /* Protected */,
      20,    0,  121,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void, QMetaType::Bool,   14,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CMainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CMainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->OnNewTrack(); break;
        case 1: _t->OnLoadTrack(); break;
        case 2: _t->OnSaveTrack(); break;
        case 3: _t->OnSaveTrackAs(); break;
        case 4: _t->OnImportMangled(); break;
        case 5: _t->OnExportMangled(); break;
        case 6: _t->OnDebug(); break;
        case 7: _t->OnAbout(); break;
        case 8: _t->OnSelChunksFromChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->OnSelChunksToChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->OnToChecked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->OnApplyClicked(); break;
        case 12: _t->OnCancelClicked(); break;
        case 13: _t->OnEditLSurface(); break;
        case 14: _t->OnEditCSurface(); break;
        case 15: _t->OnEditRSurface(); break;
        case 16: _t->UpdateGeometryEditMode(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CMainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CMainWindow.data,
    qt_meta_data_CMainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CMainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int CMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 17;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
