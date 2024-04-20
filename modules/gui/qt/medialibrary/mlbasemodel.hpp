/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
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

#ifndef MLBASEMODEL_HPP
#define MLBASEMODEL_HPP

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <vlc_common.h>


#include <memory>

#include <vlc_media_library.h>
#include "mlqmltypes.hpp"
#include "medialib.hpp"
#include "mlevent.hpp"
#include "mlqueryparams.hpp"
#include "util/base_model.hpp"

#include "util/listcacheloader.hpp"

// Fordward declarations
class MediaLib;
template<typename T>
class ListCache;
using MLListCache = ListCache<std::unique_ptr<MLItem>>;
class MLListCacheLoader;

/* Medialib data loader for the cache */
class MLBaseModelPrivate;
class MLBaseModel : public BaseModel
{
    Q_OBJECT
    Q_PROPERTY(MLItemId parentId READ parentId WRITE setParentId NOTIFY parentIdChanged
               RESET unsetParentId FINAL)

    Q_PROPERTY(MediaLib * ml READ ml WRITE setMl NOTIFY mlChanged FINAL)

public:
    explicit MLBaseModel(QObject *parent = nullptr);

    virtual ~MLBaseModel();

    virtual QVariant itemRoleData(MLItem *item, int role) const = 0;

    Q_INVOKABLE void sortByColumn(QByteArray criteria, Qt::SortOrder order);

    Q_INVOKABLE void getData(const QVariant &indexesVariant, QJSValue callback);

    QVariant data(const QModelIndex &index, int role) const override final;

public:
    // properties functions

    MLItemId parentId() const;
    void setParentId(MLItemId parentId);
    void unsetParentId();

    MediaLib* ml() const;
    void setMl(MediaLib* ml);

    Q_INVOKABLE void addAndPlay(const QModelIndexList &list, const QStringList &options = {});

    vlc_ml_sorting_criteria_t getMLSortingCriteria() const;

signals:
    void parentIdChanged();
    void mlChanged();


protected:
    virtual std::unique_ptr<MLListCacheLoader> createMLLoader() const  = 0;

    virtual vlc_ml_sorting_criteria_t roleToCriteria(int role) const = 0;
    static QString getFirstSymbol(QString str);
    virtual vlc_ml_sorting_criteria_t nameToCriteria(QByteArray) const {
        return VLC_ML_SORTING_DEFAULT;
    }
    virtual QByteArray criteriaToName(vlc_ml_sorting_criteria_t ) const
    {
        return "";
    }

    MLItem *item(int signedidx) const;

    // NOTE: This is faster because it only returns items available in cache.
    MLItem *itemCache(int signedidx) const;

    MLItem *findInCache(const MLItemId& id, int *index) const;

    //update and notify changes on an item if this item is in the cache
    void updateItemInCache(const MLItemId& id);

    //delete and notify deletion of an item if this item is in the cache
    //this is only to reflect changes from the ML, it won't alter the database
    void deleteItemInCache(const MLItemId& mlid);

    void moveRangeInCache(int first, int last, int to);
    void deleteRangeInCache(int first, int last);

    virtual void onVlcMlEvent( const MLEvent &event );

    virtual void thumbnailUpdated(const QModelIndex& , MLItem* , const QString& , vlc_ml_thumbnail_status_t )  {}

private:
    static void onVlcMlEvent( void* data, const vlc_ml_event_t* event );

    using ItemCallback = std::function<void (quint64 requestID
                                            , std::vector<std::unique_ptr<MLItem>> &items)>;

    quint64 loadItems(const QVector<int> &index, ItemCallback cb);


protected:
    MLItemId m_parent;

    MediaLib* m_mediaLib = nullptr;
    vlc_ml_sorting_criteria_t m_mlSortingCriteria = VLC_ML_SORTING_DEFAULT;
    std::unique_ptr<vlc_ml_event_callback_t,
                    std::function<void(vlc_ml_event_callback_t*)>> m_ml_event_handle;
    bool m_need_reset = false;


    //loader used to load single items
    mutable std::shared_ptr<MLListCacheLoader> m_itemLoader;

    friend class MLListCacheLoader;

private:
    Q_DECLARE_PRIVATE(MLBaseModel)
};


class MLListCacheLoader: public QObject, public ListCacheLoader<std::unique_ptr<MLItem>>
{
    Q_OBJECT
public:
    struct MLOp {
    public:
        MLOp(MLItemId parentId, QString searchPattern, vlc_ml_sorting_criteria_t sort, bool sort_desc);
        inline MLOp(const MLBaseModel& model)
            : MLOp(model.parentId(), model.searchPattern(), model.getMLSortingCriteria(), model.getSortOrder() == Qt::SortOrder::DescendingOrder)
        {}
        virtual ~MLOp() = default;

        vlc_ml_query_params_t getQueryParams(size_t index = 0, size_t count = 0) const;

        virtual ItemType loadItemById(vlc_medialibrary_t* ml, MLItemId itemId) const = 0;
        virtual size_t count(vlc_medialibrary_t* ml, const vlc_ml_query_params_t* queryParams) const = 0;
        virtual std::vector<ItemType> load(vlc_medialibrary_t* ml, const vlc_ml_query_params_t* queryParams) const = 0;
    protected:
        MLItemId m_parent;
        QByteArray m_searchPattern;
        vlc_ml_sorting_criteria_t m_sort;
        bool m_sortDesc;
    };

public:
    MLListCacheLoader(MediaLib* medialib, std::shared_ptr<MLOp> op, QObject* parent = nullptr);
    ~MLListCacheLoader() = default;

    void cancelTask(size_t taskId) override;
    size_t countTask(std::function<void(size_t taskId, size_t count)> cb) override;
    virtual size_t loadTask(size_t offset, size_t limit,
                            std::function<void(size_t taskId, std::vector<ItemType>& data)> cb) override;
    virtual size_t countAndLoadTask(size_t offset, size_t limit,
                                    std::function<void (size_t, size_t, std::vector<ItemType>&)> cb) override;


    //ML specific operations
    quint64 loadItemsTask(size_t offset, const QVector<int> &indexes, MLBaseModel::ItemCallback cb);
    size_t loadItemByIdTask(MLItemId itemId, std::function<void (size_t, ItemType&&)> cb) const;

protected:
    MediaLib* m_medialib = nullptr;
    std::shared_ptr<MLOp> m_op;
};
#endif // MLBASEMODEL_HPP
