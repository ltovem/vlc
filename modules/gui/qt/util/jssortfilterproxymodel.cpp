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
#include "jssortfilterproxymodel.hpp"

#include <QJSEngine>

JSSortFilterProxyModel::JSSortFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{

}

void JSSortFilterProxyModel::setFilterAcceptsRowFunction(const QJSValue &value)
{
    if (m_filterAcceptsRowFunction.strictlyEquals(value))
        return;

    if (!value.isCallable())
        return;

    m_filterAcceptsRowFunction = value;

    emit filterAcceptsRowFunctionChanged();
}

void JSSortFilterProxyModel::setFilterAcceptsColumnFunction(const QJSValue &value)
{
    if (m_filterAcceptsColumnFunction.strictlyEquals(value))
        return;

    if (!value.isCallable())
        return;

    m_filterAcceptsColumnFunction = value;

    emit filterAcceptsColumnFunctionChanged();
}

bool JSSortFilterProxyModel::filterAccepts(QJSValue &evalFunc, const int source, const QModelIndex &source_parent) const
{
    if (evalFunc.isUndefined())
        return true;

    assert(evalFunc.isCallable());

    QJSEngine * const engine = qjsEngine(this);
    assert(engine); // Why are you using this class where there is no engine?

    const QList<QJSValue> params {source, engine->toScriptValue(source_parent)};

    const QJSValue ret = evalFunc.call(params);
    // assert(ret.isBool()); // does not work...
    return ret.toBool();
}

void JSSortFilterProxyModel::invalidateFilter()
{
    QSortFilterProxyModel::invalidateFilter();
}

bool JSSortFilterProxyModel::filterAcceptsRow(const int source_row, const QModelIndex &source_parent) const
{
    return filterAccepts(m_filterAcceptsRowFunction, source_row, source_parent);
}

bool JSSortFilterProxyModel::filterAcceptsColumn(const int source_column, const QModelIndex &source_parent) const
{
    return filterAccepts(m_filterAcceptsColumnFunction, source_column, source_parent);
}
