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
#include "platformagnosticmenu.hpp"

#include <QMenu>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQuickWindow>
#include <QQuickItem>
#include <QQmlListReference>
#include <QWidgetAction>
#include <QQmlInfo>

#define QQUICKCONTROLS2_MENU_PATH "qrc:///widgets/MenuExt.qml"
#define QQUICKCONTROLS2_MENU_SEPARATOR_PATH "qrc:///widgets/MenuSeparatorExt.qml"


PlatformAgnosticMenu::PlatformAgnosticMenu(QObject * parent)
    : QObject{parent}
{

}

void PlatformAgnosticMenu::installEventFilter(QObject *object)
{
    assert(menu());
    menu()->installEventFilter(object);
}

void PlatformAgnosticMenu::removeEventFilter(QObject *object)
{
    assert(menu());
    menu()->installEventFilter(object);
}

template<>
PlatformAgnosticMenu* PlatformAgnosticMenu::fromMenu(QMenu * menu)
{
    assert(menu);

    PlatformAgnosticMenu* const widgetsMenu = new WidgetsMenu;

    widgetsMenu->setMenu(menu);

    connect(menu, &QMenu::aboutToHide, widgetsMenu, &PlatformAgnosticMenu::aboutToHide);
    connect(menu, &QMenu::aboutToShow, widgetsMenu, &PlatformAgnosticMenu::aboutToShow);

    menu->setProperty("platformAgnosticMenu", QVariant::fromValue(widgetsMenu));

    return widgetsMenu;
}

template<>
PlatformAgnosticMenu* PlatformAgnosticMenu::fromMenu(PlatformAgnosticMenu * PlatformagnosticMenu)
{
    assert(PlatformagnosticMenu);
    return PlatformagnosticMenu;
}

PlatformAgnosticMenu* PlatformAgnosticMenu::createMenu(QObject* parent)
{
    if (!parent)
    {
        return new WidgetsMenu;
    }
    else
    {
        if (const auto widgetsMenuParent = qobject_cast<WidgetsMenu*>(parent))
            return new WidgetsMenu(widgetsMenuParent);
        else if (const auto quickControls2MenuParent = qobject_cast<QuickControls2Menu*>(parent))
            return new QuickControls2Menu(quickControls2MenuParent);
        else if (const auto quickWindowParent = qobject_cast<QQuickWindow*>(parent))
            return new QuickControls2Menu(quickWindowParent);
        else if (const auto quickItemParent = qobject_cast<QQuickItem*>(parent))
            return new QuickControls2Menu(quickItemParent);
        else
            Q_UNREACHABLE();
    }
}

PlatformAgnosticMenu* PlatformAgnosticMenu::createMenu(const QString& text, QObject *parent)
{
    PlatformAgnosticMenu* const menu = createMenu(parent);
    menu->setTitle(text);
    return menu;
}

PlatformAgnosticMenu* PlatformAgnosticMenu::addMenu(const QString &title)
{
    PlatformAgnosticMenu* const menu = createMenu(this);
    menu->setTitle(title);
    addMenu(menu);
    return menu;
}

void PlatformAgnosticMenu::clear()
{
    const auto actionList = actions();

    for (const auto action : actionList)
    {
        removeAction(action);
        if (action->parent() == this)
            delete action;
    }
}

bool PlatformAgnosticMenu::isEmpty() const
{
    bool empty = true;
    const auto actionList = actions();
    for (const auto action : actionList)
    {
        if (action->isVisible())
            empty = false;
    }
    return empty;
}

void PlatformAgnosticMenu::setTitle(const QString &title)
{
    assert(menu());

    // Both QQuickMenu and QMenu has the 'title' property
    // Which makes this method very convenient to implement
    menu()->setProperty("title", title);
}

void PlatformAgnosticMenu::setEnabled(const bool enabled)
{
    assert(menu());
    menu()->setProperty("enabled", enabled);
}

PlatformAgnosticAction* PlatformAgnosticMenu::addAction(const QString& iconSource, const QString& text)
{
    const auto action = addAction(text);
    action->setIcon(iconSource);
    return action;
}

PlatformAgnosticAction* PlatformAgnosticMenu::addAction(const QString& text)
{
    const auto action = PlatformAgnosticAction::createAction(this);
    action->setText(text);
    addAction(action);
    return action;
}

