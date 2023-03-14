/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *
 * Authors: Benjamin Arnaud <bunjee@omega.gg>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#include "mlgroup.hpp"

// VLC includes
#include "qt.hpp"

//-------------------------------------------------------------------------------------------------
// Ctor / dtor
//-------------------------------------------------------------------------------------------------

MLGroup::MLGroup(const vlc_ml_group_t * data)
    : MLItem(MLItemId(data->i_id, VLC_ML_PARENT_GROUP))
    , m_title(qfu(data->psz_name))
    , m_duration(data->i_duration)
    , m_date(data->i_creation_date)
    , m_count(data->i_nb_total_media)
{
    assert(data);
}

//-------------------------------------------------------------------------------------------------
// Interface
//-------------------------------------------------------------------------------------------------

QString MLGroup::getTitle() const
{
    return m_title;
}

//-------------------------------------------------------------------------------------------------

VLCTick MLGroup::getDuration() const
{
    return VLCTick::fromMS(m_duration);
}

unsigned int MLGroup::getDate() const
{
    return m_date;
}

unsigned int MLGroup::getCount() const
{
    return m_count;
}
