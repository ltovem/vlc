/* Copyright (C)2012 Xiph.Org Foundation
   File: opus_header.h

   SPDX-License-Identifier: BSD-2-Clause
*/

#ifndef OPUS_HEADER_H
#define OPUS_HEADER_H

#include <stdint.h>

typedef struct {
    int version;
    int channels; /* Number of channels: 1..255 */
    int preskip;
    uint32_t input_sample_rate;
    int gain; /* in dB S7.8 should be zero whenever possible */
    uint8_t channel_mapping;
    /* The rest is only used if channel_mapping != 0 */
    int nb_streams;
    int nb_coupled;
    unsigned char stream_map[255];
    size_t dmatrix_size;
    unsigned char *dmatrix;
} OpusHeader;

void opus_header_init(OpusHeader *);
void opus_header_clean(OpusHeader *);
int opus_header_parse(const unsigned char *header, int len, OpusHeader *h);
void opus_prepare_header(unsigned channels, unsigned rate, OpusHeader *header);
int opus_write_header(uint8_t **p_extra, int *i_extra, OpusHeader *header, const char *vendor);

#endif
