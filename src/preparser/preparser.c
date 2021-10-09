/*****************************************************************************
 * preparser.c
 *****************************************************************************
 * Copyright © 2017-2017 VLC authors and VideoLAN
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include <vlc_preparser.h>
#include <vlc_modules.h>

#include "input/input_internal.h"
#include "fetcher.h"

struct vlc_preparser_private
{
    struct vlc_preparser p;

    input_fetcher_t* fetcher;
    int default_timeout;
};

struct vlc_preparser_task_private
{
    struct vlc_preparser_task t;
    struct vlc_preparser* p;
    const input_preparser_callbacks_t *user_cbs;
    void* user_data;
    enum input_item_preparse_status status;
};

static struct vlc_preparser_private* preparser_priv(struct vlc_preparser* p)
{
    return container_of(p, struct vlc_preparser_private, p);
}

static void vlc_preparser_Task_Delete(struct vlc_preparser_task* t)
{
    input_item_Release(t->item);
    free(t);
}

static void
vlc_preparser_OnArtFetchEnded(input_item_t *item, bool fetched, void *userdata)
{
    VLC_UNUSED(fetched);

    struct vlc_preparser_task_private *task = userdata;
    task->user_cbs->on_preparse_ended(item, task->status, task->user_data);
    vlc_preparser_Task_Delete(&task->t);
}

static const input_fetcher_callbacks_t fetcher_cbs = {
    .on_art_fetch_ended = vlc_preparser_OnArtFetchEnded,
};

static void vlc_preparser_OnPreparseEnded(input_item_t* item,
                                          enum input_item_preparse_status s,
                                          void* data)
{
    struct vlc_preparser_task_private* task = data;
    struct vlc_preparser_private* priv = preparser_priv(task->p);

    input_fetcher_t *fetcher = priv->fetcher;
    if (fetcher && (task->t.options & META_REQUEST_OPTION_FETCH_ANY) &&
        s != ITEM_PREPARSE_TIMEOUT)
    {
        int ret = input_fetcher_Push(fetcher, item,
                               task->t.options & META_REQUEST_OPTION_FETCH_ANY,
                               &fetcher_cbs, task);
        if (ret == VLC_SUCCESS)
        {
            // Postpone the callback until the fetcher completes
            task->status = s;
            return;
        }
    }

    if (s != ITEM_PREPARSE_TIMEOUT)
        input_item_SetPreparsed(item, true);

    if (task->user_cbs && task->user_cbs->on_preparse_ended)
        task->user_cbs->on_preparse_ended(item, s, task->user_data);
    vlc_preparser_Task_Delete(&task->t);
}

static void vlc_preparser_OnSubTreeAdded(input_item_t* item,
                                         input_item_node_t* subtree, void* data)
{
    struct vlc_preparser_task_private* task = data;
    if (task->user_cbs && task->user_cbs->on_subtree_added)
        task->user_cbs->on_subtree_added(item, subtree, task->user_data);
}

static const input_preparser_callbacks_t priv_cbs = {
    .on_preparse_ended = vlc_preparser_OnPreparseEnded,
    .on_subtree_added = vlc_preparser_OnSubTreeAdded,
};

int
vlc_preparser_Preparse(struct vlc_preparser* p, input_item_t* item,
                       input_item_meta_request_option_t opts,
                       const input_preparser_callbacks_t* cbs, void* data,
                       int timeout, void* id)
{
    struct vlc_preparser_task_private* priv = malloc(sizeof(*priv));
    if (unlikely(!priv))
        return VLC_ENOMEM;
    struct vlc_preparser_private* pp = preparser_priv(p);
    priv->user_cbs = cbs;
    priv->user_data = data;
    priv->p = p;
    priv->status = ITEM_PREPARSE_SKIPPED;
    struct vlc_preparser_task* t = &priv->t;
    t->item = input_item_Hold(item);
    t->options = opts;
    t->cbs = &priv_cbs;
    t->data = priv;
    t->timeout = timeout == -1 ? pp->default_timeout : VLC_TICK_FROM_MS(timeout);
    t->id = id;
    if (p->pf_preparse( p, t ) != VLC_SUCCESS)
    {
        input_item_Release(item);
        free(t);
        return VLC_EGENERIC;
    }
    return VLC_SUCCESS;
}

void vlc_preparser_Cancel(struct vlc_preparser* p, void* id)
{
    p->pf_cancel(p, id);
}

struct vlc_preparser* vlc_preparser_New(vlc_object_t* parent)
{
    struct vlc_preparser_private* priv =
            vlc_custom_create(parent, sizeof(*priv), "preparser");
    if (unlikely(!priv))
        return NULL;
    struct vlc_preparser* p = &priv->p;

    priv->p.module = module_need(p, "preparser", NULL, false );
    if (!priv->p.module)
    {
        vlc_object_delete(p);
        return NULL;
    }

    priv->fetcher = input_fetcher_New( VLC_OBJECT(p) );
    if( unlikely( !priv->fetcher ) )
        msg_Warn( p, "unable to create art fetcher" );

    priv->default_timeout =
        VLC_TICK_FROM_MS(var_InheritInteger(parent, "preparse-timeout"));
    if (priv->default_timeout < 0)
        priv->default_timeout = 0;

    return p;
}

void vlc_preparser_Delete(struct vlc_preparser* p)
{
    struct vlc_preparser_private* priv = preparser_priv(p);
    module_unneed(p, p->module);
    if( priv->fetcher )
        input_fetcher_Delete( priv->fetcher );
    vlc_object_delete(p);
}

void vlc_preparser_fetcher_Push( struct vlc_preparser *p,
    input_item_t *item, input_item_meta_request_option_t options,
    const input_fetcher_callbacks_t *cbs, void *cbs_userdata )
{
    struct vlc_preparser_private* priv = preparser_priv(p);
    if( priv->fetcher )
        input_fetcher_Push( priv->fetcher, item, options, cbs, cbs_userdata );
}
