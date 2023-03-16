// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * emscripten.c: Emscripten logger
 *****************************************************************************
 * Copyright © 2022 VLC authors, VideoLAN and Videolabs
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <emscripten.h>

#include <stdarg.h>
#include <stdio.h>

#include <vlc_common.h>
#include <vlc_plugin.h>

static void EmscriptenPrintMsg(void *opaque, int type, const vlc_log_t *p_item,
                               const char *format, va_list ap)
{
    int prio;
    char *message;
    int verbosity = (int)opaque;

    if (verbosity < type)
        return;

    if (vasprintf(&message, format, ap) < 0)
        return;
    switch (type) {
        case VLC_MSG_ERR:
            prio = EM_LOG_CONSOLE | EM_LOG_ERROR;
            break;
        case VLC_MSG_WARN:
            prio = EM_LOG_CONSOLE | EM_LOG_WARN;
            break;
        case VLC_MSG_INFO:
            prio = EM_LOG_CONSOLE | EM_LOG_INFO;
            break;
        case VLC_MSG_DBG:
            prio = EM_LOG_CONSOLE | EM_LOG_DEBUG;
        default:
            prio = EM_LOG_CONSOLE;
    }

    emscripten_log(prio, "[vlc.js: 0x%"PRIxPTR"/0x%"PRIxPTR"] %s %s: %s",
                   p_item->i_object_id, p_item->tid, p_item->psz_module,
                   p_item->psz_object_type, message);
    free(message);
}

static const struct vlc_logger_operations ops = { EmscriptenPrintMsg, NULL };

static const struct vlc_logger_operations *Open(vlc_object_t *obj, void **sysp)
{
    int verbosity = var_InheritInteger(obj, "verbose");

    if (verbosity < 0)
        return NULL;
    verbosity += VLC_MSG_ERR;
    *sysp = (void *)(int)verbosity;
    return &ops;
}

vlc_module_begin()
    set_shortname(N_("emlog"))
    set_description(N_("Emscripten logger"))
    set_subcategory(SUBCAT_ADVANCED_MISC)
    set_capability("logger", 30)
    set_callback(Open)
vlc_module_end ()
