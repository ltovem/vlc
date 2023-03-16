// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCRendererMenuController.h: Controller class for the renderer menu
 *****************************************************************************
 * Copyright (C) 2016 VLC authors and VideoLAN
 *
 * Authors: Marvin Scholz <epirat07 at gmail dot com>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

#import "menus/renderers/VLCRendererItem.h"
#import "menus/renderers/VLCRendererDiscovery.h"

@interface VLCRendererMenuController : NSObject <VLCRendererDiscoveryDelegate>

@property (readwrite, weak) NSMenu *rendererMenu;
@property (readwrite, weak) NSMenuItem *rendererNoneItem;

- (void)startRendererDiscoveries;
- (void)stopRendererDiscoveries;
- (void)selectRenderer:(NSMenuItem *)sender;

@end
