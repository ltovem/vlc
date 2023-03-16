/*****************************************************************************
 * ft2_err.h: turn freetype2 errors into strings
 *****************************************************************************
 * Copyright (C) 2009 the VideoLAN team
 *
 * Authors: JP Dinger <jpd (at) videolan (dot) org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef FT2_ERR_H
#define FT2_ERR_H
#ifdef __cplusplus
extern "C" {
#endif

const char *ft2_strerror(unsigned err);

#ifdef __cplusplus
}
#endif
#endif
