// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * filters.h: VAAPI filters helper for VLC
 *****************************************************************************
 * Copyright (C) 2017 VLC authors, VideoLAN and VideoLabs
 *
 * Authors: Thomas Guillem <thomas@gllm.fr>
 *          Petri Hintukainen <phintuka@gmail.com>
 *          Victorien Le Couviour--Tuffet <victorien.lecouviour.tuffet@gmail.com>
 *****************************************************************************/

#ifndef VLC_VAAPI_FILTERS_H
# define VLC_VAAPI_FILTERS_H

#include "vlc_vaapi.h"

/* chroma filters callbacks */
int  vlc_vaapi_OpenChroma(filter_t *);
void vlc_vaapi_CloseChroma(filter_t *);

#endif /* VLC_VAAPI_FILTERS_H */
