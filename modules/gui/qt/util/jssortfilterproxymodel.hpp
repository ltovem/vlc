/*****************************************************************************
 * Copyright (C) 2023 VLC authors and VideoLAN
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
#ifndef JSSORTFILTERPROXYMODEL_HPP
#define JSSORTFILTERPROXYMODEL_HPP

#include <QSortFilterProxyModel>
#include <QObject>
#include <QJSValue>

class JSSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY(QJSValue filterAcceptsRowFunction
                   MEMBER m_filterAcceptsRowFunction
                   WRITE setFilterAcceptsRowFunction
                   NOTIFY filterAcceptsRowFunctionChanged
                   FINAL)

    Q_PROPERTY(QJSValue filterAcceptsColumnFunction
                    MEMBER m_filterAcceptsColumnFunction
                    WRITE setFilterAcceptsColumnFunction
                    NOTIFY filterAcceptsColumnFunctionChanged
                    FINAL)

public:
    explicit JSSortFilterProxyModel(QObject * parent = nullptr);

    void setFilterAcceptsRowFunction(const QJSValue& value);
    void setFilterAcceptsColumnFunction(const QJSValue& value);

public slots:
    void invalidateFilter();

signals:
    void filterAcceptsRowFunctionChanged();
    void filterAcceptsColumnFunctionChanged();

protected:
    bool filterAcceptsRow(const int source_row, const QModelIndex &source_parent) const override;
    bool filterAcceptsColumn(const int source_column, const QModelIndex &source_parent) const override;

    bool filterAccepts(QJSValue& evalFunc, const int source, const QModelIndex &source_parent) const;

private:
    // TODO: Qt 7 remove mutable. They forgot to add const...
    mutable QJSValue m_filterAcceptsRowFunction;
    mutable QJSValue m_filterAcceptsColumnFunction;
};

#endif // JSSORTFILTERPROXYMODEL_HPP
