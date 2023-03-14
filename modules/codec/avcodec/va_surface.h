/*****************************************************************************
 * va_surface.h: libavcodec Generic Video Acceleration helpers
 *****************************************************************************
 * Copyright (C) 2009 Geoffroy Couprie
 * Copyright (C) 2009 Laurent Aimar
 * Copyright (C) 2015 Steve Lhomme
 *
 * Authors: Geoffroy Couprie <geal@videolan.org>
 *          Laurent Aimar <fenrir _AT_ videolan _DOT_ org>
 *          Steve Lhomme <robux4@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef AVCODEC_VA_SURFACE_INTERNAL_H
#define AVCODEC_VA_SURFACE_INTERNAL_H

#include <libavcodec/avcodec.h>
#include "va.h"

#ifdef __cplusplus
extern "C" {
#endif

/* */
typedef struct vlc_va_surface_t vlc_va_surface_t;
typedef struct va_pool_t va_pool_t;

#define MAX_SURFACE_COUNT (64)

struct va_pool_cfg {
    int (*pf_create_device)(vlc_va_t *);
    /**
     * Destroy resources allocated with the surfaces and the associated decoder
     */
    void (*pf_destroy_device)(void *opaque);

    /**
     * Create the DirectX surfaces in hw_surface and the decoder in decoder
     */
    int (*pf_create_decoder_surfaces)(vlc_va_t *, int codec_id,
                                      const video_format_t *fmt,
                                      size_t surface_count);
    /**
     * Set the avcodec hw context after the decoder is created
     */
    void (*pf_setup_avcodec_ctx)(void *opaque, AVCodecContext *avctx);

    void *opaque;
};

/**
 * Create a VA pool for the given va module.
 *
 * The pf_create_device callback of the pool configuration is called. If it
 * fails, the pool is not created.
 */
va_pool_t * va_pool_Create(vlc_va_t *, const struct va_pool_cfg *);

/**
 * Close the VA pool.
 *
 * If there is no in-flight surfaces left, the pf_destroy_device callback of the
 * pool configuration is called and the pool is released.
 * If threre are in-flight pictures the last surface that calls va_surface_Release
 * will trigger the same behaviour to fully release the pool.
 */
void va_pool_Close(va_pool_t *);

/**
 * Setup the VA pool to allocate the amount of surfaces for the given format.
 *
 * The pf_create_decoder_surfaces callback of the pool configuration is called.
 * If it succeeds, the pf_setup_avcodec_ctx callback will be called afterwards.
 */
int va_pool_SetupDecoder(vlc_va_t *, va_pool_t *, AVCodecContext *, const video_format_t *, size_t count);

/**
 * Get a reference to an available surface or NULL on timeout
 */
vlc_va_surface_t *va_pool_Get(va_pool_t *);

/**
 * Get the index of the va_surface in the pool.
 */
size_t va_surface_GetIndex(const vlc_va_surface_t *surface);

/**
 * Add a reference to the surface.
 */
void va_surface_AddRef(vlc_va_surface_t *surface);

/**
 * Remove a reference to the surface.
 *
 * If the pool has been closed and it was the last reference used from the pool,
 * the pool we be released.
 *
 * \see va_pool_Close()
 */
void va_surface_Release(vlc_va_surface_t *surface);

#ifdef __cplusplus
}
#endif

#endif /* AVCODEC_VA_SURFACE_INTERNAL_H */
