/*****************************************************************************
 * avfilter.c : avfilter audio emphasis filter
 *****************************************************************************
 * Copyright © 2022 VLC authors, VideoLAN and VideoLabs
 *
 * Authors: Steve Lhomme <robux4@videolabs.io>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

/*****************************************************************************
 * Preamble
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>

/*****************************************************************************
 * Module descriptor
 *****************************************************************************/
int  avemphasis_Create ( vlc_object_t * );

#define AVEMPHASIS_MODULE \
    set_description( N_("Nearest-neighbor audio resampler") ) \
    set_capability( "audio converter", 2 )                    \
    set_subcategory( SUBCAT_AUDIO_EMPHASIS )                  \
    set_callback( avemphasis_Create )
