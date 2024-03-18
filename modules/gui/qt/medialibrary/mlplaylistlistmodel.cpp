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

#if HAVE_CONFIG_H
# include "config.h"
#endif

#include "mlplaylistlistmodel.hpp"

// VLC includes
#include <vlc_media_library.h>
#include "qt.hpp"
#include "util/vlctick.hpp"
#include "dialogs/dialogs_provider.hpp"

// MediaLibrary includes
#include "mlhelper.hpp"
#include "mlcustomcover.hpp"
#include "mlplaylist.hpp"

//-------------------------------------------------------------------------------------------------
// Static variables


namespace  {

// NOTE: We multiply by 3 to cover most dpi settings.
const int MLPLAYLISTMODEL_COVER_WIDTH  = 260 * 3; // 16 / 10 ratio
const int MLPLAYLISTMODEL_COVER_HEIGHT = 162 * 3;

const int PLAYLIST_COVERX = 2;
const int PLAYLIST_COVERY = 2;

void appendMediaIntoPlaylist(vlc_medialibrary_t* ml, int64_t playlistId, const std::vector<MLItemId>& itemList)
{
    vlc_ml_query_params_t query;
    memset(&query, 0, sizeof(vlc_ml_query_params_t));

    for (auto itemId : itemList)
    {
        // NOTE: When we have a parent it's a collection of media(s).
        if (itemId.type != VLC_ML_PARENT_UNKNOWN)
        {
            ml_unique_ptr<vlc_ml_media_list_t> list;

            list.reset(vlc_ml_list_media_of(ml, &query, itemId.type, itemId.id));

            if (!list)
                continue;

            for (const vlc_ml_media_t & media : ml_range_iterate<vlc_ml_media_t>(list))
                vlc_ml_playlist_append(ml, playlistId, media.i_id);
        }
        // NOTE: Otherwise we add the media directly.
        else
            vlc_ml_playlist_append(ml, playlistId, itemId.id);
    }
}

} //anonymous namespace

//=================================================================================================
// MLPlaylistListModel
//=================================================================================================

/* explicit */ MLPlaylistListModel::MLPlaylistListModel(QObject * parent)
    : MLBaseModel(parent)
    , m_coverSize(MLPLAYLISTMODEL_COVER_WIDTH, MLPLAYLISTMODEL_COVER_HEIGHT) {}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE */ void MLPlaylistListModel::create(const QString & name, const QVariantList& initialItems)
{
    assert(m_mediaLib);


    std::vector<MLItemId> itemList;
    for (const QVariant & id : initialItems)
    {
        if (id.canConvert<MLItemId>() == false)
            continue;

        const MLItemId & itemId = id.value<MLItemId>();

        if (itemId.id == 0)
            continue;
        itemList.push_back(itemId);
    }

    m_mediaLib->runOnMLThread(this,
    //ML thread
    [name, itemList](vlc_medialibrary_t* ml)
    {
        vlc_ml_playlist_t * playlist = vlc_ml_playlist_create(ml, qtu(name));
        if (playlist)
        {
            auto playlistId = playlist->i_id;
            vlc_ml_playlist_release(playlist);

            appendMediaIntoPlaylist(ml, playlistId, itemList);
        }
    });
}

/* Q_INVOKABLE */ void MLPlaylistListModel::append(const MLItemId     & playlistId,
                                                   const QVariantList & ids)
{
    assert(m_mediaLib);

    if (unlikely(m_transactionPending))
        return;

    m_transactionPending = true;

    bool result = true;
    std::vector<MLItemId> itemList;
    for (const QVariant & id : ids)
    {
        if (id.canConvert<MLItemId>() == false)
        {
            result = false;
            continue;
        }

        const MLItemId & itemId = id.value<MLItemId>();

        if (itemId.id == 0)
        {
            result = false;
            continue;
        }
        itemList.push_back(itemId);
    }

    m_mediaLib->runOnMLThread(this,
    //ML thread
    [playlistId, itemList](vlc_medialibrary_t* ml)
    {
        appendMediaIntoPlaylist(ml, playlistId.id, itemList);
    },
    //UI thread
    [this]() {
        endTransaction();
    });
}