WidgetsMenu::WidgetsMenu(WidgetsMenu* parent)
    : PlatformAgnosticMenu{parent}
{
    m_menu = new QMenu(parent ? parent->m_menu : nullptr);
    connect(m_menu.data(), &QMenu::aboutToHide, this, &PlatformAgnosticMenu::aboutToHide);
    connect(m_menu.data(), &QMenu::aboutToShow, this, &PlatformAgnosticMenu::aboutToShow);

    //static_cast<QObject*>(m_menu)->setParent(this); // Qt bug

    m_menu->setProperty("platformAgnosticMenu", QVariant::fromValue(this));
}

WidgetsMenu::~WidgetsMenu()
{
    // Since we can not set QObject::parent on m_menu
    // we should delete it here
    assert(m_menu);
    m_menu->deleteLater();
}

void WidgetsMenu::insertAction(PlatformAgnosticAction *before, PlatformAgnosticAction *action)
{
    assert(m_menu);
    assert(qobject_cast<WidgetsAction*>(action));

    if (before)
    {
        assert(qobject_cast<WidgetsAction*>(before));
        m_menu->insertAction(static_cast<WidgetsAction*>(before)->m_action, static_cast<WidgetsAction*>(action)->m_action);
    }
    else
    {
        addAction(action);
    }
}

void WidgetsMenu::clear()
{
    assert(m_menu);
    m_menu->clear();
}

void WidgetsMenu::addAction(PlatformAgnosticAction *action)
{
    assert(qobject_cast<WidgetsAction*>(action));
    assert(m_menu);

    m_menu->addAction(static_cast<WidgetsAction*>(action)->m_action);
}

void WidgetsMenu::removeAction(PlatformAgnosticAction *action)
{
    assert(qobject_cast<WidgetsAction*>(action));
    assert(m_menu);

    m_menu->removeAction(static_cast<WidgetsAction*>(action)->m_action);
}

void WidgetsMenu::addMenu(PlatformAgnosticMenu *menu)
{
    assert(qobject_cast<WidgetsMenu*>(menu));
    assert(m_menu);

    m_menu->addMenu(static_cast<WidgetsMenu*>(menu)->m_menu);
}

void WidgetsMenu::popup(const QPoint &pos)
{
    assert(m_menu);
    m_menu->popup(pos);
}

QList<PlatformAgnosticAction *> WidgetsMenu::actions() const
{
    assert(m_menu);

    QList<PlatformAgnosticAction *> list;

    const auto actions = static_cast<QMenu*>(m_menu.data())->actions();

    for (const auto i : actions)
    {
        if (const auto PlatformagnosticAction = i->property("platformAgnosticAction").value<PlatformAgnosticAction*>())
            list.push_back(PlatformagnosticAction);
    }

    return list;
}

void WidgetsMenu::close()
{
    assert(m_menu);

    m_menu->close();
}

void WidgetsMenu::addSeparator()
{
    assert(m_menu);
    m_menu->addSeparator();
}

QSize WidgetsMenu::sizeHint() const
{
    assert(m_menu);
    return m_menu->sizeHint();
}

void WidgetsMenu::setTearOffEnabled(bool enabled)
{
    assert(m_menu);
    m_menu->setTearOffEnabled(enabled);
}

void WidgetsMenu::addItem(QObject *item)
{
    assert(m_menu);
    assert(qobject_cast<QWidgetAction*>(item));

    m_menu->addAction(static_cast<QWidgetAction*>(item));
}

void WidgetsMenu::removeItem(QObject *item)
{
    assert(m_menu);
    assert(qobject_cast<QWidgetAction*>(item));

    m_menu->removeAction(static_cast<QWidgetAction*>(item));
}

QObject* WidgetsMenu::menu() const
{
    return m_menu.data();
}

void WidgetsMenu::setMenu(QObject *menu)
{
    assert(menu);
    assert(qobject_cast<QMenu*>(menu));
    m_menu = static_cast<QMenu*>(menu);
}

