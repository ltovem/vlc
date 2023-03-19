// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * extradata.h: Muxing extradata builder/gatherer
 *****************************************************************************
 * Copyright (C) 2018 VideoLabs, VLC authors and VideoLAN
 *****************************************************************************/

typedef struct mux_extradata_builder_t mux_extradata_builder_t;

enum mux_extradata_type_e
{
    EXTRADATA_ISOBMFF,
};

mux_extradata_builder_t * mux_extradata_builder_New(vlc_object_t *obj,
                                                    vlc_fourcc_t,
                                                    enum mux_extradata_type_e);
void mux_extradata_builder_Delete(mux_extradata_builder_t *);
void mux_extradata_builder_Feed(mux_extradata_builder_t *, const uint8_t *, size_t);
size_t mux_extradata_builder_Get(mux_extradata_builder_t *, const uint8_t **);
