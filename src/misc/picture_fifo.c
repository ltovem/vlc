// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * picture_fifo.c : picture fifo functions
 *****************************************************************************
 * Copyright (C) 2009 VLC authors and VideoLAN
 * Copyright (C) 2009 Laurent Aimar <fenrir _AT_ videolan _DOT_ org>
 *
 * Authors: Laurent Aimar <fenrir _AT_ videolan _DOT_ org>
 *****************************************************************************/

/*****************************************************************************
 * Preamble
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#include <assert.h>

#include <vlc_common.h>
#include <vlc_picture_fifo.h>

/*****************************************************************************
 *
 *****************************************************************************/
struct picture_fifo_t {
    vlc_mutex_t lock;
    vlc_picture_chain_t pics;
};

static void PictureFifoReset(picture_fifo_t *fifo)
{
    vlc_picture_chain_Init( &fifo->pics );
}
static void PictureFifoPush(picture_fifo_t *fifo, picture_t *picture)
{
    assert(!picture_HasChainedPics(picture));
    vlc_picture_chain_Append( &fifo->pics, picture );
}
static picture_t *PictureFifoPop(picture_fifo_t *fifo)
{
    return vlc_picture_chain_PopFront( &fifo->pics );
}

picture_fifo_t *picture_fifo_New(void)
{
    picture_fifo_t *fifo = malloc(sizeof(*fifo));
    if (!fifo)
        return NULL;

    vlc_mutex_init(&fifo->lock);
    PictureFifoReset(fifo);
    return fifo;
}

void picture_fifo_Push(picture_fifo_t *fifo, picture_t *picture)
{
    vlc_mutex_lock(&fifo->lock);
    PictureFifoPush(fifo, picture);
    vlc_mutex_unlock(&fifo->lock);
}
picture_t *picture_fifo_Pop(picture_fifo_t *fifo)
{
    vlc_mutex_lock(&fifo->lock);
    picture_t *picture = PictureFifoPop(fifo);
    vlc_mutex_unlock(&fifo->lock);

    return picture;
}
bool picture_fifo_IsEmpty(picture_fifo_t *fifo)
{
    vlc_mutex_lock(&fifo->lock);
    bool empty = vlc_picture_chain_IsEmpty( &fifo->pics );
    vlc_mutex_unlock(&fifo->lock);

    return empty;
}
void picture_fifo_Flush(picture_fifo_t *fifo, vlc_tick_t date, bool flush_before)
{
    picture_t *picture;

    vlc_picture_chain_t flush_chain;

    vlc_picture_chain_Init(&flush_chain);

    vlc_mutex_lock(&fifo->lock);
    if (date == VLC_TICK_INVALID)
        vlc_picture_chain_GetAndClear(&fifo->pics, &flush_chain);
    else {
        vlc_picture_chain_t filter_chain;
        vlc_picture_chain_GetAndClear(&fifo->pics, &filter_chain);

        while ( !vlc_picture_chain_IsEmpty( &filter_chain ) ) {
            picture = vlc_picture_chain_PopFront( &filter_chain );

            if (( flush_before && picture->date <= date) ||
                (!flush_before && picture->date >= date))
                vlc_picture_chain_Append( &flush_chain, picture );
            else
                PictureFifoPush(fifo, picture);
        }
    }
    vlc_mutex_unlock(&fifo->lock);

    while ((picture = vlc_picture_chain_PopFront(&flush_chain)) != NULL)
        picture_Release(picture);
}
void picture_fifo_Delete(picture_fifo_t *fifo)
{
    picture_fifo_Flush(fifo, VLC_TICK_INVALID, true);
    free(fifo);
}
