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
#ifndef PLATFORMAGNOSTICACTIONGROUP_HPP
#define PLATFORMAGNOSTICACTIONGROUP_HPP

#include <QObject>
#include <QPointer>

class PlatformAgnosticAction;

// Common denominator for QActionGroup and QQuickActionGroup
class PlatformAgnosticActionGroup : public QObject
{
    Q_OBJECT

    friend class PlatformAgnosticAction;

public:
    explicit PlatformAgnosticActionGroup(QObject * parent = nullptr);
    virtual ~PlatformAgnosticActionGroup() = default;

    template<class ActionGroup>
    static PlatformAgnosticActionGroup* fromActionGroup(ActionGroup actionGroup);

    static PlatformAgnosticActionGroup* createActionGroup(QObject *parent = nullptr);

    virtual void addAction(PlatformAgnosticAction *action) = 0;
    virtual void removeAction(PlatformAgnosticAction *action) = 0;

public slots:
    virtual void setEnabled(bool enabled);
    virtual void setExclusive(bool exclusive);

signals:
    void triggered(QObject *action);

protected:
    QObject* operator()() const { return actionGroup(); };
    virtual QObject *actionGroup() const = 0;
    virtual void setActionGroup(QObject* actionGroup) = 0;
};

class WidgetsActionGroup : public PlatformAgnosticActionGroup
{
    Q_OBJECT

    friend class WidgetsAction;

public:
    explicit WidgetsActionGroup(QObject* parent = nullptr);
    virtual ~WidgetsActionGroup() = default;

    void addAction(PlatformAgnosticAction *action) override;
    void removeAction(PlatformAgnosticAction *action) override;

protected:
    QObject* actionGroup() const override;
    void setActionGroup(QObject* actionGroup) override;

private:
    QPointer<class QActionGroup> m_actionGroup;
};

class QuickControls2ActionGroup : public PlatformAgnosticActionGroup
{
    Q_OBJECT

    friend class QuickControls2Action;

public:
    QuickControls2ActionGroup(QObject* quickParent, class QObject* parent = nullptr);

    explicit QuickControls2ActionGroup(QObject* parent);
    virtual ~QuickControls2ActionGroup() = default;

    void addAction(PlatformAgnosticAction *action) override;
    void removeAction(PlatformAgnosticAction *action) override;

protected:
    QObject* actionGroup() const override;
    void setActionGroup(QObject* actionGroup) override;

private:
    QPointer<QObject> m_actionGroup;
};

#endif // PLATFORMAGNOSTICACTIONGROUP_HPP
