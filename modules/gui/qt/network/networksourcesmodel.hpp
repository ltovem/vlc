// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *****************************************************************************/

#ifndef MLNetworkSourcesModel_HPP
#define MLNetworkSourcesModel_HPP

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <QAbstractListModel>

#include <vlc_media_library.h>
#include <vlc_media_source.h>
#include <vlc_threads.h>
#include <vlc_cxx_helpers.hpp>

#include <maininterface/mainctx.hpp>

#include <memory>

class NetworkSourcesModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(MainCtx* ctx READ getCtx WRITE setCtx NOTIFY ctxChanged FINAL)
    Q_PROPERTY(int count READ getCount NOTIFY countChanged FINAL)

public:
    enum Role {
        SOURCE_NAME = Qt::UserRole + 1,
        SOURCE_LONGNAME,
        SOURCE_TYPE,
        SOURCE_ARTWORK
    };
    Q_ENUM(Role);

    enum ItemType {
        TYPE_DUMMY = -1, // provided for UI for entry "Add a service"
        TYPE_SOURCE = 0
    };
    Q_ENUM(ItemType);

    NetworkSourcesModel( QObject* parent = nullptr );

    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex& parent = {}) const override;

    void setCtx(MainCtx* ctx);

    inline MainCtx* getCtx() { return m_ctx; }

    int getCount() const;

    Q_INVOKABLE QMap<QString, QVariant> getDataAt(int index);

signals:
    void ctxChanged();
    void countChanged();

private:
    struct Item
    {
        QString name;
        QString longName;
        QUrl artworkUrl;
    };

    bool initializeMediaTree();

private:
    std::vector<Item> m_items;
    MainCtx* m_ctx = nullptr;
    services_discovery_category_e m_sdSource = services_discovery_category_e::SD_CAT_INTERNET;
};

#endif // MLNetworkSourcesModel_HPP
