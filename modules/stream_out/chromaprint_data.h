// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * chromaprint_data.h: Chromaprint's sout fingerprint data header
 *****************************************************************************
 * Copyright (C) 2012 VLC authors and VideoLAN
 *****************************************************************************/

struct chromaprint_fingerprint_t
{
    char *psz_fingerprint;
    unsigned int i_duration;
};

typedef struct chromaprint_fingerprint_t chromaprint_fingerprint_t;
