// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * iso_lang.c: function to decode language code (in dvd or a52 for instance).
 *****************************************************************************
 * Copyright (C) 1998-2004 VLC authors and VideoLAN
 *
 * Author: Stéphane Borel <stef@via.ecp.fr>
 *         Arnaud de Bossoreille de Ribou <bozo@via.ecp.fr>
 *****************************************************************************/

/*****************************************************************************
 * Preamble
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdio.h>

#include <vlc_common.h>

#include "vlc_iso_lang.h"

/*****************************************************************************
 * Local tables
 *****************************************************************************/
#include "iso-639_def.h"

static const iso639_lang_t * GetLang_1( const char * psz_code )
{
    const iso639_lang_t *p_lang;

    for( p_lang = p_languages; p_lang->psz_eng_name; p_lang++ )
        if( !strcasecmp( p_lang->psz_iso639_1, psz_code ) )
            return p_lang;

    return NULL;
}

static const iso639_lang_t * GetLang_2T( const char * psz_code )
{
    const iso639_lang_t *p_lang;

    for( p_lang = p_languages; p_lang->psz_eng_name; p_lang++ )
        if( !strcasecmp( p_lang->psz_iso639_2T, psz_code ) )
            return p_lang;

    return NULL;
}

static const iso639_lang_t * GetLang_2B( const char * psz_code )
{
    const iso639_lang_t *p_lang;

    for( p_lang = p_languages; p_lang->psz_eng_name; p_lang++ )
        if( !strcasecmp( p_lang->psz_iso639_2B, psz_code ) )
            return p_lang;

    return NULL;
}

static const iso639_lang_t * GetLang_name( const char * psz_lang )
{
    const iso639_lang_t *p_lang;

    for( p_lang = p_languages; p_lang->psz_eng_name; p_lang++ )
        if( !strcasecmp( p_lang->psz_eng_name, psz_lang ) )
            return p_lang;

    return NULL;
}

const iso639_lang_t * vlc_find_iso639( const char *code, bool try_name )
{
    const iso639_lang_t *result = NULL;
    size_t len = strlen(code);

    if (len == 2)
        result = GetLang_1(code);
    else if (len == 3)
    {
        result = GetLang_2B(code);
        if (result == NULL)
            result = GetLang_2T(code);
    }

    if (try_name && !result && len != 0)
        result = GetLang_name(code);

    return result;
}
