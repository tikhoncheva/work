/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../nouna_project/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[22];
    char stringdata[468];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 29),
QT_MOC_LITERAL(2, 41, 0),
QT_MOC_LITERAL(3, 42, 26),
QT_MOC_LITERAL(4, 69, 27),
QT_MOC_LITERAL(5, 97, 21),
QT_MOC_LITERAL(6, 119, 31),
QT_MOC_LITERAL(7, 151, 28),
QT_MOC_LITERAL(8, 180, 35),
QT_MOC_LITERAL(9, 216, 5),
QT_MOC_LITERAL(10, 222, 33),
QT_MOC_LITERAL(11, 256, 12),
QT_MOC_LITERAL(12, 269, 8),
QT_MOC_LITERAL(13, 278, 5),
QT_MOC_LITERAL(14, 284, 12),
QT_MOC_LITERAL(15, 297, 36),
QT_MOC_LITERAL(16, 334, 30),
QT_MOC_LITERAL(17, 365, 29),
QT_MOC_LITERAL(18, 395, 12),
QT_MOC_LITERAL(19, 408, 11),
QT_MOC_LITERAL(20, 420, 30),
QT_MOC_LITERAL(21, 451, 16)
    },
    "MainWindow\0on_buttonOpenVillages_clicked\0"
    "\0on_buttonOpenRoads_clicked\0"
    "on_buttonOpenHouseh_clicked\0"
    "on_buttonPlot_clicked\0"
    "on_checkBoxVillageNames_clicked\0"
    "on_checkBoxShowRoads_clicked\0"
    "on_horizontalScrollBar_valueChanged\0"
    "value\0on_verticalScrollBar_valueChanged\0"
    "xAxisChanged\0QCPRange\0range\0yAxisChanged\0"
    "on_pushButtonInitialSolution_clicked\0"
    "on_pushButtonShowRoute_pressed\0"
    "on_checkBoxVillageIDs_clicked\0"
    "weekSelected\0daySelected\0"
    "on_pushButtonShowRoute_clicked\0"
    "showReportWindow"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   99,    2, 0x08 /* Private */,
       3,    0,  100,    2, 0x08 /* Private */,
       4,    0,  101,    2, 0x08 /* Private */,
       5,    0,  102,    2, 0x08 /* Private */,
       6,    0,  103,    2, 0x08 /* Private */,
       7,    0,  104,    2, 0x08 /* Private */,
       8,    1,  105,    2, 0x08 /* Private */,
      10,    1,  108,    2, 0x08 /* Private */,
      11,    1,  111,    2, 0x08 /* Private */,
      14,    1,  114,    2, 0x08 /* Private */,
      15,    0,  117,    2, 0x08 /* Private */,
      16,    0,  118,    2, 0x08 /* Private */,
      17,    0,  119,    2, 0x08 /* Private */,
      18,    2,  120,    2, 0x08 /* Private */,
      19,    2,  125,    2, 0x08 /* Private */,
      20,    0,  130,    2, 0x08 /* Private */,
      21,    0,  131,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->on_buttonOpenVillages_clicked(); break;
        case 1: _t->on_buttonOpenRoads_clicked(); break;
        case 2: _t->on_buttonOpenHouseh_clicked(); break;
        case 3: _t->on_buttonPlot_clicked(); break;
        case 4: _t->on_checkBoxVillageNames_clicked(); break;
        case 5: _t->on_checkBoxShowRoads_clicked(); break;
        case 6: _t->on_horizontalScrollBar_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->on_verticalScrollBar_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->xAxisChanged((*reinterpret_cast< QCPRange(*)>(_a[1]))); break;
        case 9: _t->yAxisChanged((*reinterpret_cast< QCPRange(*)>(_a[1]))); break;
        case 10: _t->on_pushButtonInitialSolution_clicked(); break;
        //case 11: _t->on_pushButtonShowRoute_pressed(); break;
        case 12: _t->on_checkBoxVillageIDs_clicked(); break;
        case 13: _t->weekSelected((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 14: _t->daySelected((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 15: _t->on_pushButtonShowRoute_clicked(); break;
        case 16: _t->showReportWindow(); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, 0, 0}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_END_MOC_NAMESPACE
