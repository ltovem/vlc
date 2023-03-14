/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *
 * Authors: Benjamin Arnaud <bunjee@omega.gg>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#include "cliplistmodel.hpp"

// BaseClipListModel

/* explicit */ BaseClipListModel::BaseClipListModel(QObject * parent)
    : QAbstractListModel(parent) {}

// Interface

void BaseClipListModel::updateItems()
{
    int count = implicitCount();

    if (m_count == count) return;

    if (m_count < count)
        expandItems(count);
    else
        shrinkItems(count);
}

// QAbstractItemModel implementation

int BaseClipListModel::rowCount(const QModelIndex &) const /* override */
{
    return count();
}

// Properties

int BaseClipListModel::count() const
{
    return m_count;
}

int BaseClipListModel::maximumCount() const
{
    return m_maximumCount;
}

void BaseClipListModel::setMaximumCount(int count)
{
    if (m_maximumCount == count)
        return;

    m_maximumCount = count;

    count = implicitCount();

    if (m_count == count)
    {
        emit maximumCountChanged();

        return;
    }

    if (m_count < count)
        expandItems(count);
    else
        shrinkItems(count);

    emit maximumCountChanged();
}

QString BaseClipListModel::searchPattern() const
{
    return m_searchPattern;
}

void BaseClipListModel::setSearchPattern(const QString & pattern)
{
    if (m_searchPattern == pattern)
        return;

    m_searchPattern = pattern;

    emit searchPatternChanged();
}

QByteArray BaseClipListModel::searchRole() const
{
    return m_searchRole;
}

void BaseClipListModel::setSearchRole(const QByteArray & role)
{
    if (m_searchRole == role)
        return;

    m_searchRole = role;

    emit searchRoleChanged();
}

QString BaseClipListModel::sortCriteria() const
{
    return m_sortCriteria;
}

void BaseClipListModel::setSortCriteria(const QString & criteria)
{
    if (m_sortCriteria == criteria)
        return;

    m_sortCriteria = criteria;

    updateSort();

    emit sortCriteriaChanged();
}

Qt::SortOrder BaseClipListModel::sortOrder() const
{
    return m_sortOrder;
}

void BaseClipListModel::setSortOrder(Qt::SortOrder order)
{
    if (m_sortOrder == order)
        return;

    m_sortOrder = order;

    updateSort();

    emit sortOrderChanged();
}
