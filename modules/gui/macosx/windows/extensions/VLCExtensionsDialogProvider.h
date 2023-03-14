/*****************************************************************************
 * VLCExtensionsDialogProvider.h: Mac OS X Extensions Dialogs
 *****************************************************************************
 * Copyright (C) 2005-2012 VLC authors and VideoLAN
 *
 * Authors: Brendon Justin <brendonjustin@gmail.com>,
 *          Derk-Jan Hartman <hartman@videolan dot org>,
 *          Felix Paul Kühne <fkuehne@videolan dot org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

#import <vlc_common.h>
#import <vlc_dialog.h>
#import <vlc_extensions.h>

@class VLCDialogWindow;

/*****************************************************************************
 * ExtensionsDialogProvider interface
 *****************************************************************************/
@interface VLCExtensionsDialogProvider : NSObject <NSWindowDelegate>

- (void)performEventWithObject: (NSValue *)o_value ofType:(const char*)type;

- (void)triggerClick:(id)sender;
- (void)syncTextField:(NSNotification *)notification;
- (void)tableViewSelectionDidChange:(NSNotification *)notification;
- (void)popUpSelectionChanged:(id)sender;
- (NSSize)windowWillResize:(NSWindow *)sender toSize:(NSSize)frameSize;
- (BOOL)windowShouldClose:(id)sender;
- (void)updateWidgets:(extension_dialog_t *)dialog;

- (VLCDialogWindow *)createExtensionDialog:(extension_dialog_t *)p_dialog;
- (int)destroyExtensionDialog:(extension_dialog_t *)o_value;
- (VLCDialogWindow *)updateExtensionDialog:(NSValue *)o_value;
- (void)manageDialog:(extension_dialog_t *)p_dialog;

@end
