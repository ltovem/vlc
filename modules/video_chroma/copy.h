// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * copy.h: Fast YV12/NV12 copy
 *****************************************************************************
 * Copyright (C) 2009 Laurent Aimar
 *
 * Authors: Laurent Aimar <fenrir_AT_ videolan _DOT_ org>
 *****************************************************************************/

#ifndef VLC_VIDEOCHROMA_COPY_H_
#define VLC_VIDEOCHROMA_COPY_H_

#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
# ifdef CAN_COMPILE_SSE2
    uint8_t *buffer;
    size_t  size;
# else
    char dummy;
# endif
} copy_cache_t;

int  CopyInitCache(copy_cache_t *cache, unsigned width);
void CopyCleanCache(copy_cache_t *cache);

/* YUVY/RGB copies */
void CopyPacked(picture_t *dst, const uint8_t *src,
                const size_t src_pitch, unsigned height,
                const copy_cache_t *cache);

/* Copy planes from NV12/NV21 to NV12/NV21 */
void Copy420_SP_to_SP(picture_t *dst, const uint8_t *src[ARRAY_STATIC_SIZE 2],
                      const size_t src_pitch[ARRAY_STATIC_SIZE 2], unsigned height,
                      const copy_cache_t *cache);

/* Copy planes from I420/YV12 to I420/YV12 */
void Copy420_P_to_P(picture_t *dst, const uint8_t *src[ARRAY_STATIC_SIZE 3],
                    const size_t src_pitch[ARRAY_STATIC_SIZE 3], unsigned height,
                    const copy_cache_t *cache);

/* Copy planes from I420/YV12 to NV12/NV21 */
void Copy420_P_to_SP(picture_t *dst, const uint8_t *src[ARRAY_STATIC_SIZE 3],
                     const size_t src_pitch[ARRAY_STATIC_SIZE 3], unsigned height,
                     const copy_cache_t *cache);

/* Copy planes from NV12/NV21 to I420/YV12 */
void Copy420_SP_to_P(picture_t *dst, const uint8_t *src[ARRAY_STATIC_SIZE 2],
                     const size_t src_pitch[ARRAY_STATIC_SIZE 2], unsigned height,
                     const copy_cache_t *cache);

/* Copy planes from I420_10 to P010. A positive bitshift value will shift bits
 * to the right, a negative value will shift to the left. */
void Copy420_16_P_to_SP(picture_t *dst, const uint8_t *src[ARRAY_STATIC_SIZE 3],
                     const size_t src_pitch[ARRAY_STATIC_SIZE 3], unsigned height,
                     int bitshift, const copy_cache_t *cache);

/* Copy planes from P010 to I420_10. A positive bitshift value will shift bits
 * to the right, a negative value will shift to the left. */
void Copy420_16_SP_to_P(picture_t *dst, const uint8_t *src[ARRAY_STATIC_SIZE 2],
                        const size_t src_pitch[ARRAY_STATIC_SIZE 2], unsigned height,
                        int bitshift, const copy_cache_t *cache);

/**
 * This functions sets the internal plane pointers/dimensions for the given
 * buffer.
 * This is useful when mapping opaque surfaces into CPU planes.
 *
 * picture is the picture to update
 * data is the buffer pointer to use as the start of data for all the planes
 * pitch is the internal line pitch for the buffer
 */
int picture_UpdatePlanes(picture_t *picture, uint8_t *data, unsigned pitch);

#ifdef __cplusplus
}
#endif

#endif
