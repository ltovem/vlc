/*****************************************************************************
 * thread.c: Background worker based preparser module
 *****************************************************************************
 * Copyright © 2021 VLC authors and VideoLAN
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
#include <vlc_plugin.h>
#include <vlc_preparser.h>
#include <vlc_executor.h>
#include <vlc_input_item.h>

#include <assert.h>

struct preparser_sys_t
{
    vlc_executor_t *executor;
    atomic_bool deactivated;

    vlc_mutex_t lock;
    struct vlc_list submitted_tasks; /**< list of struct task */
};

struct task
{
    struct vlc_preparser* preparser;
    struct vlc_preparser_task* task;

    input_item_parser_id_t *parser;

    vlc_mutex_t lock;
    vlc_cond_t cond_ended;
    bool preparse_ended;
    int preparse_status;

    atomic_bool interrupted;

    struct vlc_runnable runnable; /**< to be passed to the executor */

    struct vlc_list node; /**< node of input_preparser_t.submitted_tasks */
};

static void
OnParserEnded(input_item_t *item, int status, void *task_)
{
    VLC_UNUSED(item);
    struct task *task = task_;

    if (atomic_load(&task->interrupted))
        /*
         * On interruption, the call to input_item_parser_id_Release() may
         * trigger this "parser ended" callback. Ignore it.
         */
        return;

    vlc_mutex_lock(&task->lock);
    assert(!task->preparse_ended);
    task->preparse_status = status == VLC_SUCCESS ? ITEM_PREPARSE_DONE
                                                  : ITEM_PREPARSE_FAILED;
    task->preparse_ended = true;
    vlc_mutex_unlock(&task->lock);

    vlc_cond_signal(&task->cond_ended);
}

static void
OnParserSubtreeAdded(input_item_t *item, input_item_node_t *subtree,
                     void *task_)
{
    VLC_UNUSED(item);
    struct task *task = task_;

    if (task->task->cbs && task->task->cbs->on_subtree_added)
        task->task->cbs->on_subtree_added(task->task->item, subtree,
                                          task->task->data);
}

static void
Parse(struct task *task, vlc_tick_t deadline)
{
    static const input_item_parser_cbs_t cbs = {
        .on_ended = OnParserEnded,
        .on_subtree_added = OnParserSubtreeAdded,
    };

    vlc_object_t *obj = &task->preparser->obj;
    task->parser = input_item_Parse(task->task->item, obj, &cbs, task);
    if (!task->parser)
    {
        task->preparse_status = ITEM_PREPARSE_FAILED;
        return;
    }

    /* Wait until the end of parsing */
    vlc_mutex_lock(&task->lock);
    if (deadline == VLC_TICK_INVALID)
    {
        while (!task->preparse_ended)
            vlc_cond_wait(&task->cond_ended, &task->lock);
    }
    else
    {
        bool timeout = false;
        while (!task->preparse_ended && !timeout)
            timeout =
                vlc_cond_timedwait(&task->cond_ended, &task->lock, deadline);

        if (timeout)
        {
            task->preparse_status = ITEM_PREPARSE_TIMEOUT;
            atomic_store(&task->interrupted, true);
        }
    }
    vlc_mutex_unlock(&task->lock);

    /* This call also interrupts the parsing if it is still running */
    input_item_parser_id_Release(task->parser);
}

static void RunnableRun(void*);

static struct task *
TaskNew(struct vlc_preparser* p, struct vlc_preparser_task* t)
{
    struct task *task = malloc(sizeof(*task));
    if (!task)
        return NULL;

    task->preparser = p;
    task->task = t;

    task->parser = NULL;
    vlc_mutex_init(&task->lock);
    vlc_cond_init(&task->cond_ended);
    task->preparse_ended = false;
    task->preparse_status = ITEM_PREPARSE_SKIPPED;

    atomic_init(&task->interrupted, false);

    task->runnable.run = RunnableRun;
    task->runnable.userdata = task;

    return task;
}

static void
TaskDelete(struct task *task)
{
    free(task);
}

static void
PreparserAddTask(struct vlc_preparser* p, struct task *task)
{
    struct preparser_sys_t* sys = p->sys;

    vlc_mutex_lock(&sys->lock);
    vlc_list_append(&task->node, &sys->submitted_tasks);
    vlc_mutex_unlock(&sys->lock);
}

static void
PreparserRemoveTask(struct vlc_preparser* p, struct task *task)
{
    struct preparser_sys_t* sys = p->sys;

    vlc_mutex_lock(&sys->lock);
    vlc_list_remove(&task->node);
    vlc_mutex_unlock(&sys->lock);
}

