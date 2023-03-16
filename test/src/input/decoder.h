// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * decoder.c
 *****************************************************************************
 * Copyright (C) 2017 VLC authors and VideoLAN
 *
 * Authors: Shaleen Jain <shaleen.jain95@gmail.com>
 *****************************************************************************/

decoder_t *test_decoder_create(vlc_object_t *parent, const es_format_t *fmt);
void test_decoder_destroy(decoder_t *decoder);
int test_decoder_process(decoder_t *decoder, block_t *block);