/* Q_INVOKABLE */ bool MLPlaylistListModel::deletePlaylists(const QVariantList & ids)
{
    assert(m_mediaLib);

    if (unlikely(m_transactionPending))
        return false;

    m_transactionPending = true;

    std::vector<MLItemId> itemList;
    for (const QVariant & id : ids)
    {
        if (id.canConvert<MLItemId>() == false)
            continue;

        const MLItemId & itemId = id.value<MLItemId>();

        if (itemId.id == 0)
            continue;

        itemList.push_back(itemId);
    }

    m_mediaLib->runOnMLThread(this,
    //ML thread
    [itemList](vlc_medialibrary_t* ml)
    {
        for (auto itemId : itemList)
            vlc_ml_playlist_delete(ml, itemId.id);
    },
    //UI thread
    [this](){
        endTransaction();
    });

    return true;
}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE */ bool MLPlaylistListModel::showDialogRename(const QModelIndex & index)
{
    int row = index.row();

    if (row < 0 || row >= rowCount())
        return false;

    MLPlaylist * playlist = static_cast<MLPlaylist *> (item(row));

    QString name = playlist->getName();

    static DialogsProvider * provider = DialogsProvider::getInstance();

    bool ok = false;

    QString result = provider->getTextDialog(NULL, qtr("Rename playlist"),
                                             qtr("Please enter the new playlist name:"),
                                             name, &ok).toString();

    if (ok == false || result.isEmpty() || result == name)
        return false;

    int64_t id = playlist->getId().id;

    m_mediaLib->runOnMLThread(this,
    // ML thread
    [id, result](vlc_medialibrary_t * ml)
    {
        vlc_ml_playlist_rename(ml, id, qtu(result));
    });

    // NOTE: We want the change to be visible right away.
    playlist->setName(result);

    emit dataChanged(index, index, { PLAYLIST_NAME });

    return true;
}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE */ MLItemId MLPlaylistListModel::getItemId(int index) const
{
    if (index < 0 || index >= rowCount())
        return MLItemId();

    return item(index)->getId();
}

//-------------------------------------------------------------------------------------------------
// QAbstractItemModel implementation
//-------------------------------------------------------------------------------------------------

QHash<int, QByteArray> MLPlaylistListModel::roleNames() const /* override */
{
    return
    {
        { PLAYLIST_ID,        "id"        },
        { PLAYLIST_NAME,      "name"      },
        { PLAYLIST_THUMBNAIL, "thumbnail" },
        { PLAYLIST_DURATION,  "duration"  },
        { PLAYLIST_COUNT,     "count"     }
    };
}

QVariant MLPlaylistListModel::itemRoleData(MLItem *item, int role) const /* override */
{
    MLPlaylist * playlist = static_cast<MLPlaylist *>(item);
    if (playlist == nullptr)
        return QVariant();

    switch (role)
    {
        // NOTE: This is the condition for QWidget view(s).
        case Qt::DisplayRole:
            return playlist->getName();
        // NOTE: These are the conditions for QML view(s).
        case PLAYLIST_ID:
            return QVariant::fromValue(playlist->getId());
        case PLAYLIST_NAME:
            return playlist->getName();
        case PLAYLIST_THUMBNAIL:
            return getCover(playlist);
        case PLAYLIST_DURATION:
            return QVariant::fromValue(playlist->getDuration());
        case PLAYLIST_COUNT:
            return QVariant::fromValue(playlist->getCount());
        default:
            return QVariant();
    }
}

//-------------------------------------------------------------------------------------------------
// QAbstractItemModel reimplementation
//-------------------------------------------------------------------------------------------------

QVariant MLPlaylistListModel::headerData(int section, Qt::Orientation orientation,
                                         int role) const /* override */
{
    if (role != Qt::DisplayRole || orientation == Qt::Vertical)
        return QVariant();

    if (section == 0)
        return QVariant::fromValue(qtr("Name"));
    else
        return QVariant();
}

//-------------------------------------------------------------------------------------------------
// Protected MLBaseModel implementation
//-------------------------------------------------------------------------------------------------

vlc_ml_sorting_criteria_t MLPlaylistListModel::roleToCriteria(int role) const /* override */
{
    if (role == PLAYLIST_NAME)
        return VLC_ML_SORTING_ALPHA;
    else
        return VLC_ML_SORTING_DEFAULT;
}

std::unique_ptr<MLListCacheLoader> MLPlaylistListModel::createMLLoader() const /* override */
{
    return std::make_unique<MLListCacheLoader>(m_mediaLib, std::make_shared<MLPlaylistListModel::Loader>(*this, m_playlistType));
}

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------

void MLPlaylistListModel::endTransaction()
{
    m_transactionPending = false;
    if (m_resetAfterTransaction)
    {
        m_resetAfterTransaction = false;
        emit resetRequested();
    }
}

QString MLPlaylistListModel::getCover(MLPlaylist * playlist) const
{
    return ml()->customCover()->get(playlist->getId()
                                    , m_coverSize
                                    , m_coverDefault
                                    , PLAYLIST_COVERX, PLAYLIST_COVERY);
}

//-------------------------------------------------------------------------------------------------
// Private MLBaseModel reimplementation
//-------------------------------------------------------------------------------------------------

