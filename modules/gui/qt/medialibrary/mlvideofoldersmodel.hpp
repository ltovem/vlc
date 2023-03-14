/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *
 * Authors: Benjamin Arnaud <bunjee@omega.gg>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef MLVIDEOFOLDERSMODEL_HPP
#define MLVIDEOFOLDERSMODEL_HPP

// MediaLibrary includes
#include "mlvideomodel.hpp"

// Forward declarations
class MLFolder;

class MLVideoFoldersModel : public MLBaseModel
{
    Q_OBJECT

public:
    enum Roles
    {
        FOLDER_ID = Qt::UserRole + 1,
        FOLDER_TITLE,
        FOLDER_THUMBNAIL,
        FOLDER_DURATION,
        FOLDER_COUNT
    };

public:
    explicit MLVideoFoldersModel(QObject * parent = nullptr);

public: // MLBaseModel reimplementation
    QHash<int, QByteArray> roleNames() const override;

protected:
    QVariant itemRoleData(MLItem *item, int role = Qt::DisplayRole) const override;

    vlc_ml_sorting_criteria_t roleToCriteria(int role) const override;

    vlc_ml_sorting_criteria_t nameToCriteria(QByteArray name) const override;

    QByteArray criteriaToName(vlc_ml_sorting_criteria_t criteria) const override;

    std::unique_ptr<MLBaseModel::BaseLoader> createLoader() const override;

protected: // MLBaseModel reimplementation
    void onVlcMlEvent(const MLEvent & event) override;

private:
    struct Loader : public BaseLoader
    {
        Loader(const MLVideoFoldersModel & model);

        size_t count(vlc_medialibrary_t * ml) const override;

        std::vector<std::unique_ptr<MLItem>> load(vlc_medialibrary_t * ml,
                                                  size_t index, size_t count) const override;

        std::unique_ptr<MLItem> loadItemById(vlc_medialibrary_t* ml, MLItemId itemId) const override;
    };
};

#endif // MLVIDEOFOLDERSMODEL_HPP
