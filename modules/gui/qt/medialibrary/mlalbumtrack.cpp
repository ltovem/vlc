// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *****************************************************************************/

#include <cassert>
#include "mlalbumtrack.hpp"
#include "mlhelper.hpp"

MLAlbumTrack::MLAlbumTrack(vlc_medialibrary_t* _ml, const vlc_ml_media_t *_data, QObject *_parent )
    : QObject( _parent )
    , MLItem       ( MLItemId( _data->i_id, VLC_ML_PARENT_UNKNOWN ) )
    , m_title      ( QString::fromUtf8( _data->psz_title ) )
    , m_trackNumber( _data->album_track.i_track_nb )
    , m_discNumber ( _data->album_track.i_disc_nb )
    , m_duration   ( _data->i_duration )
{
    assert( _data );
    assert( _data->i_type == VLC_ML_MEDIA_TYPE_AUDIO );

    for( const vlc_ml_file_t& file: ml_range_iterate<vlc_ml_file_t>( _data->p_files ) )
        if( file.i_type == VLC_ML_FILE_TYPE_MAIN )
        {
            //FIXME should we store every mrl
            m_mrl = QString::fromUtf8(file.psz_mrl);
            break;
        }

    for( const vlc_ml_thumbnail_t& thumbnail: _data->thumbnails )
        if( thumbnail.i_status == VLC_ML_THUMBNAIL_STATUS_AVAILABLE )
        {
            m_cover = QString::fromUtf8(thumbnail.psz_mrl);
            break;
        }

    if ( _data->album_track.i_album_id != 0 )
    {
        ml_unique_ptr<vlc_ml_album_t> album(vlc_ml_get_album(_ml, _data->album_track.i_album_id));
        if (album)
             m_albumTitle =  album->psz_title;
    }

    if ( _data->album_track.i_artist_id != 0 )
    {
        ml_unique_ptr<vlc_ml_artist_t> artist(vlc_ml_get_artist(_ml, _data->album_track.i_artist_id));
        if (artist)
             m_artist =  artist->psz_name;
    }
}

QString MLAlbumTrack::getTitle() const
{
    return m_title;
}

QString MLAlbumTrack::getAlbumTitle() const
{
    return m_albumTitle;
}

QString MLAlbumTrack::getArtist() const
{
    return m_artist;
}

QString MLAlbumTrack::getCover() const
{
    return m_cover;
}

unsigned int MLAlbumTrack::getTrackNumber() const
{
    return m_trackNumber;
}

unsigned int MLAlbumTrack::getDiscNumber() const
{
    return m_discNumber;
}

VLCTick MLAlbumTrack::getDuration() const
{
    return VLCTick::fromMS(m_duration);
}

QString MLAlbumTrack::getMRL() const
{
    return m_mrl;
}
