// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCExtensionsManager.h: Extensions manager for Cocoa
 *****************************************************************************
 * Copyright (C) 2012 VideoLAN and authors
 *
 * Authors: Brendon Justin <brendonjustin@gmail.com>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#import "VLCExtensionsDialogProvider.h"

@interface VLCExtensionsManager : NSObject

- (void)buildMenu:(NSMenu *)extMenu;

- (BOOL)loadExtensions;
- (void)unloadExtensions;
- (void)reloadExtensions;

- (void)triggerMenu:(id)sender;
- (void)playingChanged:(int)state;
- (void)metaChanged:(input_item_t *)p_input;

- (BOOL)isLoaded;
- (BOOL)cannotLoad;

@property (readonly) BOOL isUnloading;

@end