void MLPlaylistListModel::onVlcMlEvent(const MLEvent & event) /* override */
{
    if (m_transactionPending
        && (event.i_type == VLC_ML_EVENT_PLAYLIST_UPDATED
            || event.i_type == VLC_ML_EVENT_PLAYLIST_DELETED
            || event.i_type == VLC_ML_EVENT_PLAYLIST_ADDED))
    {
        m_resetAfterTransaction = true;
        return;
    }

    switch (event.i_type)
    {
    case VLC_ML_EVENT_PLAYLIST_DELETED:
    {
        MLItemId itemId(event.deletion.i_entity_id, VLC_ML_PARENT_PLAYLIST);
        deleteItemInCache(itemId);
        return;
    }
    case VLC_ML_EVENT_PLAYLIST_UPDATED:
    {

        MLItemId itemId(event.modification.i_entity_id, VLC_ML_PARENT_PLAYLIST);
        updateItemInCache(itemId);
        return;
    }
    case VLC_ML_EVENT_PLAYLIST_ADDED:
    {
        emit resetRequested();
        break;
    }
    default:
        break;
    }

    MLBaseModel::onVlcMlEvent(event);
}

//-------------------------------------------------------------------------------------------------
// Properties
//-------------------------------------------------------------------------------------------------

QSize MLPlaylistListModel::coverSize() const
{
    return m_coverSize;
}

void MLPlaylistListModel::setCoverSize(const QSize & size)
{
    if (m_coverSize == size)
        return;

    m_coverSize = size;

    emit coverSizeChanged();
}

QString MLPlaylistListModel::coverDefault() const
{
    return m_coverDefault;
}

void MLPlaylistListModel::setCoverDefault(const QString & fileName)
{
    if (m_coverDefault == fileName)
        return;

    m_coverDefault = fileName;

    emit coverDefaultChanged();
}

QString MLPlaylistListModel::coverPrefix() const
{
    return m_coverPrefix;
}

void MLPlaylistListModel::setCoverPrefix(const QString & prefix)
{
    if (m_coverPrefix == prefix)
        return;

    m_coverPrefix = prefix;

    emit coverPrefixChanged();
}

MLPlaylistListModel::PlaylistType MLPlaylistListModel::playlistType() const
{
    return m_playlistType;
}

void MLPlaylistListModel::setPlaylistType(PlaylistType playlistType)
{
    if (m_playlistType == playlistType)
        return;
    m_playlistType = playlistType;
    resetCache();
    emit playlistTypeChanged();
}

//=================================================================================================
// Loader
//=================================================================================================

static inline vlc_ml_playlist_type_t qmlToMLPlaylistType(MLPlaylistListModel::PlaylistType type)
{
    switch(type) {
    case MLPlaylistListModel::PlaylistType::PLAYLIST_TYPE_ALL:
        return VLC_ML_PLAYLIST_TYPE_ALL;
    case MLPlaylistListModel::PlaylistType::PLAYLIST_TYPE_AUDIO:
        return VLC_ML_PLAYLIST_TYPE_AUDIO;
    case MLPlaylistListModel::PlaylistType::PLAYLIST_TYPE_VIDEO:
        return VLC_ML_PLAYLIST_TYPE_VIDEO;
    default:
        vlc_assert_unreachable();
    }
}

MLPlaylistListModel::Loader::Loader(const MLPlaylistListModel & model, PlaylistType playlistType)
    : MLListCacheLoader::MLOp(model)
    , m_playlistType(playlistType)
{}

size_t MLPlaylistListModel::Loader::count(vlc_medialibrary_t* ml, const vlc_ml_query_params_t* queryParams) const /* override */
{
    vlc_ml_playlist_type_t mlPlaylistType = qmlToMLPlaylistType(m_playlistType);

    return vlc_ml_count_playlists(ml, queryParams, mlPlaylistType);
}

std::vector<std::unique_ptr<MLItem>>
MLPlaylistListModel::Loader::load(vlc_medialibrary_t* ml, const vlc_ml_query_params_t* queryParams) const /* override */
{
    vlc_ml_playlist_type_t mlPlaylistType = qmlToMLPlaylistType(m_playlistType);

    ml_unique_ptr<vlc_ml_playlist_list_t> list(vlc_ml_list_playlists(ml, queryParams, mlPlaylistType));

    if (list == nullptr)
        return {};

    std::vector<std::unique_ptr<MLItem>> result;

    for (const vlc_ml_playlist_t & playlist : ml_range_iterate<vlc_ml_playlist_t>(list))
    {
        result.emplace_back(std::make_unique<MLPlaylist>(&playlist));
    }

    return result;
}

std::unique_ptr<MLItem>
MLPlaylistListModel::Loader::loadItemById(vlc_medialibrary_t* ml, MLItemId itemId) const
{
    assert(itemId.type == VLC_ML_PARENT_PLAYLIST);
    ml_unique_ptr<vlc_ml_playlist_t> playlist(vlc_ml_get_playlist(ml, itemId.id));
    if (!playlist)
        return nullptr;
    return std::make_unique<MLPlaylist>(playlist.get());
}
