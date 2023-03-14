/*****************************************************************************
 * importer.h
 *****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef VLC_GL_IMPORTER_H
#define VLC_GL_IMPORTER_H

typedef struct picture_t picture_t;

/**
 * An importer uses an interop to convert picture_t to a valid vlc_gl_picture,
 * with all necessary transformations computed.
 */
struct vlc_gl_importer;
struct vlc_gl_interop;

struct vlc_gl_importer *
vlc_gl_importer_New(struct vlc_gl_interop *interop);

void
vlc_gl_importer_Delete(struct vlc_gl_importer *importer);

int
vlc_gl_importer_Update(struct vlc_gl_importer *importer, picture_t *picture);

#endif
