// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *****************************************************************************/

#ifndef MLARTISTMODEL_HPP
#define MLARTISTMODEL_HPP

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "mlbasemodel.hpp"
#include "mlartist.hpp"

class MLArtistModel : public MLBaseModel
{
    Q_OBJECT
public:
    enum Roles
    {
        ARTIST_ID = Qt::UserRole + 1,
        ARTIST_NAME,
        ARTIST_SHORT_BIO,
        ARTIST_COVER,
        ARTIST_NB_ALBUMS,
        ARTIST_NB_TRACKS
    };

public:
    explicit MLArtistModel(QObject *parent = nullptr);
    virtual ~MLArtistModel() = default;

    QHash<int, QByteArray> roleNames() const override;

protected:
    QVariant itemRoleData(MLItem *item, int role) const override;

    std::unique_ptr<MLBaseModel::BaseLoader> createLoader() const override;

private:
    vlc_ml_sorting_criteria_t roleToCriteria(int role) const override;
    vlc_ml_sorting_criteria_t nameToCriteria(QByteArray name) const override;
    QByteArray criteriaToName(vlc_ml_sorting_criteria_t criteria) const override;
    virtual void onVlcMlEvent(const MLEvent &event) override;

    static QHash<QByteArray, vlc_ml_sorting_criteria_t> M_names_to_criteria;

    struct Loader : public BaseLoader
    {
        Loader(const MLArtistModel &model) : BaseLoader(model) {}
        size_t count(vlc_medialibrary_t* ml) const override;
        std::vector<std::unique_ptr<MLItem>> load(vlc_medialibrary_t* ml, size_t index, size_t count) const override;
        std::unique_ptr<MLItem> loadItemById(vlc_medialibrary_t* ml, MLItemId itemId) const override;
    };
};

#endif // MLARTISTMODEL_HPP