QuickControls2Menu::QuickControls2Menu(QObject *quickParent, QObject* parent)
    : PlatformAgnosticMenu{parent}
{
    assert(quickParent);

    // In normal cases, quickParent should have a valid qml engine associated to its context:
    QQmlEngine* engine = qmlEngine(quickParent);

    if (!engine)
    {
        // Sometimes QQuickRootItem does not have an engine associated to its context
        // In these cases, the engine is probed using its first QQuickItem child.
        if (const auto itemParent = qobject_cast<QQuickItem*>(quickParent))
        {
            const auto children = QQmlListReference(quickParent, "children");
            if (children.count() > 0)
                engine = qmlEngine(children.at(0));
        }
    }

    assert(engine);

    QQmlComponent menuComponent{engine, QUrl(QStringLiteral(QQUICKCONTROLS2_MENU_PATH))};
    m_menuSeparatorComponent = new QQmlComponent(
        engine,
        QUrl(QStringLiteral(QQUICKCONTROLS2_MENU_SEPARATOR_PATH)),
        this);
    m_menu = menuComponent.create(qmlContext(quickParent));
    assert(m_menu);
    assert(m_menu->inherits("QQuickMenu"));

    QQmlEngine::setObjectOwnership(m_menu, QQmlEngine::CppOwnership);

    m_menu->setParent(this);

    connect(m_menu, SIGNAL(aboutToShow()), this, SIGNAL(aboutToShow()));
    connect(m_menu, SIGNAL(aboutToHide()), this, SIGNAL(aboutToHide()));

    if (const auto itemParent = qobject_cast<QQuickItem*>(quickParent))
        m_menu->setProperty("parent", QVariant::fromValue(itemParent));

    m_menu->setProperty("platformAgnosticMenu", QVariant::fromValue(this));
}

QuickControls2Menu::QuickControls2Menu(QuickControls2Menu *parent)
    : QuickControls2Menu{parent->m_menu, parent}
{

}

QuickControls2Menu::QuickControls2Menu(QQuickWindow *parent)
    : QuickControls2Menu{parent->contentItem(), parent}
{

}

QuickControls2Menu::QuickControls2Menu(QQuickItem *parent)
    : QuickControls2Menu{parent, parent}
{

}

void QuickControls2Menu::installEventFilter(QObject *object)
{
    PlatformAgnosticMenu::installEventFilter(object);
    assert(m_menu);
    QQuickItem* const contentItem = m_menu->property("contentItem").value<QQuickItem*>();
    assert(contentItem);
    contentItem->installEventFilter(object);
}

void QuickControls2Menu::removeEventFilter(QObject *object)
{
    PlatformAgnosticMenu::removeEventFilter(object);
    assert(m_menu);
    QQuickItem* const contentItem = m_menu->property("contentItem").value<QQuickItem*>();
    assert(contentItem);
    contentItem->removeEventFilter(object);
}

void QuickControls2Menu::insertAction(PlatformAgnosticAction *before, PlatformAgnosticAction *action)
{
    if (!before)
    {
        addAction(action);
        return;
    }

    assert(m_menu);
    assert(action);
    assert(qobject_cast<QuickControls2Action*>(action));

    // Akin to QWidgets::insertAction()

    const auto& actionList = actions();

    if (actionList.contains(action))
        removeAction(action);

    const int pos = actionList.indexOf(before);
    if (pos >= 0)
    {
        QMetaObject::invokeMethod(m_menu.data(),
                                  "_insertAction",
                                  Q_ARG(QVariant, QVariant{pos}),
                                  Q_ARG(QVariant, QVariant::fromValue(static_cast<QuickControls2Action*>(action)->m_action)));
    }
    else
    {
        addAction(action);
    }
}

void QuickControls2Menu::addAction(PlatformAgnosticAction *action)
{
    assert(action);
    assert(m_menu);
    assert(qobject_cast<QuickControls2Action*>(action));

    QMetaObject::invokeMethod(m_menu.data(),
                              "_addAction",
                              Q_ARG(QVariant,
                                    QVariant::fromValue(static_cast<QuickControls2Action*>(action)->m_action.data())));
}

void QuickControls2Menu::removeAction(PlatformAgnosticAction *action)
{
    assert(action);
    assert(m_menu);
    assert(qobject_cast<QuickControls2Action*>(action));

    QMetaObject::invokeMethod(m_menu.data(),
                              "_removeAction",
                              Q_ARG(QVariant,
                                    QVariant::fromValue(static_cast<QuickControls2Action*>(action)->m_action.data())));
}

