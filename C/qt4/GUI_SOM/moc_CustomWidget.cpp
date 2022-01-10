/****************************************************************************
** Meta object code from reading C++ file 'CustomWidget.hpp'
**
** Created: Sat Oct 29 21:54:36 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "CustomWidget.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CustomWidget.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CustomWidget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x09,
      31,   13,   13,   13, 0x09,
      48,   13,   13,   13, 0x09,
      69,   13,   13,   13, 0x09,
      86,   13,   13,   13, 0x09,
     102,   13,   13,   13, 0x09,
     118,   13,   13,   13, 0x09,
     138,   13,   13,   13, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_CustomWidget[] = {
    "CustomWidget\0\0doCommandStart()\0"
    "doCommandClear()\0doCommandSeqential()\0"
    "doCommandBatch()\0doCommandNorm()\0"
    "doCommandProd()\0doCommandWSpin(int)\0"
    "doCommandHSpin(int)\0"
};

const QMetaObject CustomWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CustomWidget,
      qt_meta_data_CustomWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CustomWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CustomWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CustomWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CustomWidget))
        return static_cast<void*>(const_cast< CustomWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int CustomWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: doCommandStart(); break;
        case 1: doCommandClear(); break;
        case 2: doCommandSeqential(); break;
        case 3: doCommandBatch(); break;
        case 4: doCommandNorm(); break;
        case 5: doCommandProd(); break;
        case 6: doCommandWSpin((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: doCommandHSpin((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
