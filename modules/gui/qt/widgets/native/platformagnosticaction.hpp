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
#ifndef PLATFORMAGNOSTICACTION_HPP
#define PLATFORMAGNOSTICACTION_HPP

#include <QObject>
#include <QPointer>
#include <QVariant>

class PlatformAgnosticActionGroup;

// Common denominator for QAction and QQuickAction
class PlatformAgnosticAction : public QObject
{
    Q_OBJECT

    friend class PlatformAgnosticActionGroup;
    friend class PlatformAgnosticMenu;

public:
    // QAction::MenuRole
    enum MenuRole { NoRole = 0, TextHeuristicRole, ApplicationSpecificRole, AboutQtRole,
                    AboutRole, PreferencesRole, QuitRole };

    explicit PlatformAgnosticAction(QObject *parent);
    virtual ~PlatformAgnosticAction() = default;

    template<class Action>
    static PlatformAgnosticAction* fromAction(Action action);

    static PlatformAgnosticAction* createAction(QObject * parent = nullptr);
    static PlatformAgnosticAction* createAction(const QString& text, QObject * parent = nullptr);

    virtual void setVisible(const bool visible);
    virtual bool isVisible() const;
    virtual QString text() const;
    virtual void setText(const QString &text);
    virtual void setCheckable(bool checkable);
    virtual void setShortcut(const QKeySequence &shortcut) = 0;
    virtual void setActionGroup(PlatformAgnosticActionGroup * actionGroup) = 0;
    virtual void setIcon(const QString& iconSourceOrName, bool isSource = true) = 0;
    virtual void setData(const QVariant& data);
    virtual QVariant data() const;
    virtual void setMenuRole(MenuRole role) = 0;

public slots:
    virtual void setEnabled(bool enabled);
    virtual void setChecked(bool checked);

signals:
    void toggled(bool);
    void triggered(bool);

protected:
    QObject* operator()() const { return action(); };
    virtual QObject* action() const = 0;
    virtual void setAction(QObject* action) = 0;

    QVariant m_data;
    QString m_text;
};

class WidgetsAction : public PlatformAgnosticAction
{
    Q_OBJECT

    friend class WidgetsActionGroup;
    friend class WidgetsMenu;

public:
    WidgetsAction(QObject *parent = nullptr);
    virtual ~WidgetsAction() = default;

    void setShortcut(const QKeySequence &shortcut) override;
    void setActionGroup(PlatformAgnosticActionGroup* actionGroup) override;
    void setIcon(const QString& iconSourceOrName, bool isSource = true) override;
    void setMenuRole(MenuRole role) override;

protected:
    QObject* action() const override;
    void setAction(QObject* action) override;

private:
    QPointer<class QAction> m_action;
};

class QuickControls2Action : public PlatformAgnosticAction
{
    Q_OBJECT

    friend class QuickControls2ActionGroup;
    friend class QuickControls2Menu;

public:
    QuickControls2Action(QObject* quickParent, class QObject* parent = nullptr);

    explicit QuickControls2Action(QObject *parent);
    virtual ~QuickControls2Action() = default;

    void setShortcut(const QKeySequence &shortcut) override;
    void setActionGroup(PlatformAgnosticActionGroup* actionGroup) override;
    void setIcon(const QString& iconSourceOrName, bool isSource = true) override;
    void setMenuRole(MenuRole role) override;

protected:
    QObject* action() const override;
    void setAction(QObject* action) override;

private:
    QPointer<QObject> m_action;

private slots:
    void onTriggered(QObject* source);
    void onToggled(QObject* source);
};

#endif // PLATFORMAGNOSTICACTION_HPP
