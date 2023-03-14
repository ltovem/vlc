/*****************************************************************************
 * sections.h: Transport Stream sections assembler
 *****************************************************************************
 * Copyright (C) 2016 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifndef TS_SECTIONS_H
#define TS_SECTIONS_H

typedef void(* ts_section_processor_callback_t)( demux_t *,
                                                 const uint8_t *, size_t,
                                                 const uint8_t *, size_t,
                                                 void * );

typedef struct ts_sections_processor_t ts_sections_processor_t;

void ts_sections_processor_Add( demux_t *,
                                ts_sections_processor_t **pp_chain,
                                uint8_t i_table_id, uint16_t i_extension_id,
                                ts_section_processor_callback_t pf_callback,
                                void *p_callback_data );

void ts_sections_processor_ChainDelete( ts_sections_processor_t *p_chain );

void ts_sections_processor_Reset( ts_sections_processor_t *p_chain );

void ts_sections_processor_Push( ts_sections_processor_t *p_chain,
                                 const uint8_t * );
#endif
