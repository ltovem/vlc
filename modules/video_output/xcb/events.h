// SPDX-License-Identifier: LGPL-2.1-or-later
/**
 * @file events.h
 * @brief X C Bindings VLC module common header
 */
/*****************************************************************************
 * Copyright © 2009 Rémi Denis-Courmont
 *****************************************************************************/

#ifndef XCB_CURSOR_NONE
# define XCB_CURSOR_NONE ((xcb_cursor_t) 0U)
#endif

struct vlc_logger;

/* events.c */

/**
 * Checks for an XCB error.
 */
int vlc_xcb_error_Check(struct vlc_logger *, xcb_connection_t *conn,
                        const char *str, xcb_void_cookie_t);

/**
 * Allocates a window for XCB-based output.
 *
 * Creates a VLC video X window object, connects to the corresponding X server,
 * finds the corresponding X server screen.
 */
int vlc_xcb_parent_Create(struct vlc_logger *, const vlc_window_t *wnd,
                          xcb_connection_t **connp,
                          const xcb_screen_t **screenp);
/**
 * Processes XCB events.
 */
int vlc_xcb_Manage(struct vlc_logger *, xcb_connection_t *conn);
