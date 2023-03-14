/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#include "mlgenre.hpp"

MLGenre::MLGenre(const vlc_ml_genre_t *_data )
    : MLItem( MLItemId( _data->i_id, VLC_ML_PARENT_GENRE ) )
    , m_name     ( QString::fromUtf8( _data->psz_name ) )
    , m_nbTracks ( (unsigned int)_data->i_nb_tracks )

{
    assert(_data);
}

QString MLGenre::getName() const
{
    return m_name;
}

unsigned int MLGenre::getNbTracks() const
{
    return m_nbTracks;
}
