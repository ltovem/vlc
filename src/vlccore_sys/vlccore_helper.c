/*****************************************************************************
 * vlccore_helper.c : vlccore_sys C function helper
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

#include <vlc_stream.h>
#include <stdarg.h>
#include <vlc_demux.h>
#include "vlccore_helper.h"
#include "vlccore_sys/vlccore_sys.h"

#define static_control_match(foo) \
    static_assert((unsigned) DEMUX_##foo == STREAM_##foo, "Mismatch")

VLC_API int vlcrs_demux_vaControlHelper( stream_t *s,
        int query, va_list args )
{
    void *sys = s->p_sys;
    double  *pf;

    va_list cp_args;
    va_copy(cp_args, args);
    int ret = VLC_ENOOBJ;

    int64_t bitrate = vlcrs_demux_get_bitrate(sys);
    int align = vlcrs_demux_get_align(sys);
    int64_t start = vlcrs_demux_get_start(sys);
    int64_t end = vlcrs_demux_get_end(sys);
    if( end < 0 )    end   = stream_Size( s );
    if( start < 0 )  start = 0;
    if( align <= 0 ) align = 1;



    static_control_match(CAN_PAUSE);
    static_control_match(CAN_CONTROL_PACE);
    static_control_match(GET_PTS_DELAY);
    static_control_match(GET_META);
    static_control_match(GET_SIGNAL);
    static_control_match(SET_PAUSE_STATE);


    switch( query )
    {
        case DEMUX_CAN_SEEK:
            {
                bool *b = va_arg( args, bool * );
                ret = vlcrs_demux_control_can_seek(s, sys, b);
                break;
            }
        case DEMUX_GET_LENGTH:
            {
                vlc_tick_t *tick = *va_arg( args, vlc_tick_t * );
                ret = vlcrs_demux_control_get_length(s, sys, tick);
                break;
            }
        case DEMUX_GET_POSITION:
            {
                pf = va_arg( args, double * );
                if (start < end)
                    ret = vlcrs_demux_control_get_position(s, sys, pf);
                break;
            }
        default:
            ret = VLC_ENOOBJ;
    }
    if (ret == VLC_ENOOBJ)
        ret = demux_vaControlHelper(s->s, start, end, bitrate, align,
                                query, cp_args);
    va_end(cp_args);
    return ret;
}
