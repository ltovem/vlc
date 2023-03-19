// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * playlist.h:  Playlist import module common functions
 *****************************************************************************
 * Copyright (C) 2004 VLC authors and VideoLAN
 *
 * Authors: Sigmund Augdal Helberg <dnumgis@videolan.org>
 *****************************************************************************/

#include <vlc_input.h>

char *ProcessMRL( const char *, const char * );

int PlaylistControl( stream_t *p_access, int i_query, va_list args );

int Import_M3U ( vlc_object_t * );

int Import_RAM ( vlc_object_t * );

int Import_PLS ( vlc_object_t * );

int Import_B4S ( vlc_object_t * );

int Import_DVB ( vlc_object_t * );

int Import_podcast ( vlc_object_t * );

int Import_xspf ( vlc_object_t * );
void Close_xspf ( vlc_object_t * );

int Import_Shoutcast ( vlc_object_t * );

int Import_ASX ( vlc_object_t * );

int Import_SGIMB ( vlc_object_t * );
void Close_SGIMB ( vlc_object_t * );

int Import_QTL ( vlc_object_t * );

int Import_IFO ( vlc_object_t * );
void Close_IFO ( vlc_object_t * );

int Import_BDMV ( vlc_object_t * );
void Close_BDMV ( vlc_object_t * );

int Import_iTML ( vlc_object_t * );

int Import_WMS(vlc_object_t *);

int Import_WPL ( vlc_object_t * );
void Close_WPL ( vlc_object_t * );

#define GetCurrentItem(obj) ((obj)->p_input_item)
#define GetSource(obj) ((obj)->s)
