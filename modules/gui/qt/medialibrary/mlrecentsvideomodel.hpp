/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef ML_RECENTS_VIDEO_MODEL_H
#define ML_RECENTS_VIDEO_MODEL_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "mlvideomodel.hpp"

class MLRecentsVideoModel : public MLVideoModel
{
    Q_OBJECT

    Q_PROPERTY(int numberOfItemsToShow READ getNumberOfItemsToShow
               WRITE setNumberOfItemsToShow FINAL)

public:
    explicit MLRecentsVideoModel(QObject * parent = nullptr);

    virtual ~MLRecentsVideoModel() = default;

protected: // MLBaseModel implementation
    std::unique_ptr<MLBaseModel::BaseLoader> createLoader() const override;

private: // Functions
    int  getNumberOfItemsToShow();
    void setNumberOfItemsToShow(int number);

protected: // MLVideoModel reimplementation
    void onVlcMlEvent(const MLEvent & event) override;

private: // Variables
    int m_numberOfItemsToShow = 10;

private:
    struct Loader : public BaseLoader
    {
        Loader(const MLRecentsVideoModel & model, int numberOfItemsToShow);

        size_t count(vlc_medialibrary_t* ml) const override;

        std::vector<std::unique_ptr<MLItem>> load(vlc_medialibrary_t* ml, size_t index, size_t count) const override;

        std::unique_ptr<MLItem> loadItemById(vlc_medialibrary_t* ml, MLItemId itemId) const override;
    private:
        int m_numberOfItemsToShow;
    };
};

#endif // ML_RECENTS_VIDEO_MODEL_H
