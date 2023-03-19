// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * ts_metadata.h : TS demuxer metadata handling
 *****************************************************************************
 * Copyright (C) 2016 - VideoLAN Authors
 *****************************************************************************/
#ifndef VLC_TS_METADATA_H
#define VLC_TS_METADATA_H

#define METADATA_IDENTIFIER_ID3  VLC_FOURCC('I','D','3',' ')

ts_stream_processor_t *Metadata_stream_processor_New( ts_stream_t *, es_out_t * );

#endif
