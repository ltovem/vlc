// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCUIWidgets.h: Widgets for VLC's extensions dialogs for Mac OS X
 *****************************************************************************
 * Copyright (C) 2009-2015 the VideoLAN team and authors
 *
 * Authors: Pierre d'Herbemont <pdherbemont # videolan dot>,
 *          Brendon Justin <brendonjustin@gmail.com>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>
#import <vlc_extensions.h>

@class VLCDialogGridView;

@interface VLCDialogButton : NSButton
@property (readwrite) extension_widget_t *widget;
@end

@interface VLCDialogPopUpButton : NSPopUpButton
@property (readwrite) extension_widget_t *widget;
@end

@interface VLCDialogLabel : NSTextField
@end

@interface VLCDialogTextField : NSTextField
@property (readwrite) extension_widget_t *widget;
@end

@interface VLCDialogSecureTextField : NSSecureTextField
@property (readwrite) extension_widget_t *widget;
@end

@interface VLCDialogWindow : NSWindow
@property (readwrite) extension_dialog_t *dialog;
@property (readwrite) BOOL has_lock;
@end


@interface VLCDialogList : NSTableView <NSTableViewDataSource>
@property (readwrite) extension_widget_t *widget;
@property (readwrite, retain) NSMutableArray *contentArray;
@end

@interface VLCDialogGridView : NSView

- (void)addSubview:(NSView *)view atRow:(NSUInteger)row column:(NSUInteger)column rowSpan:(NSUInteger)rowSpan colSpan:(NSUInteger)colSpan;
- (NSSize)flexSize:(NSSize)size;
- (void)updateSubview:(NSView *)view
                atRow:(NSUInteger)row
               column:(NSUInteger)column
              rowSpan:(NSUInteger)rowSpan
              colSpan:(NSUInteger)colSpan;
- (void)removeSubview:(NSView *)view;

@property (readonly) NSUInteger numViews;

@end
