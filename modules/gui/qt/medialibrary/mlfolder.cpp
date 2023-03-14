/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *
 * Authors: Benjamin Arnaud <bunjee@omega.gg>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#include "mlfolder.hpp"

// Ctor / dtor

MLFolder::MLFolder(const vlc_ml_folder_t * data)
    : MLItem(MLItemId(data->i_id, VLC_ML_PARENT_FOLDER))
    , m_present(data->b_present)
    , m_banned(data->b_banned)
    , m_title(data->psz_name)
    , m_mrl(data->psz_mrl)
    , m_duration(0) // FIXME: We should have a duration field in vlc_ml_folder_t.
    , m_count(data->i_nb_media)
    , m_audioCount(data->i_nb_audio)
    , m_videoCount(data->i_nb_video)
{}

// Interface

bool MLFolder::isPresent() const
{
    return m_present;
}

bool MLFolder::isBanned() const
{
    return m_banned;
}

QString MLFolder::getTitle() const
{
    return m_title;
}

QString MLFolder::getMRL() const
{
    return m_mrl;
}

VLCTick MLFolder::getDuration() const
{
    return VLCTick::fromMS(m_duration);
}

unsigned int MLFolder::getCount() const
{
    return m_count;
}

unsigned int MLFolder::getAudioCount() const
{
    return m_audioCount;
}

unsigned int MLFolder::getVideoCount() const
{
    return m_videoCount;
}
