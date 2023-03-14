/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
#include "item_key_event_filter.hpp"
#include <QEvent>

ItemKeyEventFilter::ItemKeyEventFilter(QQuickItem *parent)
    : QQuickItem(parent)
{
}

ItemKeyEventFilter::~ItemKeyEventFilter()
{
    if (m_target)
        m_target->removeEventFilter(this);
}

void ItemKeyEventFilter::setTarget(QObject* target)
{
    assert(target);

    target->installEventFilter(this);
    m_target = target;
}

void ItemKeyEventFilter::keyPressEvent(QKeyEvent* event)
{
    // This is actually called when the QML event handler hasn't accepted the event
    m_qmlAccepted = false;
    // Ensure the event won't be propagated further
    event->setAccepted(true);
}

void ItemKeyEventFilter::keyReleaseEvent(QKeyEvent* event)
{
    m_qmlAccepted = false;
    event->setAccepted(true);
}

bool ItemKeyEventFilter::eventFilter(QObject*, QEvent* event)
{
    if (!m_enabled)
        return false;

    bool ret = false;
    switch (event->type())
    {
    case QEvent::KeyPress:
    case QEvent::KeyRelease:
        m_qmlAccepted = true;
        QCoreApplication::sendEvent(this, event);
        ret = m_qmlAccepted;
        break;
    default:
        break;
    }
    return ret;
}
