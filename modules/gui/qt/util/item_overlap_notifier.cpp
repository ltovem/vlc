/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
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

#include "item_overlap_notifier.hpp"

#include <QQuickItem>

#define PARSE_TIMER_INTERVAL 0

ItemOverlapNotifier::ItemOverlapNotifier(QObject *parent)
    : QObject(parent)
{
    m_parseTimer.setInterval(PARSE_TIMER_INTERVAL);
    m_parseTimer.setSingleShot(true);
    connect(&m_parseTimer, &QTimer::timeout, this, &ItemOverlapNotifier::parseOverlaps);
}

void ItemOverlapNotifier::setMaskItem(QQuickItem *newMaskItem)
{
    if (m_maskItem == newMaskItem)
        return;

    if (m_maskItem)
        disconnect(m_maskItem, nullptr, this, nullptr);

    m_maskItem = newMaskItem;

    if (m_maskItem)
    {
        connect (m_maskItem, &QQuickItem::visibleChanged, this, [this]() {
            if (!m_maskItem->isVisible())
            {
                m_parseTimer.stop();
                m_setNotified.clear();
            }
        });

        const auto slot = [this]() {
            if (!m_parseTimer.isActive())
                m_parseTimer.start();
        };

        connect(m_maskItem, &QQuickItem::xChanged, this, slot);
        connect(m_maskItem, &QQuickItem::yChanged, this, slot);
        connect(m_maskItem, &QQuickItem::widthChanged, this, slot);
        connect(m_maskItem, &QQuickItem::heightChanged, this, slot);
        connect(m_maskItem, &QQuickItem::visibleChanged, this, slot);
    }

    emit maskItemChanged();
}

void ItemOverlapNotifier::parseOverlaps()
{
    assert (m_maskItem);
    assert (m_containerItem);
    assert (m_maskItem->parentItem());

    if (!m_maskItem->isVisible())
        return;

    const QRectF selectorGeometry = m_containerItem->mapRectFromItem(m_maskItem->parentItem(),
                                                                     {m_maskItem->x(),
                                                                      m_maskItem->y(),
                                                                      m_maskItem->width(),
                                                                      m_maskItem->height()});

    const auto& childItems = m_containerItem->childItems();
    for (const auto item : childItems)
    {
        if (item == m_maskItem || !item->isVisible())
            continue;

        const QRectF itemGeometry (item->x(), item->y(), item->width(), item->height());
        if (itemGeometry.isEmpty())
            continue;

        const auto itemPtr = reinterpret_cast<qintptr>(item);
        const auto iterator = m_setNotified.find(itemPtr);
        const bool alreadyNotified = (iterator != m_setNotified.end());

        if (selectorGeometry.intersects(itemGeometry) != alreadyNotified)
        {
            emit overlapToggled(item);

            if (alreadyNotified)
                m_setNotified.erase(iterator);
            else
                m_setNotified.insert(itemPtr);
        }
    }
}

