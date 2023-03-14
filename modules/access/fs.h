/*****************************************************************************
 * fs.h: file system access plug-in common header
 *****************************************************************************
 * Copyright (C) 2009 Rémi Denis-Courmont
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#include <vlc_fs.h>

int FileOpen (vlc_object_t *);
void FileClose (vlc_object_t *);

int DirOpen (vlc_object_t *);
int DirInit (stream_t *p_access, vlc_DIR *handle);
void DirClose (vlc_object_t *);
