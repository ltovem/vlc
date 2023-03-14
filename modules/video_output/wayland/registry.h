/**
 * @file registry.h
 * @brief Wayland client register common helpers for VLC media player
 */
/*****************************************************************************
 * Copyright © 2019 Rémi Denis-Courmont
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#include <stdint.h>

struct wl_display;
struct wl_event_queue;
struct wl_interface;
struct wl_shm;
struct vlc_wl_registry;

struct vlc_wl_registry *vlc_wl_registry_get(struct wl_display *display,
                                            struct wl_event_queue *queue);
void vlc_wl_registry_destroy(struct vlc_wl_registry *vr);

uint_fast32_t vlc_wl_interface_get_version(struct vlc_wl_registry *,
                                           const char *interface);
struct wl_proxy *vlc_wl_interface_bind(struct vlc_wl_registry *,
                                       const char *interface,
                                       const struct wl_interface *,
                                       uint32_t *version);

struct wl_compositor *vlc_wl_compositor_get(struct vlc_wl_registry *);
struct wl_shm *vlc_wl_shm_get(struct vlc_wl_registry *);

