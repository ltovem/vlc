// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * win32touch.c: touch gestures recognition
 *****************************************************************************
 * Copyright © 2013-2014 VideoLAN
 *
 * Authors: Ludovic Fauvet <etix@videolan.org>
 *          Jean-Baptiste Kempf <jb@videolan.org>
 *****************************************************************************/

#ifndef VLC_GESTURE_H_
#define VLC_GESTURE_H_

# include <windows.h>
# include <winuser.h>

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>

#ifndef WM_GESTURE
# define WM_GESTURE 0x0119
#endif

#ifdef __cplusplus
extern "C" {
#endif

enum {
    GESTURE_ACTION_UNDEFINED = 0,
    GESTURE_ACTION_VOLUME,
    GESTURE_ACTION_JUMP,
    GESTURE_ACTION_BRIGHTNESS
};


typedef struct win32_gesture_sys_t {
    DWORD       i_type;                 /* Gesture ID */
    int         i_action;               /* GESTURE_ACTION */

    int         i_beginx;               /* Start X position */
    int         i_beginy;               /* Start Y position */
    int         i_lasty;                /* Last known Y position for PAN */
    double      f_lastzoom;             /* Last zoom factor */

    ULONGLONG   i_ullArguments;         /* Base values to compare between 2 zoom gestures */
    bool        b_2fingers;             /* Did we detect 2 fingers? */

    BOOL (*DecodeGestureImpl)( vlc_object_t *p_this, struct win32_gesture_sys_t *p_gesture, const GESTUREINFO* p_gi );

    HINSTANCE   huser_dll;              /* user32.dll */
    BOOL (WINAPI * OurCloseGestureInfoHandle)(HGESTUREINFO hGestureInfo);
    BOOL (WINAPI * OurGetGestureInfo)(HGESTUREINFO hGestureInfo, PGESTUREINFO pGestureInfo);
} win32_gesture_sys_t;


BOOL InitGestures( HWND hwnd, win32_gesture_sys_t **p_gesture, bool b_isProjected );

LRESULT DecodeGesture( vlc_object_t *p_intf, win32_gesture_sys_t *p_gesture,
        HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

void CloseGestures( win32_gesture_sys_t *p_gesture );

#ifdef __cplusplus
}
#endif

#endif
