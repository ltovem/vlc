/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef PROXY_COLUMN_MODEL
#define PROXY_COLUMN_MODEL

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "qt.hpp"
#include <type_traits>


template <typename Model>
class ProxyColumnModel : public Model
{
public:
    ProxyColumnModel(const int extraColumns, const QHash<int, QString> &sectionDisplayData, QObject *parent = nullptr)
        : Model {parent}
        , m_extraColumns {extraColumns}
        , m_sectionDisplayData {sectionDisplayData}
    {
    }

    int columnCount(const QModelIndex &) const override
    {
        return 1 + m_extraColumns;
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override
    {
        if (role == Qt::DisplayRole && m_sectionDisplayData.contains(section))
            return m_sectionDisplayData.value(section);

        return Model::headerData(section, orientation, role);
    }

    QVariant data(const QModelIndex &index, const int role) const override
    {
        if (index.column() != 0)
            return {};

        return Model::data(index, role);
    }

private:
    const int m_extraColumns;
    const QHash<int, QString> m_sectionDisplayData;

    static_assert(std::is_base_of<QAbstractListModel, Model>::value, "must be a subclass of list model");
};

#endif // PROXY_COLUMN_MODEL
