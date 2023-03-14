/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Benjamin Arnaud <bunjee@omega.gg>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef STANDARDPATHMODEL_HPP
#define STANDARDPATHMODEL_HPP

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// VLC includes
#include <vlc_media_source.h>
#include <vlc_cxx_helpers.hpp>
#include "networkdevicemodel.hpp"
#include "util/cliplistmodel.hpp"

// Qt includes
#include <QAbstractListModel>
#include <QStandardPaths>
#include <QUrl>

struct StandardPathItem;

class StandardPathModel : public ClipListModel<StandardPathItem>
{
    Q_OBJECT

public: // Enums
    // NOTE: Roles should be aligned with the NetworkDeviceModel.
    enum Role
    {
        PATH_NAME = Qt::UserRole + 1,
        PATH_MRL,
        PATH_TYPE,
        PATH_PROTOCOL,
        PATH_SOURCE,
        PATH_TREE,
        PATH_ARTWORK
    };

public: // Declarations
    using InputItemPtr = vlc_shared_data_ptr_type(input_item_t,
                                                  input_item_Hold,
                                                  input_item_Release);

    using MediaTreePtr = vlc_shared_data_ptr_type(vlc_media_tree_t,
                                                  vlc_media_tree_Hold,
                                                  vlc_media_tree_Release);

public:
    StandardPathModel(QObject * parent = nullptr);

public: // QAbstractItemModel implementation
    QHash<int, QByteArray> roleNames() const override;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;

protected: // ClipListModel implementation
    void onUpdateSort(const QString & criteria, Qt::SortOrder order) override;

private: // Static functions
    static bool ascendingName(const StandardPathItem & a, const StandardPathItem & b);
    static bool ascendingMrl (const StandardPathItem & a, const StandardPathItem & b);

    static bool descendingName(const StandardPathItem & a, const StandardPathItem & b);
    static bool descendingMrl (const StandardPathItem & a, const StandardPathItem & b);

private: // Functions
    void addItem(const QString & path, const QString & name, const QUrl & artwork);
};

struct StandardPathItem
{
    QString name;
    QUrl    mrl;

    QString protocol;

    NetworkDeviceModel::ItemType type;

    StandardPathModel::InputItemPtr inputItem;
    StandardPathModel::MediaTreePtr tree;

    QUrl artwork;
};

#endif // STANDARDPATHMODEL_HPP