static void
NotifyPreparseEnded(struct task *task)
{
    if (task->task->cbs && task->task->cbs->on_preparse_ended)
        task->task->cbs->on_preparse_ended(task->task->item, task->preparse_status,
                                           task->task->data);
}

static void
RunnableRun(void *userdata)
{
    struct task *task = userdata;

    vlc_tick_t deadline = task->task->timeout ? vlc_tick_now() + task->task->timeout
                                        : VLC_TICK_INVALID;

    if (atomic_load(&task->interrupted))
        goto end;

    Parse(task, deadline);

end:
    NotifyPreparseEnded(task);
    PreparserRemoveTask(task->preparser, task);
    TaskDelete(task);
}

static int Preparse(struct vlc_preparser* p, struct vlc_preparser_task* task)
{
    struct preparser_sys_t* sys = p->sys;
    if( atomic_load( &sys->deactivated ) )
        return VLC_EGENERIC;

    vlc_mutex_lock( &task->item->lock );
    enum input_item_type_e i_type = task->item->i_type;
    int b_net = task->item->b_net;
    if( task->options & META_REQUEST_OPTION_DO_INTERACT )
        task->item->b_preparse_interact = true;
    vlc_mutex_unlock( &task->item->lock );

    switch( i_type )
    {
        case ITEM_TYPE_NODE:
        case ITEM_TYPE_FILE:
        case ITEM_TYPE_DIRECTORY:
        case ITEM_TYPE_PLAYLIST:
            if( !b_net || task->options & META_REQUEST_OPTION_SCOPE_NETWORK )
                break;
            /* fallthrough */
        default:
            if (task->cbs && task->cbs->on_preparse_ended)
                task->cbs->on_preparse_ended(task->item, ITEM_PREPARSE_SKIPPED,
                                             task->data);
            return VLC_SUCCESS;
    }

    struct task *t = TaskNew(p, task);
    if (unlikely(!t))
        return VLC_ENOMEM;

    PreparserAddTask(p, t);

    vlc_executor_Submit(sys->executor, &t->runnable);
    return VLC_SUCCESS;
}

static void
Interrupt(struct task *task)
{
    atomic_store(&task->interrupted, true);

    /* Wake up the preparser cond_wait */
    vlc_mutex_lock(&task->lock);
    task->preparse_status = ITEM_PREPARSE_TIMEOUT;
    task->preparse_ended = true;
    vlc_mutex_unlock(&task->lock);
    vlc_cond_signal(&task->cond_ended);
}

static void Cancel(struct vlc_preparser* p, void* id)
{
    struct preparser_sys_t* sys = p->sys;
    vlc_mutex_lock(&sys->lock);

    struct task *task;
    vlc_list_foreach(task, &sys->submitted_tasks, node)
    {
        if (!id || id == task->task->id)
        {
            bool canceled =
                vlc_executor_Cancel(sys->executor, &task->runnable);
            if (canceled)
            {
                NotifyPreparseEnded(task);
                vlc_list_remove(&task->node);
                TaskDelete(task);
            }
            else
                /* The task will be finished and destroyed after run() */
                Interrupt(task);
        }
    }

    vlc_mutex_unlock(&sys->lock);
}

static int Open(vlc_object_t* obj)
{
    struct vlc_preparser* p = (struct vlc_preparser*)obj;
    struct preparser_sys_t* sys = malloc(sizeof(*sys));
    if (unlikely(!sys))
        return VLC_ENOMEM;

    int max_threads = var_InheritInteger(obj, "preparse-threads");
    if (max_threads < 1)
        max_threads = 1;

    sys->executor = vlc_executor_New(max_threads);
    if (!sys->executor)
    {
        free(sys);
        return VLC_EGENERIC;
    }

    atomic_init( &sys->deactivated, false );

    vlc_mutex_init(&sys->lock);
    vlc_list_init(&sys->submitted_tasks);

    p->sys = sys;
    p->pf_cancel = Cancel;
    p->pf_preparse = Preparse;

    return VLC_SUCCESS;
}

static void Close(struct vlc_preparser* p)
{
    struct preparser_sys_t* sys = p->sys;
    atomic_store( &sys->deactivated, true );
    Cancel(p, NULL);

    vlc_executor_Delete(sys->executor);
    free(sys);
}

vlc_module_begin()
    set_shortname(N_("preparser"))
    set_description(N_( "Background worker based preparser" ))
    set_category(CAT_ADVANCED)
    set_subcategory(SUBCAT_ADVANCED_MISC)
    set_capability("preparser", 10)
    set_callbacks(Open, Close)
vlc_module_end()
