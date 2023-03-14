/*****************************************************************************
 * chroma_neon.h
 *****************************************************************************
 * Copyright (C) 2011 Rémi Denis-Courmont
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

/* Planes must start on a 16-bytes boundary. Pitches must be multiples of 16
 * bytes even for subsampled components. */

/* Planar picture buffer.
 * Pitch corresponds to luminance component in bytes. Chrominance pitches are
 * inferred from the color subsampling ratio. */
struct yuv_planes
{
    void *y, *u, *v;
    size_t pitch;
};

/* Planar chroma buffers.
 * Pitch is in bytes. */
struct uv_planes
{
    void *u, *v;
    size_t pitch;
};

/* Packed picture buffer. Pitch is in bytes (_not_ pixels). */
struct yuv_pack
{
    void *yuv;
    size_t pitch;
};

/* I420 to YUYV conversion. */
void i420_yuyv_neon (struct yuv_pack *const out,
                     const struct yuv_planes *const in,
                     int width, int height);

/* I420 to UYVY conversion. */
void i420_uyvy_neon (struct yuv_pack *const out,
                     const struct yuv_planes *const in,
                     int width, int height);

/* I422 to YUYV conversion. */
void i422_yuyv_neon (struct yuv_pack *const out,
                     const struct yuv_planes *const in,
                     int width, int height);

/* I422 to UYVY conversion. */
void i422_uyvy_neon (struct yuv_pack *const out,
                     const struct yuv_planes *const in,
                     int width, int height);

/* YUYV to I422 conversion. */
void yuyv_i422_neon (struct yuv_planes *const out,
                     const struct yuv_pack *const in,
                     int width, int height);

/* UYVY to I422 conversion. */
void uyvy_i422_neon (struct yuv_planes *const out,
                     const struct yuv_pack *const in,
                     int width, int height);

/* Semiplanar to planar conversion. */
void deinterleave_chroma_neon (struct uv_planes *const out,
                               const struct yuv_pack *const in,
                               int width, int height);

/* I420 to RGBA conversion. */
void i420_rgb_neon (struct yuv_pack *const out,
                    const struct yuv_planes *const in,
                    int width, int height);

/* I420 to RV16 conversion. */
void i420_rv16_neon (struct yuv_pack *const out,
                     const struct yuv_planes *const in,
                     int width, int height);

/* NV21 to RGBA conversion. */
void nv21_rgb_neon (struct yuv_pack *const out,
                    const struct yuv_planes *const in,
                    int width, int height);

/* NV12 to RGBA conversion. */
void nv12_rgb_neon (struct yuv_pack *const out,
                    const struct yuv_planes *const in,
                    int width, int height);
