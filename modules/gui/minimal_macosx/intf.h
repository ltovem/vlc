// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * intf.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2002-2012 VLC authors and VideoLAN
 *
 * Authors: Pierre d'Herbemont <pdherbemont # videolan.org>
 *          Felix Paul Kühne <fkuehne at videolan dot org>
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
#   import "config.h"
#endif

#import <Cocoa/Cocoa.h>
#import <vlc_common.h>
#import <vlc_interface.h>

/*****************************************************************************
 * Local prototypes.
 *****************************************************************************/

/*****************************************************************************
 * intf_sys_t: description and status of the interface
 *****************************************************************************/
struct intf_sys_t
{
    int nothing_for_now;
};

