/*****************************************************************************
 * csa.h
 *****************************************************************************
 * Copyright (C) 2004 Laurent Aimar
 *
 * Authors: Laurent Aimar <fenrir@via.ecp.fr>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef VLC_MPEG_CSA_H_
#define VLC_MPEG_CSA_H_

typedef struct csa_t csa_t;
#define csa_New     __csa_New
#define csa_Delete  __csa_Delete
#define csa_SetCW  __csa_SetCW
#define csa_UseKey  __csa_UseKey
#define csa_Decrypt __csa_decrypt
#define csa_Encrypt __csa_encrypt

csa_t *csa_New( void );
void   csa_Delete( csa_t * );

int    csa_SetCW( vlc_object_t *p_caller, csa_t *c, char *psz_ck, bool odd );
void   csa_UseKey( vlc_object_t *p_caller, csa_t *, bool use_odd );

void   csa_Decrypt( csa_t *, uint8_t *pkt, int i_pkt_size );
void   csa_Encrypt( csa_t *, uint8_t *pkt, int i_pkt_size );

#endif /* _CSA_H */
