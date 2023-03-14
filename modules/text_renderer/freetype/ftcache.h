/*****************************************************************************
 * ftcache.h : Font Face and glyph cache freetype2
 *****************************************************************************
 * Copyright (C) 2020 - VideoLabs, VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifndef FTCACHE_H
#define FTCACHE_H

#include FT_CACHE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct vlc_ftcache_t vlc_ftcache_t;

typedef struct
{
    char *psz_filename;
    unsigned idx;
    unsigned int charmap_index;

} vlc_face_id_t;

vlc_ftcache_t * vlc_ftcache_New( vlc_object_t *, FT_Library, unsigned );
void vlc_ftcache_Delete( vlc_ftcache_t * );

/* Glyphs managed by the cache. Always use vlc_ftcache_Init/Release */
typedef struct
{
    FT_Glyph p_glyph;
    FTC_Node ref;
} vlc_ftcache_glyph_t;

void vlc_ftcache_Glyph_Init( vlc_ftcache_glyph_t * );
void vlc_ftcache_Glyph_Release( vlc_ftcache_t *, vlc_ftcache_glyph_t * );

vlc_face_id_t * vlc_ftcache_GetFaceID( vlc_ftcache_t *, const char *psz_fontfile, int i_idx );

typedef struct
{
    int width_px;
    int height_px;
} vlc_ftcache_metrics_t;

vlc_face_id_t * vlc_ftcache_GetActiveFaceInfo( vlc_ftcache_t *, vlc_ftcache_metrics_t * );
int vlc_ftcache_GetGlyphForCurrentFace( vlc_ftcache_t *, FT_UInt charmap_index,
                                        vlc_ftcache_glyph_t *, FT_Long * );
FT_UInt vlc_ftcache_LookupCMapIndex( vlc_ftcache_t *, vlc_face_id_t *faceid, FT_UInt codepoint );
/* Big fat warning : Do not store FT_Face.
 * Faces are fully managed by cache and possibly invalidated when changing face */
FT_Face vlc_ftcache_LoadFaceByID( vlc_ftcache_t *, vlc_face_id_t *faceid,
                                  const vlc_ftcache_metrics_t * );
int vlc_ftcache_LoadFaceByIDNoSize( vlc_ftcache_t *ftcache, vlc_face_id_t *faceid );

/* Custom modified glyphs cache. Similar to native caching.
 * Stores and returns a refcounted copy of the original glyph. */
typedef struct vlc_ftcache_custom_glyph_ref_Rec_ * vlc_ftcache_custom_glyph_ref_t;
typedef struct
{
    FT_Glyph p_glyph;
    vlc_ftcache_custom_glyph_ref_t ref;
} vlc_ftcache_custom_glyph_t;

FT_Glyph vlc_ftcache_GetOutlinedGlyph( vlc_ftcache_t *ftcache, const vlc_face_id_t *faceid,
                                       FT_UInt index, const vlc_ftcache_metrics_t *,
                                       FT_Long style, int radius, const FT_Glyph sourceglyph,
                                       int(*createOutline)(FT_Glyph, FT_Glyph *, void *), void *,
                                       vlc_ftcache_custom_glyph_ref_t * );

void vlc_ftcache_Custom_Glyph_Init( vlc_ftcache_custom_glyph_t * );
void vlc_ftcache_Custom_Glyph_Release( vlc_ftcache_custom_glyph_t * );

#ifdef __cplusplus
}
#endif

#endif
