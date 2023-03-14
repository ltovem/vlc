/*****************************************************************************
 * Copyright (C) 2022 the VideoLAN team
 *
 * Authors: Prince Gupta <guptaprince8832@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef CSDBUTTONCONTROLLER_HPP
#define CSDBUTTONCONTROLLER_HPP

#include <QObject>
#include <QRect>
#include <memory>

class CSDButton : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ButtonType type READ type CONSTANT)
    Q_PROPERTY(bool showHovered READ showHovered WRITE setShowHovered NOTIFY showHoveredChanged)
    Q_PROPERTY(QRect rect READ rect WRITE setRect NOTIFY rectChanged)

public:
    enum ButtonType
    {
        Minimize,
        MaximizeRestore,
        Close,
        SystemMenu,

        TypeCount
    };

    Q_ENUM(ButtonType);

    CSDButton(ButtonType type, QObject *parent);

    ButtonType type() const;

    // 'showHovered' is hint for UI to the show this
    // button as in 'hovered' state
    // used by implmentation incase custom event handling is required for CSD
    bool showHovered() const;
    void setShowHovered(bool newShowHovered);

    // 'rect' is location of the button in the UI
    // may be used by implementation to relay the information
    // to OS such as to show snaplay out menu on Windows 11
    const QRect &rect() const;
    void setRect(const QRect &newRect);

public slots:
    // signals to perfrom action associated with button
    // actions are dependent on implmentation
    void click();
    void doubleClick();

signals:
    void showHoveredChanged();
    void rectChanged();
    void clicked();
    void doubleClicked();

private:
    const ButtonType m_type;
    bool m_showHovered = false;
    QRect m_rect;
};


class SystemMenuButton : public CSDButton
{
    Q_OBJECT

public:
    SystemMenuButton(QObject *parent = nullptr) : CSDButton {SystemMenuButton::SystemMenu, parent} {}

    virtual void showSystemMenu() = 0;

signals:
    void systemMenuVisibilityChanged(bool visible);
};


class MainCtx;


class CSDButtonModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<CSDButton *> windowCSDButtons READ windowCSDButtons CONSTANT)

    Q_PROPERTY(CSDButton *systemMenuButton READ systemMenuButton CONSTANT)

public:
    CSDButtonModel(MainCtx *mainCtx, QObject *parent = nullptr);

    QList<CSDButton *> windowCSDButtons() const;
    CSDButton *systemMenuButton() const;

    // set by internal implmentation
    // all the actions are also handled by implementation
    void setSystemMenuButton(std::shared_ptr<SystemMenuButton> button);

private slots:
    void minimizeButtonClicked();
    void maximizeRestoreButtonClicked();
    void closeButtonClicked();

private:
    MainCtx *m_mainCtx;

    // CSD window action buttons i.e minimize, maximize, close
    QList<CSDButton *> m_windowCSDButtons;

    // sysmenu button, available on windows only
    std::shared_ptr<SystemMenuButton> m_systemMenuButton = nullptr;
};


Q_DECLARE_METATYPE(CSDButton *)

#endif // CSDBUTTONCONTROLLER_HPP
