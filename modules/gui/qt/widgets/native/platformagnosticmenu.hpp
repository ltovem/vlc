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
#ifndef PLATFORMAGNOSTICMENU_HPP
#define PLATFORMAGNOSTICMENU_HPP

#include <QObject>
#include <QPointer>
#include <QList>
#include <QKeySequence>

#include "platformagnosticaction.hpp"

// Common denominator for QMenu and QQuickMenu
class PlatformAgnosticMenu : public QObject
{
    Q_OBJECT

    friend class PlatformAgnosticAction;
    friend class PlatformAgnosticActionGroup;

public:
    explicit PlatformAgnosticMenu(QObject *parent);
    virtual ~PlatformAgnosticMenu() = default;

    virtual void installEventFilter(QObject* object);
    virtual void removeEventFilter(QObject* object);

    template<class Menu>
    static PlatformAgnosticMenu* fromMenu(Menu menu);

    static PlatformAgnosticMenu* createMenu(QObject * parent = nullptr);
    static PlatformAgnosticMenu* createMenu(const QString& text, QObject * parent);

    virtual PlatformAgnosticMenu *addMenu(const QString &title);
    virtual void addMenu(PlatformAgnosticMenu *menu) = 0;

    virtual void setTearOffEnabled(bool enabled) = 0;
    virtual void clear();
    virtual bool isEmpty() const;
    virtual void setTitle(const QString& title);
    virtual void setEnabled(bool enabled);
    virtual void popup(const QPoint& pos) = 0;
    virtual void close() = 0;
    virtual void addSeparator() = 0;
    virtual void addItem(QObject* item) = 0;
    virtual void removeItem(QObject* item) = 0;

    virtual void insertAction(PlatformAgnosticAction *before, PlatformAgnosticAction *action) = 0;
    virtual void addAction(PlatformAgnosticAction *action) = 0;
    virtual PlatformAgnosticAction* addAction(const QString& text);
    virtual PlatformAgnosticAction* addAction(const QString& iconSource, const QString& text);
    virtual void removeAction(PlatformAgnosticAction *action) = 0;

    virtual QList<PlatformAgnosticAction*> actions() const = 0;

    virtual QSize sizeHint() const = 0;

    template<typename Functor>
    PlatformAgnosticAction* addAction(const QString& text, Functor func, const QKeySequence &shortcut = 0)
    {
        const auto action = addAction(text);
        action->setShortcut(shortcut);
        connect(action, &PlatformAgnosticAction::triggered, func);
        return action;
    }

    template<class Object, typename Functor>
    PlatformAgnosticAction* addAction(const QString& text, Object context, Functor func, const QKeySequence &shortcut = 0)
    {
        const auto action = addAction(text);
        action->setShortcut(shortcut);
        connect(action, &PlatformAgnosticAction::triggered, context, func);
        return action;
    }

    template<typename Functor>
    PlatformAgnosticAction* addAction(const QString& iconSource, const QString& text, Functor func, const QKeySequence &shortcut = 0)
    {
        const auto action = addAction(text, func, shortcut);
        action->setIcon(iconSource);
        return action;
    }

    template<class Object, typename Functor>
    PlatformAgnosticAction* addAction(const QString& iconSource, const QString& text, Object context, Functor func, const QKeySequence &shortcut = 0)
    {
        const auto action = addAction(text, context, func, shortcut);
        action->setIcon(iconSource);
        return action;
    }

    template<typename Functor>
    PlatformAgnosticAction* addAction(const QString& iconSource, const QString& text)
    {
        const auto action = addAction(text);
        action->setIcon(iconSource);
        return action;
    }

signals:
    void aboutToShow();
    void aboutToHide();

protected:
    QObject* operator()() const { return menu(); };
    virtual QObject* menu() const = 0;
    virtual void setMenu(QObject* menu) = 0;
};

class WidgetsMenu : public PlatformAgnosticMenu
{
    Q_OBJECT

public:
    explicit WidgetsMenu(WidgetsMenu* parent = nullptr);
    virtual ~WidgetsMenu();

    void insertAction(PlatformAgnosticAction *before, PlatformAgnosticAction *action) override;

    void clear() override;

    void addAction(PlatformAgnosticAction *action) override;
    void removeAction(PlatformAgnosticAction *action) override;

    void addMenu(PlatformAgnosticMenu *menu) override;

    void popup(const QPoint& pos) override;

    QList<PlatformAgnosticAction*> actions() const override;

    void close() override;

    void addSeparator() override;

    QSize sizeHint() const override;

    void setTearOffEnabled(bool enabled) override;

    void addItem(QObject* item) override;
    void removeItem(QObject* item) override;

protected:
    QObject* menu() const override;
    void setMenu(QObject * menu) override;

private:
    QPointer<class QMenu> m_menu;
};

class QuickControls2Menu : public PlatformAgnosticMenu
{
    Q_OBJECT

    friend class RendererMenu;

public:
    QuickControls2Menu(QObject* quickParent, class QObject* parent = nullptr);
    virtual ~QuickControls2Menu() = default;

    explicit QuickControls2Menu(QuickControls2Menu *parent);
    explicit QuickControls2Menu(class QQuickWindow* parent);
    explicit QuickControls2Menu(class QQuickItem* parent);

    virtual void installEventFilter(QObject* object) override;
    virtual void removeEventFilter(QObject* object) override;

    void insertAction(PlatformAgnosticAction *before, PlatformAgnosticAction *action) override;

    void addAction(PlatformAgnosticAction *action) override;
    void removeAction(PlatformAgnosticAction *action) override;

    void addMenu(PlatformAgnosticMenu *menu) override;

    void popup(const QPoint& pos) override;

    QList<PlatformAgnosticAction*> actions() const override;

    void close() override;

    void addSeparator() override;

    QSize sizeHint() const override;

    void setTearOffEnabled(bool enabled) override;

    void addItem(QObject* item) override;
    void removeItem(QObject* item) override;

protected:
    QObject* menu() const override;
    void setMenu(QObject * menu) override;

private:
    QPointer<QObject> m_menu;

    QPointer<class QQmlComponent> m_menuSeparatorComponent;
};



#endif // PLATFORMAGNOSTICMENU_HPP
