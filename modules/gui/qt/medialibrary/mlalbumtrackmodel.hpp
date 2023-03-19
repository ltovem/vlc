// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *****************************************************************************/

#ifndef MLTRACKMODEL_HPP
#define MLTRACKMODEL_HPP

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "mlbasemodel.hpp"
#include "mlalbumtrack.hpp"

class MLAlbumTrackModel : public MLBaseModel
{
    Q_OBJECT

public:
    enum Roles {
        TRACK_ID = Qt::UserRole + 1,
        TRACK_TITLE,
        TRACK_COVER,
        TRACK_NUMBER,
        TRACK_DISC_NUMBER,
        TRACK_DURATION,
        TRACK_ALBUM,
        TRACK_ARTIST,

        TRACK_TITLE_FIRST_SYMBOL,
        TRACK_ALBUM_FIRST_SYMBOL,
        TRACK_ARTIST_FIRST_SYMBOL,
    };

public:
    explicit MLAlbumTrackModel(QObject *parent = nullptr);

    virtual ~MLAlbumTrackModel() = default;

    QHash<int, QByteArray> roleNames() const override;

protected:
    QVariant itemRoleData(MLItem *item, int role) const override;

    std::unique_ptr<MLBaseModel::BaseLoader> createLoader() const override;

private:
    vlc_ml_sorting_criteria_t roleToCriteria(int role) const override;
    vlc_ml_sorting_criteria_t nameToCriteria(QByteArray name) const override;
    QByteArray criteriaToName(vlc_ml_sorting_criteria_t criteria) const override;
    virtual void onVlcMlEvent( const MLEvent &event ) override;

    static QHash<QByteArray, vlc_ml_sorting_criteria_t> M_names_to_criteria;

    struct Loader : public BaseLoader
    {
        Loader(const MLAlbumTrackModel &model) : BaseLoader(model) {}
        size_t count(vlc_medialibrary_t* ml) const override;
        std::vector<std::unique_ptr<MLItem>> load(vlc_medialibrary_t* ml, size_t index, size_t count) const override;
        std::unique_ptr<MLItem> loadItemById(vlc_medialibrary_t* ml, MLItemId itemId) const override;
    };
};
#endif // MLTRACKMODEL_HPP
