// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *****************************************************************************/

#include "sortfilterproxymodel.hpp"

#include <cassert>

SortFilterProxyModel::SortFilterProxyModel( QObject *parent )
    : QSortFilterProxyModel( parent )
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);

    connect( this, &QAbstractListModel::rowsInserted, this, &SortFilterProxyModel::countChanged );
    connect( this, &QAbstractListModel::rowsRemoved, this, &SortFilterProxyModel::countChanged );
    connect( this, &QAbstractItemModel::modelReset, this, &SortFilterProxyModel::countChanged );
    connect( this, &QAbstractItemModel::layoutChanged, this, &SortFilterProxyModel::countChanged );

    connect( this, &QAbstractProxyModel::sourceModelChanged, this, &SortFilterProxyModel::updateFilterRole );
    connect( this, &QAbstractProxyModel::sourceModelChanged, this, &SortFilterProxyModel::updateSortRole );
}

QString SortFilterProxyModel::searchPattern() const
{
    return filterRegExp().pattern();
}

void SortFilterProxyModel::setSearchPattern( const QString &searchPattern )
{
    setFilterRegExp(searchPattern);
}

QByteArray SortFilterProxyModel::searchRole() const
{
    return m_searchRole;
}

void SortFilterProxyModel::setSearchRole( const QByteArray &searchRole )
{
    m_searchRole = searchRole;
    emit searchRoleChanged();
    updateFilterRole();
}

QString SortFilterProxyModel::sortCriteria() const
{
    return m_sortCriteria;
}

void SortFilterProxyModel::setSortCriteria(const QString &sortCriteria)
{
    if (m_sortCriteria == sortCriteria)
        return;

    m_sortCriteria = sortCriteria;
    updateSortRole();
    emit sortCriteriaChanged();
}

Qt::SortOrder SortFilterProxyModel::sortOrder() const
{
    return QSortFilterProxyModel::sortOrder();
}

void SortFilterProxyModel::setSortOrder(Qt::SortOrder sortOrder)
{
    if (this->sortOrder() == sortOrder)
        return;

    this->sort(0, sortOrder);
    emit sortOrderChanged();
}

int SortFilterProxyModel::count() const
{
    return rowCount();
}

QMap<QString, QVariant> SortFilterProxyModel::getDataAt( int idx )
{
    QMap<QString, QVariant> dataDict;
    QHash<int,QByteArray> roles = roleNames();
    for( const auto role: roles.keys() ) {
        dataDict[roles[role]] = data( index( idx, 0 ), role );
    }
    return dataDict;
}

QModelIndexList SortFilterProxyModel::mapIndexesToSource( const QModelIndexList &indexes )
{
    QModelIndexList sourceIndexes;
    sourceIndexes.reserve( indexes.size() );
    for( const auto &proxyIndex : indexes ) {
        sourceIndexes.push_back( mapToSource(proxyIndex) );
    }
    return sourceIndexes;
}

int SortFilterProxyModel::mapIndexToSource(int idx)
{
    return mapToSource( index( idx, 0 ) ).row();
}

void SortFilterProxyModel::updateFilterRole()
{
    setFilterRole( roleNames().key( m_searchRole ) );
}

void SortFilterProxyModel::updateSortRole()
{
    setSortRole( roleNames().key( m_sortCriteria.toUtf8() ) );
}
