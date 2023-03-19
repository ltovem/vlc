// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * synchro.h: frame-dropping structures
 *****************************************************************************
 * Copyright (C) 1999-2005 VLC authors and VideoLAN
 *
 * Authors: Christophe Massiot <massiot@via.ecp.fr>
 *          Jean-Marc Dressler <polux@via.ecp.fr>
 *          Stéphane Borel <stef@via.ecp.fr>
 *****************************************************************************/

/*****************************************************************************
 * decoder_synchro_t : timers for the video synchro
 *****************************************************************************/
typedef struct decoder_synchro_t decoder_synchro_t;

/* Read the discussion on top of synchro.c for more information. */
/* Pictures types */
#define I_CODING_TYPE           1
#define P_CODING_TYPE           2
#define B_CODING_TYPE           3
#define D_CODING_TYPE           4 /* MPEG-1 ONLY */
/* other values are reserved */

/*****************************************************************************
 * Prototypes
 *****************************************************************************/
decoder_synchro_t * decoder_SynchroInit( decoder_t *, int ) VLC_USED;
void decoder_SynchroRelease( decoder_synchro_t * );
void decoder_SynchroReset( decoder_synchro_t * );
bool decoder_SynchroChoose( decoder_synchro_t *, int, vlc_tick_t, bool );
void decoder_SynchroTrash( decoder_synchro_t * );
void decoder_SynchroDecode( decoder_synchro_t * );
void decoder_SynchroEnd( decoder_synchro_t *, int, bool );
vlc_tick_t decoder_SynchroDate( decoder_synchro_t * ) VLC_USED;
void decoder_SynchroNewPicture( decoder_synchro_t *, int, int, vlc_tick_t, vlc_tick_t, bool );

