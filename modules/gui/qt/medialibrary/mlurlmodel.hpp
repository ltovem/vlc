// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *****************************************************************************/

#ifndef MLURLMODEL_H
#define MLURLMODEL_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "mlbasemodel.hpp"

#include <vlc_media_library.h>
#include "mlhelper.hpp"
#include "mlqmltypes.hpp"

class MLUrl : public MLItem {
public:
    MLUrl( const vlc_ml_media_t *_data );

    MLUrl( const MLUrl& url );

    QString getUrl() const;
    QString getLastPlayedDate() const;

private:
    QString m_url;
    QString m_lastPlayedDate;
};

class MLUrlModel : public MLBaseModel
{
    Q_OBJECT

public:
    enum Roles {
        URL_ID = Qt::UserRole + 1,
        URL_URL,
        URL_LAST_PLAYED_DATE
    };
    Q_ENUM(Roles);

    explicit MLUrlModel(QObject *parent = nullptr);

    virtual ~MLUrlModel() = default;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void addAndPlay( const QString& url );

protected:
    QVariant itemRoleData(MLItem *item, int role) const override;

    std::unique_ptr<MLBaseModel::BaseLoader> createLoader() const override;

private:
    vlc_ml_sorting_criteria_t roleToCriteria(int role) const override;
    virtual void onVlcMlEvent( const MLEvent &event ) override;

    struct Loader : public BaseLoader
    {
        Loader(const MLUrlModel &model) : BaseLoader(model) {}
        size_t count(vlc_medialibrary_t* ml) const override;
        std::vector<std::unique_ptr<MLItem>> load(vlc_medialibrary_t* ml, size_t index, size_t count) const override;
        std::unique_ptr<MLItem> loadItemById(vlc_medialibrary_t* ml, MLItemId itemId) const override;
    };
};

#endif // MLURLMODEL_H
