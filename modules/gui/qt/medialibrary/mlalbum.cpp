// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *****************************************************************************/
#include <cassert>
#include "mlalbum.hpp"

MLAlbum::MLAlbum(const vlc_ml_album_t *_data, QObject *_parent)
    : QObject( _parent )
    , MLItem        ( MLItemId( _data->i_id, VLC_ML_PARENT_ALBUM ) )
    , m_title       ( QString::fromUtf8( _data->psz_title ) )
    , m_releaseYear ( _data->i_year )
    , m_shortSummary( QString::fromUtf8( _data->psz_summary ) )
    , m_cover       ( QString::fromUtf8( _data->thumbnails[VLC_ML_THUMBNAIL_SMALL].psz_mrl ) )
    , m_mainArtist  ( QString::fromUtf8( _data->psz_artist ) )
    , m_nbTracks    ( _data->i_nb_tracks )
    , m_duration    ( _data->i_duration )
{
    assert( _data );
}

QString MLAlbum::getTitle() const
{
    return m_title;
}

unsigned int MLAlbum::getReleaseYear() const
{
    return  m_releaseYear;
}

QString MLAlbum::getShortSummary() const
{
    return m_shortSummary;
}

QString MLAlbum::getCover() const
{
    return m_cover;
}


QString MLAlbum::getArtist() const
{
    return m_mainArtist;
}

unsigned int MLAlbum::getNbTracks() const
{
    return m_nbTracks;
}

VLCTick MLAlbum::getDuration() const
{
    return VLCTick::fromMS(m_duration);
}

QString MLAlbum::getPresName() const
{
    return m_title;
}

QString MLAlbum::getPresImage() const
{
    return m_cover;
}

QString MLAlbum::getPresInfo() const
{
    return m_shortSummary;
}


