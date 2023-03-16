// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * en50221.h:
 *****************************************************************************
 * Copyright (C) 1998-2010 VLC authors and VideoLAN
 *
 * Authors: Johan Bilien <jobi@via.ecp.fr>
 *          Jean-Paul Saman <jpsaman _at_ videolan _dot_ org>
 *          Christopher Ross <chris@tebibyte.org>
 *          Christophe Massiot <massiot@via.ecp.fr>
 *****************************************************************************/

typedef struct cam cam_t;
typedef struct en50221_capmt_info_s en50221_capmt_info_t;

cam_t *en50221_Init( vlc_object_t *, int fd );
void en50221_Poll( cam_t * );
int en50221_SetCAPMT( cam_t *, en50221_capmt_info_t * );
char *en50221_Status( cam_t *, char *req );
void en50221_End( cam_t * );

#define STRINGIFY( z )   UGLY_KLUDGE( z )
#define UGLY_KLUDGE( z ) #z
