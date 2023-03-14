/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#include "mlrecentsvideomodel.hpp"
#include "mlhelper.hpp"

// Ctor / dtor

MLRecentsVideoModel::MLRecentsVideoModel(QObject * parent) : MLVideoModel(parent) {}

// Protected MLBaseModel implementation

std::unique_ptr<MLBaseModel::BaseLoader>
MLRecentsVideoModel::createLoader() const
/* override */
{
    return std::make_unique<Loader>(*this, m_numberOfItemsToShow);
}

// Private functions

int MLRecentsVideoModel::getNumberOfItemsToShow()
{
    return m_numberOfItemsToShow;
}

void MLRecentsVideoModel::setNumberOfItemsToShow(int number)
{
    m_numberOfItemsToShow = number;

    invalidateCache();
}

// Private MLVideoModel reimplementation

void MLRecentsVideoModel::onVlcMlEvent(const MLEvent & event) /* override */
{
    switch (event.i_type)
    {
        case VLC_ML_EVENT_HISTORY_CHANGED:
            emit resetRequested();
            return;
        default:
            break;
    }

    MLVideoModel::onVlcMlEvent(event);
}

// Loader

MLRecentsVideoModel::Loader::Loader(const MLRecentsVideoModel & model, int numberOfItemsToShow)
    : MLBaseModel::BaseLoader(model)
    , m_numberOfItemsToShow(numberOfItemsToShow)
{}

size_t MLRecentsVideoModel::Loader::count(vlc_medialibrary_t* ml) const /* override */
{
    MLQueryParams params = getParams();

    auto queryParams = params.toCQueryParams();

    size_t realCount = vlc_ml_count_history_by_type(ml, &queryParams, VLC_ML_MEDIA_TYPE_VIDEO);

    if (m_numberOfItemsToShow >= 0)
    {
        return std::min(realCount, static_cast<size_t> (m_numberOfItemsToShow));
    }

    return realCount;
}

std::vector<std::unique_ptr<MLItem>>
MLRecentsVideoModel::Loader::load(vlc_medialibrary_t* ml, size_t index, size_t count) const /* override */
{
    MLQueryParams params = getParams(index, count);

    auto queryParams = params.toCQueryParams();

    std::vector<std::unique_ptr<MLItem>> res;

    if (m_numberOfItemsToShow >= 0)
    {
        uint32_t count = static_cast<uint32_t> (m_numberOfItemsToShow);

        if (queryParams.i_offset > count)
            return res;

        queryParams.i_nbResults = count - queryParams.i_offset;
    }

    ml_unique_ptr<vlc_ml_media_list_t> media_list
    {
        vlc_ml_list_history_by_type(ml, &queryParams, VLC_ML_MEDIA_TYPE_VIDEO)
    };

    if (media_list == nullptr)
        return {};

    for (const vlc_ml_media_t & media : ml_range_iterate<vlc_ml_media_t>(media_list))
    {
        res.emplace_back( std::make_unique<MLVideo>(&media));
    }

    return res;
}

std::unique_ptr<MLItem>
MLRecentsVideoModel::Loader::loadItemById(vlc_medialibrary_t* ml, MLItemId itemId) const
{
    assert(itemId.type == VLC_ML_PARENT_UNKNOWN);
    ml_unique_ptr<vlc_ml_media_t> media(vlc_ml_get_media(ml, itemId.id));
    if (!media)
        return nullptr;
    return std::make_unique<MLVideo>(media.get());
}
