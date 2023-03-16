// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *****************************************************************************/
#ifndef KEYNAVIGABLE_H
#define KEYNAVIGABLE_H

#include <QObject>
#include <QQuickItem>

#define VLC_QT_SIMPLE_PROPERTY(Type, property, defaultValue) \
  Q_PROPERTY(Type property READ get##property WRITE set##property NOTIFY property##Changed FINAL) \
public: \
  inline Type get##property() const { return m_##property; } \
  inline void set##property(Type val ) { m_##property = val; emit  property##Changed(); } \
private: \
  Type m_##property = defaultValue;

class NavigationAttached : public QObject
{
    Q_OBJECT

    VLC_QT_SIMPLE_PROPERTY(bool, navigable, true)

    VLC_QT_SIMPLE_PROPERTY(QQuickItem*, parentItem, nullptr)

    VLC_QT_SIMPLE_PROPERTY(QQuickItem*, upItem, nullptr)
    VLC_QT_SIMPLE_PROPERTY(QJSValue, upAction, QJSValue::UndefinedValue)

    VLC_QT_SIMPLE_PROPERTY(QQuickItem*, downItem, nullptr)
    VLC_QT_SIMPLE_PROPERTY(QJSValue, downAction, QJSValue::UndefinedValue)

    VLC_QT_SIMPLE_PROPERTY(QQuickItem*, leftItem, nullptr)
    VLC_QT_SIMPLE_PROPERTY(QJSValue, leftAction, QJSValue::UndefinedValue)

    VLC_QT_SIMPLE_PROPERTY(QQuickItem*, rightItem, nullptr)
    VLC_QT_SIMPLE_PROPERTY(QJSValue, rightAction, QJSValue::UndefinedValue)

    VLC_QT_SIMPLE_PROPERTY(QQuickItem*, cancelItem, nullptr)
    VLC_QT_SIMPLE_PROPERTY(QJSValue, cancelAction, QJSValue::UndefinedValue)

#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
    QML_NAMED_ELEMENT(Navigation)
    QML_UNCREATABLE("Navigation is only available via attached properties.")
    QML_ATTACHED(NavigationAttached)
#endif
public:
    explicit NavigationAttached(QObject *parent = nullptr);

    Q_INVOKABLE void defaultKeyAction(QObject* event);

    Q_INVOKABLE void defaultKeyReleaseAction(QObject* event);

    static NavigationAttached *qmlAttachedProperties(QObject* parent)
    {
        return new NavigationAttached(parent);
    }

    Q_INVOKABLE void defaultNavigationUp();
    Q_INVOKABLE void defaultNavigationDown();
    Q_INVOKABLE void defaultNavigationLeft();
    Q_INVOKABLE void defaultNavigationRight();
    Q_INVOKABLE void defaultNavigationCancel();

private:
    void defaultNavigationGeneric(QJSValue& jsCallback, QQuickItem* directionItem,
                                  void (NavigationAttached::* defaultNavFn)(void),
                                  Qt::FocusReason reason);

signals:
    void navigableChanged();
    void parentItemChanged();
    void upItemChanged();
    void upActionChanged();
    void downItemChanged();
    void downActionChanged();
    void leftItemChanged();
    void leftActionChanged();
    void rightItemChanged();
    void rightActionChanged();
    void cancelItemChanged();
    void cancelActionChanged();
};

#undef VLC_QT_SIMPLE_PROPERTY

QML_DECLARE_TYPEINFO(NavigationAttached, QML_HAS_ATTACHED_PROPERTIES)

#endif // KEYNAVIGABLE_H
