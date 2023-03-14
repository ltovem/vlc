/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#include <cassert>
#include "mlartist.hpp"

MLArtist::MLArtist(const vlc_ml_artist_t* _data, QObject *_parent)
    : QObject(_parent)
    , MLItem    ( MLItemId( _data->i_id, VLC_ML_PARENT_ARTIST ) )
    , m_name    ( QString::fromUtf8( _data->psz_name ) )
    , m_shortBio( QString::fromUtf8( _data->psz_shortbio ) )
    , m_cover   ( QString::fromUtf8( _data->thumbnails[VLC_ML_THUMBNAIL_SMALL].psz_mrl ) )
    , m_nbAlbums( _data->i_nb_album )
    , m_nbTracks( _data->i_nb_tracks )
{
    assert( _data );
}

QString MLArtist::getName() const
{
    return m_name;
}

QString MLArtist::getShortBio() const
{
    return m_shortBio;
}

QString MLArtist::getCover() const
{
    return m_cover;
}

unsigned int MLArtist::getNbAlbums() const
{
    return m_nbAlbums;
}


unsigned int MLArtist::getNbTracks() const
{
    return m_nbTracks;
}

QString MLArtist::getPresName() const
{
    return m_name;
}

QString MLArtist::getPresImage() const
{
    return m_cover;
}

QString MLArtist::getPresInfo() const
{
    return m_shortBio;
}

