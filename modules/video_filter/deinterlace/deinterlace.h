/*****************************************************************************
 * deinterlace.h : deinterlacer plugin for vlc
 *****************************************************************************
 * Copyright (C) 2000-2011 VLC authors and VideoLAN
 *
 * Author: Sam Hocevar <sam@zoy.org>
 *         Christophe Massiot <massiot@via.ecp.fr>
 *         Laurent Aimar <fenrir@videolan.org>
 *         Juha Jeronen <juha.jeronen@jyu.fi>
 *         ...and others
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef VLC_DEINTERLACE_H
#define VLC_DEINTERLACE_H 1

/* Forward declarations */
struct filter_t;
struct picture_t;
struct vlc_object_t;

#include <vlc_common.h>
#include <vlc_mouse.h>

/* Local algorithm headers */
#include "algo_basic.h"
#include "algo_x.h"
#include "algo_yadif.h"
#include "algo_phosphor.h"
#include "algo_ivtc.h"
#include "common.h"

/*****************************************************************************
 * Local data
 *****************************************************************************/

/** Available deinterlace modes. */
static const char *const mode_list[] = {
    "discard", "blend", "mean", "bob", "linear", "x",
    "yadif", "yadif2x", "phosphor", "ivtc" };

/** User labels for the available deinterlace modes. */
static const char *const mode_list_text[] = {
    N_("Discard"), N_("Blend"), N_("Mean"), N_("Bob"), N_("Linear"), "X",
    "Yadif", "Yadif (2x)", N_("Phosphor"), N_("Film NTSC (IVTC)") };

/*****************************************************************************
 * Data structures
 *****************************************************************************/

/**
 * Top-level deinterlace subsystem state.
 */
typedef struct
{
    const vlc_chroma_description_t *chroma;

    /** Merge routine: C, SSE, ALTIVEC, NEON, ... */
    void (*pf_merge) ( void *, const void *, const void *, size_t );
#if defined (__i386__) || defined (__x86_64__)
    /** Merge finalization routine for SSE */
    void (*pf_end_merge) ( void );
#endif

    struct deinterlace_ctx   context;

    /* Algorithm-specific substructures */
    union {
        phosphor_sys_t phosphor; /**< Phosphor algorithm state. */
        ivtc_sys_t ivtc;         /**< IVTC algorithm state. */
    };
} filter_sys_t;

#endif
