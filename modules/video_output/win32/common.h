// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * common.h: Windows video output header file
 *****************************************************************************
 * Copyright (C) 2001-2009 VLC authors and VideoLAN
 *
 * Authors: Gildas Bazin <gbazin@videolan.org>
 *          Damien Fouilleul <damienf@videolan.org>
 *          Martell Malone <martellmalone@gmail.com>
 *****************************************************************************/

#include <vlc_vout_display.h>

#ifdef __cplusplus
extern "C" {
#endif// __cplusplus

/*****************************************************************************
 * event_thread_t: event thread
 *****************************************************************************/

typedef struct event_thread_t event_thread_t;

typedef struct display_win32_area_t
{
    /* Coordinates of dest images (used when blitting to display) */
    vout_display_place_t  place;
    bool                  place_changed;
} display_win32_area_t;

#define RECTWidth(r)   (LONG)((r).right - (r).left)
#define RECTHeight(r)  (LONG)((r).bottom - (r).top)

/*****************************************************************************
 * vout_sys_t: video output method descriptor
 *****************************************************************************
 * This structure is part of the video output thread descriptor.
 * It describes the module specific properties of an output thread.
 *****************************************************************************/
typedef struct vout_display_sys_win32_t
{
    /* */
    event_thread_t *event;

    /* */
    HWND                 hvideownd;        /* Handle of the video sub-window */
    HWND                 hparent;             /* Handle of the parent window */

# if !defined(NDEBUG) && defined(HAVE_DXGIDEBUG_H)
    HINSTANCE     dxgidebug_dll;
# endif
} vout_display_sys_win32_t;


/*****************************************************************************
 * Prototypes from common.c
 *****************************************************************************/
#ifndef VLC_WINSTORE_APP
int  CommonWindowInit(vout_display_t *, display_win32_area_t *, vout_display_sys_win32_t *,
                      bool projection_gestures);
void CommonWindowClean(vout_display_sys_win32_t *);
#endif /* !VLC_WINSTORE_APP */
void CommonControl(vout_display_t *, display_win32_area_t *, vout_display_sys_win32_t *, int );

void CommonPlacePicture (vout_display_t *, display_win32_area_t *);

void CommonInit(display_win32_area_t *);

void* HookWindowsSensors(vout_display_t*, HWND);
void UnhookWindowsSensors(void*);
# ifdef __cplusplus
}
# endif
