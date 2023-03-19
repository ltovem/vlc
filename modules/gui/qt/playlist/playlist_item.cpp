// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *****************************************************************************/
#include "playlist_item.hpp"

//namespace vlc {
//namespace playlist {

PlaylistItem::PlaylistItem(vlc_playlist_item_t* item)
{
    d = new Data();
    if (item)
    {
        d->item.reset(item);
        sync();
    }
}

bool PlaylistItem::isSelected() const
{
    return d->selected;
}

void PlaylistItem::setSelected(bool selected)
{
    d->selected = selected;
}

QString PlaylistItem::getTitle() const
{
    return d->title;
}

QString PlaylistItem::getArtist() const
{
    return d->artist;
}

QString PlaylistItem::getAlbum() const
{
    return d->album;
}

QUrl PlaylistItem::getArtwork() const
{
    return d->artwork;
}

vlc_tick_t PlaylistItem::getDuration() const
{
    return d->duration;
}

QUrl PlaylistItem::getUrl() const
{
    return d->url;
}

void PlaylistItem::sync() {
    input_item_t *media = vlc_playlist_item_GetMedia(d->item.get());
    vlc_mutex_locker locker(&media->lock);
    d->duration = media->i_duration;
    d->url      = media->psz_uri;

    if (media->p_meta) {
        d->title   = vlc_meta_Get(media->p_meta, vlc_meta_Title);
        d->artist  = vlc_meta_Get(media->p_meta, vlc_meta_Artist);
        d->album   = vlc_meta_Get(media->p_meta, vlc_meta_Album);
        d->artwork = vlc_meta_Get(media->p_meta, vlc_meta_ArtworkURL);
    }

    if (d->title.isNull())
        /* If there is no title, use the item name */
        d->title = media->psz_name;
}

PlaylistItem::operator bool() const
{
    return d && d->item.get();
}


//}
//}
