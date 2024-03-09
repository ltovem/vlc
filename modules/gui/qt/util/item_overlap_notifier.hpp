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

#ifndef ITEM_OVERLAP_NOTIFIER_HPP
#define ITEM_OVERLAP_NOTIFIER_HPP

#include <QObject>
#include <QTimer>
#include <QSet>
#include <QPointer>
#include <QQuickItem>

class ItemOverlapNotifier : public QObject
{
    static_assert(sizeof(qintptr) >= sizeof(int), "Can not store (int) as (qintptr).");

    Q_OBJECT

    Q_PROPERTY(QQuickItem* maskItem MEMBER m_maskItem WRITE setMaskItem NOTIFY maskItemChanged FINAL)
    Q_PROPERTY(QQuickItem* containerItem MEMBER m_containerItem NOTIFY containerItemChanged FINAL)

public:
    explicit ItemOverlapNotifier(QObject *parent = nullptr);

    void setMaskItem(QQuickItem *newMaskItem);

signals:
    void maskItemChanged();
    void containerItemChanged();

    void overlapToggled(QQuickItem* item);

private slots:
    void parseOverlaps();

private:
    QTimer m_parseTimer;

    QPointer<QQuickItem> m_maskItem;
    QPointer<QQuickItem> m_containerItem;

    QSet<qintptr> m_setNotified;
};

#endif // ITEM_OVERLAP_NOTIFIER_HPP
