/*****************************************************************************
 * media_source.h
 *****************************************************************************
 * Copyright (C) 2018 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef MEDIA_SOURCE_H
#define MEDIA_SOURCE_H

#include <vlc_media_source.h>

vlc_media_source_provider_t *
vlc_media_source_provider_New(vlc_object_t *parent);
#define vlc_media_source_provider_New(obj) \
        vlc_media_source_provider_New(VLC_OBJECT(obj))

void vlc_media_source_provider_Delete(vlc_media_source_provider_t *msp);

#endif
