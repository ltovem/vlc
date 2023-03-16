// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * mms.h: MMS access plug-in
 *****************************************************************************
 * Copyright (C) 2001, 2002 VLC authors and VideoLAN
 *
 * Authors: Laurent Aimar <fenrir@via.ecp.fr>
 *****************************************************************************/

#ifndef VLC_MMS_MMS_H_
#define VLC_MMS_MMS_H_

#define MMS_PROTO_AUTO  0
#define MMS_PROTO_TCP   1
#define MMS_PROTO_UDP   2
#define MMS_PROTO_HTTP  3

/* mmst and mmsu */
int   MMSTUOpen   ( stream_t * );
void  MMSTUClose  ( stream_t * );

/* mmsh */
int   MMSHOpen   ( stream_t * );
void  MMSHClose  ( stream_t * );

#endif

