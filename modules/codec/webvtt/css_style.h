// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * css_style.h : CSS styles conversions
 *****************************************************************************
 * Copyright (C) 2017 VideoLabs, VLC authors and VideoLAN
 *****************************************************************************/

void webvtt_FillStyleFromCssDeclaration( const vlc_css_declaration_t *p_decl,
                                         text_style_t *p_style );
