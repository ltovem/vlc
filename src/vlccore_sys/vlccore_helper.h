/*****************************************************************************
 * vlccore_helper.h : vlccore_sys C function helper
 *****************************************************************************
 * Copyright © 2006-2021 the VideoLAN team
 *
 * Author: Johan 'Enulp' Pardo
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#include <vlc_demux.h>
#include <stdarg.h>

VLC_API int vlcrs_demux_vaControlHelper( demux_t *demux,
        int query, va_list args );
