/****************************************************************************
** Meta object code from reading C++ file 'card_text.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../Carousel_card/card_text.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'card_text.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.2. It"
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
struct qt_meta_tag_ZN9Card_textE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN9Card_textE = QtMocHelpers::stringData(
    "Card_text",
    "shrink_width",
    "enlarge_width",
    "font_size",
    "font_size2",
    "top_margin"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN9Card_textE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       5,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags, notifyId, revision
       1, QMetaType::Int, 0x00015903, uint(-1), 0,
       2, QMetaType::Int, 0x00015903, uint(-1), 0,
       3, QMetaType::Int, 0x00015903, uint(-1), 0,
       4, QMetaType::Int, 0x00015903, uint(-1), 0,
       5, QMetaType::Int, 0x00015903, uint(-1), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject Card_text::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ZN9Card_textE.offsetsAndSizes,
    qt_meta_data_ZN9Card_textE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN9Card_textE_t,
        // property 'shrink_width'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // property 'enlarge_width'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // property 'font_size'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // property 'font_size2'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // property 'top_margin'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<Card_text, std::true_type>
    >,
    nullptr
} };

void Card_text::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Card_text *>(_o);
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->shrink_width(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->enlarge_width(); break;
        case 2: *reinterpret_cast< int*>(_v) = _t->font_size(); break;
        case 3: *reinterpret_cast< int*>(_v) = _t->font_size2(); break;
        case 4: *reinterpret_cast< int*>(_v) = _t->top_margin(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setShrink_width(*reinterpret_cast< int*>(_v)); break;
        case 1: _t->setEnlarge_width(*reinterpret_cast< int*>(_v)); break;
        case 2: _t->setFont_size(*reinterpret_cast< int*>(_v)); break;
        case 3: _t->setFont_size2(*reinterpret_cast< int*>(_v)); break;
        case 4: _t->setTop_margin(*reinterpret_cast< int*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *Card_text::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Card_text::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN9Card_textE.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Card_text::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
