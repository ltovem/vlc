/*****************************************************************************
 * vlc_decklink.h: Decklink Common includes
 *****************************************************************************
 * Copyright (C) 2018 LTN Global Communications
 *
 * Authors: Devin Heitmueller <dheitmueller@ltnglobal.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef VLC_DECKLINK_H
#define VLC_DECKLINK_H 1

/**
 * \file
 * This file defines Decklink portability macros and other functions
 */

#ifdef _WIN32
#include <initguid.h>
#endif
#include <DeckLinkAPI.h>

/* Portability code to deal with differences how the Blackmagic SDK
   handles strings on various platforms */
#if defined(__APPLE__)
#include <vlc_common.h>
#include <vlc_charset.h>
typedef CFStringRef decklink_str_t;
#define DECKLINK_STRDUP(s) FromCFString(s, kCFStringEncodingUTF8)
#define DECKLINK_FREE(s) CFRelease(s)
#define PRIHR  "X"
#elif defined(_WIN32)
#include <vlc_charset.h> // FromWide
typedef BSTR decklink_str_t;
#define DECKLINK_STRDUP(s) FromWide(s)
#define DECKLINK_FREE(s) SysFreeString(s)
#define PRIHR  "lX"

static inline IDeckLinkIterator *CreateDeckLinkIteratorInstance(void)
{
    IDeckLinkIterator *decklink_iterator;
    if (SUCCEEDED(CoCreateInstance(CLSID_CDeckLinkIterator, nullptr, CLSCTX_ALL,
                                   IID_PPV_ARGS (&decklink_iterator))))
        return decklink_iterator;
    return nullptr;
}

#else
typedef const char* decklink_str_t;
#define DECKLINK_STRDUP(s) strdup(s)
#define DECKLINK_FREE(s) free((void *) s)
#define PRIHR  "X"
#endif

#endif /* VLC_DECKLINK_H */

