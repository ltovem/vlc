/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#include "mlplaylist.hpp"

// VLC includes
#include "qt.hpp"

//-------------------------------------------------------------------------------------------------
// Ctor / dtor
//-------------------------------------------------------------------------------------------------

MLPlaylist::MLPlaylist(const vlc_ml_playlist_t * data)
    : MLItem(MLItemId(data->i_id, VLC_ML_PARENT_PLAYLIST))
    , m_name(qfu(data->psz_name))
    , m_duration(0) // TODO m_duration
    , m_count(data->i_nb_media)
{
    assert(data);
}

//-------------------------------------------------------------------------------------------------
// Interface
//-------------------------------------------------------------------------------------------------

QString MLPlaylist::getName() const
{
    return m_name;
}

//-------------------------------------------------------------------------------------------------

VLCTick MLPlaylist::getDuration() const
{
    return VLCTick::fromMS(m_duration);
}

unsigned int MLPlaylist::getCount() const
{
    return m_count;
}
