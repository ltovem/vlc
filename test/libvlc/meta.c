// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * meta.c - libvlc smoke test
 *
 */

/**********************************************************************
 *  Copyright (C) 2007 Rémi Denis-Courmont.                           *
 *  Copyright (C) 2008 Pierre d'Herbemont.                            *
 **********************************************************************/

#include <string.h>

#include "test.h"
#include "media_utils.h"

static void test_meta (const char ** argv, int argc)
{
    libvlc_instance_t *vlc;
    libvlc_media_t *media;
    char * artist;

    test_log ("Testing meta\n");

    vlc = libvlc_new (argc, argv);
    assert (vlc != NULL);

    media = libvlc_media_new_path(SRCDIR "/samples/meta.mp3");
    assert( media );

    libvlc_media_parse_sync(vlc, media, libvlc_media_parse_local, -1);

    artist = libvlc_media_get_meta (media, libvlc_meta_Artist);

    const char *expected_artist = "mike";

    assert (artist);
    test_log ("+ got '%s' as Artist, expecting %s\n", artist, expected_artist);

    int string_compare = strcmp (artist, expected_artist);
    assert (!string_compare);

    free (artist);
    libvlc_media_release (media);
    libvlc_release (vlc);
}


int main (void)
{
    test_init();

    test_meta (test_defaults_args, test_defaults_nargs);

    return 0;
}
