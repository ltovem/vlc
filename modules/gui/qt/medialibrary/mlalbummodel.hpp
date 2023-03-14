/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef MLABLUMMODEL_H
#define MLABLUMMODEL_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "mlbasemodel.hpp"
#include "mlalbum.hpp"
#include "medialib.hpp"

class MLAlbumModel : public MLBaseModel
{
    Q_OBJECT

public:
    enum Roles
    {
        ALBUM_ID = Qt::UserRole + 1,
        ALBUM_TITLE,
        ALBUM_RELEASE_YEAR,
        ALBUM_SHORT_SUMMARY,
        ALBUM_COVER,
        ALBUM_MAIN_ARTIST,
        ALBUM_NB_TRACKS,
        ALBUM_DURATION,

        ALBUM_TITLE_FIRST_SYMBOL,
        ALBUM_MAIN_ARTIST_FIRST_SYMBOL
    };
    Q_ENUM(Roles)

public:
    explicit MLAlbumModel(QObject *parent = nullptr);
    virtual ~MLAlbumModel() = default;

    Q_INVOKABLE QHash<int, QByteArray> roleNames() const override;

protected:
    QVariant itemRoleData(MLItem *item, int role) const override;

    std::unique_ptr<BaseLoader> createLoader() const override;

private:
    vlc_ml_sorting_criteria_t roleToCriteria(int role) const override;
    vlc_ml_sorting_criteria_t nameToCriteria(QByteArray name) const override;
    QByteArray criteriaToName(vlc_ml_sorting_criteria_t criteria) const override;
    virtual void onVlcMlEvent( const MLEvent &event ) override;

    static  QHash<QByteArray, vlc_ml_sorting_criteria_t> M_names_to_criteria;

    struct Loader : public BaseLoader
    {
        Loader(const MLAlbumModel &model) : BaseLoader(model) {}
        size_t count(vlc_medialibrary_t* ml) const override;
        std::vector<std::unique_ptr<MLItem>> load(vlc_medialibrary_t* ml, size_t index, size_t count) const override;
        std::unique_ptr<MLItem> loadItemById(vlc_medialibrary_t* ml, MLItemId itemId) const override;
    };
};


#endif // MLABLUMMODEL_H
