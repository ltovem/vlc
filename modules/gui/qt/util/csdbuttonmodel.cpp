/*****************************************************************************
 * Copyright (C) 2022 the VideoLAN team
 *
 * Authors: Prince Gupta <guptaprince8832@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/


#include "csdbuttonmodel.hpp"

#include "maininterface/mainctx.hpp"

#include <QWindow>

CSDButton::CSDButton(ButtonType type, QObject *parent)
    : QObject {parent}
    , m_type {type}
{
}

bool CSDButton::showHovered() const
{
    return m_showHovered;
}

void CSDButton::setShowHovered(bool newShowHovered)
{
    if (m_showHovered == newShowHovered)
        return;
    m_showHovered = newShowHovered;
    emit showHoveredChanged();
}

CSDButton::ButtonType CSDButton::type() const
{
    return m_type;
}

const QRect &CSDButton::rect() const
{
    return m_rect;
}

void CSDButton::setRect(const QRect &newRect)
{
    if (m_rect == newRect)
        return;
    m_rect = newRect;
    emit rectChanged();
}

void CSDButton::click()
{
    emit clicked();
}

void CSDButton::doubleClick()
{
    emit doubleClicked();
}

CSDButtonModel::CSDButtonModel(MainCtx *mainCtx, QObject *parent)
    : QObject {parent}
    , m_mainCtx {mainCtx}
{
    auto newButton = [this](CSDButton::ButtonType type, void (CSDButtonModel::* onClick)())
    {
        auto button = new CSDButton(type, this);
        connect(button, &CSDButton::clicked, this, onClick);
        m_windowCSDButtons.append(button);
    };

    newButton(CSDButton::Minimize, &CSDButtonModel::minimizeButtonClicked);
    newButton(CSDButton::MaximizeRestore, &CSDButtonModel::maximizeRestoreButtonClicked);
    newButton(CSDButton::Close, &CSDButtonModel::closeButtonClicked);
}

QList<CSDButton *> CSDButtonModel::windowCSDButtons() const
{
    return m_windowCSDButtons;
}

CSDButton *CSDButtonModel::systemMenuButton() const
{
    return m_systemMenuButton.get();
}

void CSDButtonModel::setSystemMenuButton(std::shared_ptr<SystemMenuButton> button)
{
    m_systemMenuButton = std::move(button);
}


void CSDButtonModel::minimizeButtonClicked()
{
    emit m_mainCtx->requestInterfaceMinimized();
}

void CSDButtonModel::maximizeRestoreButtonClicked()
{
    const auto visibility = m_mainCtx->intfMainWindow()->visibility();
    if (visibility == QWindow::Maximized)
        emit m_mainCtx->requestInterfaceNormal();
    else
        emit m_mainCtx->requestInterfaceMaximized();
}

void CSDButtonModel::closeButtonClicked()
{
    m_mainCtx->intfMainWindow()->close();
}
