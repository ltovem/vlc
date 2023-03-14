/*****************************************************************************
 * heif.h : ISO/IEC 23008-12 HEIF still picture demuxer
 *****************************************************************************
 * Copyright (C) 2018 Videolabs, VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#define HEIF_DEFAULT_DURATION 10.0

int  OpenHEIF ( vlc_object_t * );
void CloseHEIF( vlc_object_t * );
