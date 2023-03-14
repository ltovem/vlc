/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *
 * Authors: Benjamin Arnaud <bunjee@omega.gg>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef MLVIDEOGROUPSMODEL_HPP
#define MLVIDEOGROUPSMODEL_HPP

// MediaLibrary includes
#include "mlvideomodel.hpp"

// Forward declarations
class MLGroup;

class MLVideoGroupsModel : public MLVideoModel
{
    Q_OBJECT

public:
    enum Roles
    {
        // NOTE: Group specific.
        GROUP_IS_VIDEO = VIDEO_TITLE_FIRST_SYMBOL + 1,
        GROUP_DATE,
        GROUP_COUNT
    };

public:
    explicit MLVideoGroupsModel(QObject * parent = nullptr);

public: // MLVideoModel reimplementation
    QHash<int, QByteArray> roleNames() const override;

protected: // MLVideoModel reimplementation
    QVariant itemRoleData(MLItem *item, int role = Qt::DisplayRole) const override;

    vlc_ml_sorting_criteria_t roleToCriteria(int role) const override;

    vlc_ml_sorting_criteria_t nameToCriteria(QByteArray name) const override;

    QByteArray criteriaToName(vlc_ml_sorting_criteria_t criteria) const override;

    std::unique_ptr<MLBaseModel::BaseLoader> createLoader() const override;

    void onVlcMlEvent(const MLEvent & event) override;

private:
    struct Loader : public BaseLoader
    {
        Loader(const MLVideoGroupsModel & model);

        size_t count(vlc_medialibrary_t* ml) const override;

        std::vector<std::unique_ptr<MLItem>> load(vlc_medialibrary_t* ml, size_t index, size_t count) const override;

        std::unique_ptr<MLItem> loadItemById(vlc_medialibrary_t* ml, MLItemId itemId) const override;
    };
};

#endif // MLVIDEOGROUPSMODEL_HPP
