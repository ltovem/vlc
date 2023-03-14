/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef SORT_FILTER_PROXY_MODEL
#define SORT_FILTER_PROXY_MODEL

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "qt.hpp"
#include <QSortFilterProxyModel>

class SortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY( QByteArray searchRole READ searchRole WRITE setSearchRole NOTIFY searchRoleChanged  FINAL)
    Q_PROPERTY( QString searchPattern READ searchPattern WRITE setSearchPattern NOTIFY searchPatternChanged  FINAL)
    Q_PROPERTY( QString sortCriteria READ sortCriteria WRITE setSortCriteria NOTIFY sortCriteriaChanged  FINAL)
    Q_PROPERTY( Qt::SortOrder sortOrder READ sortOrder WRITE setSortOrder NOTIFY sortOrderChanged  FINAL)
    Q_PROPERTY( int count READ count NOTIFY countChanged  FINAL)

public:
    SortFilterProxyModel( QObject * parent = nullptr );

    QString searchPattern() const;
    void setSearchPattern( const QString &searchPattern );

    QByteArray searchRole() const;
    void setSearchRole( const QByteArray &searchRole );

    QString sortCriteria() const;
    void setSortCriteria( const QString &sortCriteria );

    Qt::SortOrder sortOrder() const;
    void setSortOrder(Qt::SortOrder sortOrder);

    int count() const;

    Q_INVOKABLE QMap<QString, QVariant> getDataAt( int idx );
    Q_INVOKABLE QModelIndexList mapIndexesToSource( const QModelIndexList &indexes );
    Q_INVOKABLE int mapIndexToSource( int idx );

signals:
    void searchPatternChanged();
    void searchRoleChanged();
    void countChanged();
    void sortCriteriaChanged();
    void sortOrderChanged();

private slots:
    void updateFilterRole();
    void updateSortRole();

private:
    QByteArray m_searchRole;
    QString m_sortCriteria;
};

#endif // SORT_FILTER_PROXY_MODEL
