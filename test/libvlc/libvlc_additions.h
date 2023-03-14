/*
 * libvlc_additions.c - Some helper method that should probably go into libvlc
 */

/**********************************************************************
 *  Copyright (C) 2007 Rémi Denis-Courmont.                           *
 *  SPDX-License-Identifier: GPL-2.0-or-later                         *
 **********************************************************************/

static void* media_list_add_file_path(libvlc_media_list_t *ml, const char * file_path)
{
    libvlc_media_t *md = libvlc_media_new_location(file_path);
    libvlc_media_list_add_media (ml, md);
    libvlc_media_release (md);
    return md;
}

