/*****************************************************************************
 * interrupt.h:
 *****************************************************************************
 * Copyright (C) 2015 Rémi Denis-Courmont
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

/** @ingroup interrupt */
#ifndef LIBVLC_INPUT_SIGNAL_H
# define LIBVLC_INPUT_SIGNAL_H 1

# include <stdatomic.h>

# include <vlc_interrupt.h>

void vlc_interrupt_init(vlc_interrupt_t *);
void vlc_interrupt_deinit(vlc_interrupt_t *);

struct vlc_interrupt
{
    vlc_mutex_t lock;
    bool interrupted;
    atomic_bool killed;
    void (*callback)(void *);
    void *data;
};
#endif
