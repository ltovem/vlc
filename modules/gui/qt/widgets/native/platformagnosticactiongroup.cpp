/*****************************************************************************
 * Copyright (C) 2023 VLC authors and VideoLAN
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * ( at your option ) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/
#include "platformagnosticactiongroup.hpp"

#include <QActionGroup>
#include <QQuickItem>
#include <QQmlComponent>
#include <QQmlEngine>

#include "platformagnosticmenu.hpp"

#define QQUICKCONTROLS2_ACTION_GROUP_PATH "qrc:///util/ActionGroupExt.qml"

PlatformAgnosticActionGroup::PlatformAgnosticActionGroup(QObject *parent)
    : QObject{parent}
{

}

template<>
PlatformAgnosticActionGroup* PlatformAgnosticActionGroup::fromActionGroup(QActionGroup * actionGroup)
{
    assert(actionGroup);

    PlatformAgnosticActionGroup* const widgetsActionGroup = new WidgetsActionGroup;

    widgetsActionGroup->setActionGroup(actionGroup);

    connect(actionGroup, &QActionGroup::triggered, widgetsActionGroup, &PlatformAgnosticActionGroup::triggered);
    actionGroup->setProperty("platformAgnosticActionGroup", QVariant::fromValue(widgetsActionGroup));

    return widgetsActionGroup;
}

template<>
PlatformAgnosticActionGroup* PlatformAgnosticActionGroup::fromActionGroup(PlatformAgnosticActionGroup * actionGroup)
{
    assert(actionGroup);
    return actionGroup;
}

PlatformAgnosticActionGroup* PlatformAgnosticActionGroup::createActionGroup(QObject *parent)
{
    if (!parent)
    {
        return new WidgetsActionGroup;
    }
    else
    {
        if (const auto widgetsMenuParent = qobject_cast<WidgetsMenu*>(parent))
            return new WidgetsActionGroup(widgetsMenuParent);
        else if (const auto quickControls2MenuParent = qobject_cast<QuickControls2Menu*>(parent))
            return new QuickControls2ActionGroup((*quickControls2MenuParent)(), quickControls2MenuParent);
        else if (const auto quickItemParent = qobject_cast<QQuickItem*>(parent))
            return new QuickControls2ActionGroup(quickItemParent, quickItemParent);
        else
            Q_UNREACHABLE();
    }
}

void PlatformAgnosticActionGroup::setEnabled(const bool enabled)
{
    assert(actionGroup());
    actionGroup()->setProperty("enabled", enabled);
}

void PlatformAgnosticActionGroup::setExclusive(const bool exclusive)
{
    assert(actionGroup());
    actionGroup()->setProperty("exclusive", exclusive);
}

WidgetsActionGroup::WidgetsActionGroup(QObject *parent)
    : PlatformAgnosticActionGroup{parent}
{
    m_actionGroup = new QActionGroup(parent);
    connect(m_actionGroup.data(), &QActionGroup::triggered, this, &PlatformAgnosticActionGroup::triggered);

    m_actionGroup->setProperty("agnosticActionGroup", QVariant::fromValue(this));
}

void WidgetsActionGroup::addAction(PlatformAgnosticAction *action)
{
    assert(qobject_cast<WidgetsAction*>(action));
    assert(m_actionGroup);

    m_actionGroup.data()->addAction(static_cast<WidgetsAction*>(action)->m_action.data());
}

void WidgetsActionGroup::removeAction(PlatformAgnosticAction *action)
{
    assert(qobject_cast<WidgetsAction*>(action));
    assert(m_actionGroup);

    m_actionGroup.data()->removeAction(static_cast<WidgetsAction*>(action)->m_action.data());
}

QObject* WidgetsActionGroup::actionGroup() const
{
    return m_actionGroup.data();
}

void WidgetsActionGroup::setActionGroup(QObject *actionGroup)
{
    assert(actionGroup);
    assert(qobject_cast<QActionGroup*>(actionGroup));

    m_actionGroup = static_cast<QActionGroup*>(actionGroup);
}

QObject* QuickControls2ActionGroup::actionGroup() const
{
    return m_actionGroup.data();
}

void QuickControls2ActionGroup::setActionGroup(QObject *actionGroup)
{
    assert(actionGroup);
    assert(actionGroup->inherits("QQuickActionGroup"));

    m_actionGroup = actionGroup;
}

QuickControls2ActionGroup::QuickControls2ActionGroup(QObject *quickParent, QObject *parent)
    : PlatformAgnosticActionGroup{parent}
{
    assert(quickParent);

    const auto engine = qmlEngine(quickParent);
    assert(engine);

    QQmlComponent actionGroupComponent{engine, QUrl(QStringLiteral(QQUICKCONTROLS2_ACTION_GROUP_PATH))};

    m_actionGroup = actionGroupComponent.create(qmlContext(quickParent));
    assert(m_actionGroup);
    assert(m_actionGroup->inherits("QQuickActionGroup"));

    QQmlEngine::setObjectOwnership(m_actionGroup, QQmlEngine::CppOwnership);

    connect(m_actionGroup, SIGNAL(_triggered(QObject*)), this, SIGNAL(triggered(QObject*)));

    m_actionGroup->setParent(this);

    m_actionGroup->setProperty("platformAgnosticActionGroup", QVariant::fromValue(this));
}

QuickControls2ActionGroup::QuickControls2ActionGroup(QObject *parent)
    : QuickControls2ActionGroup{parent, parent}
{

}

void QuickControls2ActionGroup::addAction(PlatformAgnosticAction *action)
{
    assert(qobject_cast<QuickControls2Action*>(action));
    assert(m_actionGroup);

    QMetaObject::invokeMethod(m_actionGroup.data(),
                              "_addAction",
                              Q_ARG(QVariant,
                                    QVariant::fromValue(static_cast<QuickControls2Action*>(action)->m_action.data())));
}

void QuickControls2ActionGroup::removeAction(PlatformAgnosticAction *action)
{
    assert(qobject_cast<QuickControls2Action*>(action));
    assert(m_actionGroup);

    QMetaObject::invokeMethod(m_actionGroup.data(),
                              "_removeAction",
                              Q_ARG(QVariant,
                                    QVariant::fromValue(static_cast<QuickControls2Action*>(action)->m_action.data())));
}
