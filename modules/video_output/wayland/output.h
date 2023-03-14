/*****************************************************************************
 * Copyright © 2018 Rémi Denis-Courmont
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#include <stdint.h>

struct vlc_window;
struct wl_registry;
struct output_list;

struct output_list *output_list_create(struct vlc_window *wnd);
void output_list_destroy(struct output_list *);

struct wl_output *output_create(struct output_list *, struct wl_registry *,
                                uint32_t id, uint32_t version);
struct wl_output *output_find_by_id(struct output_list *, uint32_t id);
struct wl_output *output_find_by_name(struct output_list *, const char *name);
void output_destroy(struct output_list *, struct wl_output *);
