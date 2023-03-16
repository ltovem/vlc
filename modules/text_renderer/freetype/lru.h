// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * lru.h : Last recently used cache implementation
 *****************************************************************************
 * Copyright (C) 2020 - VideoLabs, VLC authors and VideoLAN
 *****************************************************************************/
#ifndef LRU_H
#define LRU_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct vlc_lru vlc_lru;

vlc_lru * vlc_lru_New( unsigned max,
                       void(*releaseValue)(void *, void *), void * );
void vlc_lru_Release( vlc_lru *lru );

bool   vlc_lru_HasKey( vlc_lru *lru, const char *psz_key );
void * vlc_lru_Get( vlc_lru *lru, const char *psz_key );
void   vlc_lru_Insert( vlc_lru *lru, const char *psz_key, void *value );

void   vlc_lru_Apply( vlc_lru *lru,
                      void(*func)(void *, const char *, void *),
                      void * );

#ifdef __cplusplus
}
#endif

#endif