void QuickControls2Menu::addMenu(PlatformAgnosticMenu *menu)
{
    assert(m_menu);
    assert(qobject_cast<QuickControls2Menu*>(menu));

    QMetaObject::invokeMethod(m_menu.data(),
                              "_addMenu",
                              Q_ARG(QVariant,
                                    QVariant::fromValue(static_cast<QuickControls2Menu*>(menu)->m_menu.data())));
}

void QuickControls2Menu::popup(const QPoint &pos)
{
    assert(m_menu);

    QPoint _pos = pos;

    if (const auto parentItem = m_menu->property("parent").value<QQuickItem*>())
    {
        _pos = parentItem->mapFromGlobal(pos).toPoint();
    }

    m_menu->setProperty("x", _pos.x());
    m_menu->setProperty("y", _pos.y());

    QMetaObject::invokeMethod(m_menu, "open");
}

QList<PlatformAgnosticAction *> QuickControls2Menu::actions() const
{
    QList<PlatformAgnosticAction*> list;

    if (!m_menu)
        return list;

    assert(m_menu->inherits("QQuickMenu"));

    const auto contentData = QQmlListReference(m_menu.data(), "contentData");

    for (auto i = 0; i < contentData.count(); ++i)
    {
        const auto action = contentData.at(i)->property("action").value<QObject*>();
        if (action)
        {
            assert(action->inherits("QQuickAction"));

            if (const auto platformAgnosticAction = action->property("platformAgnosticAction").value<PlatformAgnosticAction*>())
                list.push_back(platformAgnosticAction);
        }
    }

    return list;
}

void QuickControls2Menu::close()
{
    assert(m_menu);
    QMetaObject::invokeMethod(m_menu, "close");
}

void QuickControls2Menu::addSeparator()
{
    assert(m_menu);
    assert(m_menuSeparatorComponent);

    const auto separator = qobject_cast<QQuickItem*>(m_menuSeparatorComponent->create(qmlContext(m_menu)));
    assert(separator);
    QQmlEngine::setObjectOwnership(separator, QQmlEngine::CppOwnership);

    assert(separator->inherits("QQuickMenuSeparator"));
    separator->setParent(m_menu);

    addItem(separator);
}

QSize QuickControls2Menu::sizeHint() const
{
    assert(m_menu);
    // We have to polish the item view otherwise implicit size is reported incorrectly.
    // As an optimization, Qt does not calculate the item view content size until
    // it becomes necessary.
    if (const auto contentItem = m_menu->property("contentItem").value<QQuickItem*>())
    {
#if QT_VERSION >= QT_VERSION_CHECK(6, 3, 0)
        // Qt 6.3.0 has ensurePolished(), which calls updatePolish()
        contentItem->ensurePolished();
#else
        // Before Qt 6.3.0, we can not use updatePolish() because it is protected.
        // There are several alternatives, such as calling polish() and waiting
        // until the window signals QQuickWindow::beforeSynchronizing.
        // Another alternative is calling componentComplete(), which in turn
        // calls updateViewport() that calculates the content size. Here,
        // the invokable QQuickItemView::forceLayout() is used.
        assert(contentItem->inherits("QQuickItemView"));
        QMetaObject::invokeMethod(contentItem, "forceLayout");
#endif
    }

    return QSizeF{m_menu->property("implicitWidth").value<qreal>(),
                  m_menu->property("implicitHeight").value<qreal>()}.toSize();
}

void QuickControls2Menu::setTearOffEnabled(bool enabled)
{
    // Stub
    Q_UNUSED(enabled);
    qmlDebug(m_menu.data()) << "Tear off is not supported by QuickControls2Menu!";
}

void QuickControls2Menu::addItem(QObject *item)
{
    assert(m_menu);
    assert(qobject_cast<QQuickItem*>(item));

    QMetaObject::invokeMethod(m_menu, "addItem", Q_ARG(QQuickItem*, static_cast<QQuickItem*>(item)));
}

void QuickControls2Menu::removeItem(QObject* item)
{
    assert(m_menu);
    assert(qobject_cast<QQuickItem*>(item));

    QMetaObject::invokeMethod(m_menu, "removeItem", Q_ARG(QQuickItem*, static_cast<QQuickItem*>(item)));
}

QObject* QuickControls2Menu::menu() const
{
    return m_menu.data();
}

void QuickControls2Menu::setMenu(QObject *menu)
{
    assert(menu);
    assert(menu->inherits("QQuickMenu"));
    m_menu = menu;
}
