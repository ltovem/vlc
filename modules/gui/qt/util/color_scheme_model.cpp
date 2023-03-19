// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2021 the VideoLAN team
 *****************************************************************************/

#include "color_scheme_model.hpp"

#include "qt.hpp"

ColorSchemeModel::ColorSchemeModel(QObject* parent)
    : QAbstractListModel(parent)
    , m_list {{qtr("System"), ColorScheme::System}, {qtr("Day"), ColorScheme::Day}, {qtr("Night"), ColorScheme::Night}}
    , m_currentIndex {0}
{
}

int ColorSchemeModel::rowCount(const QModelIndex &) const
{
    return m_list.count();
}

Qt::ItemFlags ColorSchemeModel::flags (const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractListModel::flags(index);
    if (index.isValid())
        return defaultFlags | Qt::ItemIsUserCheckable;
    return defaultFlags;
}

QVariant ColorSchemeModel::data(const QModelIndex &index, const int role) const
{
    if (!checkIndex(index, CheckIndexOption::IndexIsValid))
        return QVariant {};

    if (role == Qt::CheckStateRole)
        return m_currentIndex == index.row() ? Qt::Checked : Qt::Unchecked;

    if (role == Qt::DisplayRole)
        return m_list[index.row()].text;

    return QVariant {};
}

bool ColorSchemeModel::setData(const QModelIndex &index,
                               const QVariant &value, const int role)
{
    if (role == Qt::CheckStateRole
            && checkIndex(index, CheckIndexOption::IndexIsValid)
            && index.row() != m_currentIndex
            && value.type() == QVariant::Bool
            && value.toBool())
    {
        setCurrentIndex(index.row());
        return true;
    }

    return false;
}

int ColorSchemeModel::currentIndex() const
{
    return m_currentIndex;
}

void ColorSchemeModel::setCurrentIndex(const int newIndex)
{
    if (m_currentIndex == newIndex)
        return;

    const auto oldIndex = this->index(m_currentIndex);
    m_currentIndex = newIndex;
    if (m_currentIndex < 0 || m_currentIndex >= m_list.size())
        m_currentIndex = 0;

    emit dataChanged(index(m_currentIndex), index(m_currentIndex), {Qt::CheckStateRole});
    emit dataChanged(oldIndex, oldIndex, {Qt::CheckStateRole});
    emit currentChanged();
}

QString ColorSchemeModel::currentText() const
{
    return m_list[m_currentIndex].text;
}

QVector<ColorSchemeModel::Item> ColorSchemeModel::getSchemes() const
{
    return m_list;
}

ColorSchemeModel::ColorScheme ColorSchemeModel::currentScheme() const
{
    return m_list[m_currentIndex].scheme;
}

void ColorSchemeModel::indexChanged(const int i)
{
    emit dataChanged(index(i), index(i));
    if (i == m_currentIndex)
        emit currentChanged();
}
