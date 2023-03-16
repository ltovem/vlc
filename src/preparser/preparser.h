// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * preparser.h
 *****************************************************************************
 * Copyright (C) 1999-2008 VLC authors and VideoLAN
 *
 * Authors: Samuel Hocevar <sam@zoy.org>
 *          Clément Stenac <zorglub@videolan.org>
 *****************************************************************************/

#ifndef _INPUT_PREPARSER_H
#define _INPUT_PREPARSER_H 1

#include <vlc_input_item.h>
/**
 * Preparser opaque structure.
 *
 * The preparser object will retrieve the meta data of any given input item in
 * an asynchronous way.
 * It will also issue art fetching requests.
 */
typedef struct input_preparser_t input_preparser_t;

/**
 * This function creates the preparser object and thread.
 */
input_preparser_t *input_preparser_New( vlc_object_t * );

/**
 * This function enqueues the provided item to be preparsed.
 *
 * The input item is retained until the preparsing is done or until the
 * preparser object is deleted.
 *
 * @param timeout maximum time allowed to preparse the item. If -1, the default
 * "preparse-timeout" option will be used as a timeout. If 0, it will wait
 * indefinitely. If > 0, the timeout will be used (in milliseconds).
 * @param id unique id provided by the caller. This is can be used to cancel
 * the request with input_preparser_Cancel()
 * @returns VLC_SUCCESS if the item was scheduled for preparsing, an error code
 * otherwise
 * If this returns an error, the on_preparse_ended will *not* be invoked
 */
int input_preparser_Push( input_preparser_t *, input_item_t *,
                           input_item_meta_request_option_t,
                           const input_preparser_callbacks_t *cbs,
                           void *cbs_userdata,
                           int timeout, void *id );

void input_preparser_fetcher_Push( input_preparser_t *, input_item_t *,
                                   input_item_meta_request_option_t,
                                   const input_fetcher_callbacks_t *cbs,
                                   void *cbs_userdata );

/**
 * This function cancel all preparsing requests for a given id
 *
 * @param id unique id given to input_preparser_Push()
 */
void input_preparser_Cancel( input_preparser_t *, void *id );

/**
 * This function destroys the preparser object and thread.
 *
 * All pending input items will be released.
 */
void input_preparser_Delete( input_preparser_t * );

/**
 * This function deactivates the preparser
 *
 * All pending requests will be removed, and it will block until the currently
 * running entity has finished (if any).
 */
void input_preparser_Deactivate( input_preparser_t * );

#endif

