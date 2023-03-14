/*****************************************************************************
 * Copyright (C) 2023 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
#include "hover_handler_rev11.hpp"

HoverHandlerRev11::HoverHandlerRev11(QObject *parent)
    : QObject(parent)
{
}

HoverHandlerRev11::~HoverHandlerRev11()
{
    setTarget(nullptr);
}

void HoverHandlerRev11::setTarget(QQuickItem* target)
{
    if (m_target)
        m_target->removeEventFilter(this);

    m_target = target;

    if (m_target)
    {
        m_target->setAcceptHoverEvents(true);
        m_target->installEventFilter(this);
    }
}

bool HoverHandlerRev11::eventFilter(QObject*, QEvent* event)
{
    bool changed = true;

    switch (event->type())
    {
    case QEvent::HoverEnter:
        m_hovered = true;
        break;
    case QEvent::HoverLeave:
        m_hovered = false;
        break;
    default:
        changed = false;
        break;
    }

    if (changed)
        emit hoveredChanged();

    return changed;
}

void HoverHandlerRev11::classBegin()
{
}

void HoverHandlerRev11::componentComplete()
{
    if (!m_target)
    {
        auto parentItem = qobject_cast<QQuickItem*>(QObject::parent());
        if (parentItem)
            setTarget(parentItem);
    }
}
