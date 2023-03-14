/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
#include "mouse_event_filter.hpp"

#include <QMouseEvent>
#include <QQuickItem>

MouseEventFilter::MouseEventFilter(QObject *parent)
    : QObject(parent)
{

}

MouseEventFilter::~MouseEventFilter()
{
    detach();
}

QObject *MouseEventFilter::target() const
{
    return m_target;
}

void MouseEventFilter::setTarget(QObject *newTarget)
{
    if (m_target == newTarget)
        return;

    detach();
    m_target = newTarget;
    attach();

    emit targetChanged();
}

bool MouseEventFilter::eventFilter(QObject *watched, QEvent *event)
{
    assert(watched == m_target);

    const auto mouse = dynamic_cast<QMouseEvent*>(event);
    if (!mouse)
        return false;

    if (!m_filterEventsSynthesizedByQt &&
        mouse->source() == Qt::MouseEventSource::MouseEventSynthesizedByQt)
        return false;

    switch (event->type())
    {
    case QEvent::MouseButtonDblClick:
        emit mouseButtonDblClick(mouse->localPos(),
                                 mouse->globalPos(),
                                 mouse->buttons(),
                                 mouse->modifiers(),
                                 mouse->source(),
                                 mouse->flags()); break;
    case QEvent::MouseButtonPress:
        emit mouseButtonPress(mouse->localPos(),
                              mouse->globalPos(),
                              mouse->buttons(),
                              mouse->modifiers(),
                              mouse->source(),
                              mouse->flags()); break;
    case QEvent::MouseButtonRelease:
        emit mouseButtonRelease(mouse->localPos(),
                                mouse->globalPos(),
                                mouse->button(),
                                mouse->modifiers(),
                                mouse->source(),
                                mouse->flags()); break;
    case QEvent::MouseMove:
        emit mouseMove(mouse->localPos(),
                       mouse->globalPos(),
                       mouse->buttons(),
                       mouse->modifiers(),
                       mouse->source(),
                       mouse->flags()); break;

    default:
        return false;
    }

    return true;
}

void MouseEventFilter::attach()
{
    if (m_target)
    {
        m_target->installEventFilter(this);
        const auto item = qobject_cast<QQuickItem*>(m_target);
        if (item)
        {
            m_targetItemInitialAcceptedMouseButtons = item->acceptedMouseButtons();
            item->setAcceptedMouseButtons(Qt::AllButtons);
        }
    }
}

void MouseEventFilter::detach()
{
    if (m_target)
    {
        m_target->removeEventFilter(this);
        const auto item = qobject_cast<QQuickItem*>(m_target);
        if (item)
            item->setAcceptedMouseButtons(m_targetItemInitialAcceptedMouseButtons);
    }
}
