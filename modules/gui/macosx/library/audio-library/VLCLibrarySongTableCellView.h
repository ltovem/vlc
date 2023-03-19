// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCLibrarySongTableCellView.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <claudio.cambra@gmail.com>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@class VLCTrackingView;
@class VLCMediaLibraryMediaItem;

@interface VLCLibrarySongTableCellView : NSTableCellView

extern NSString *VLCAudioLibrarySongCellIdentifier;

+ (instancetype)fromNibWithOwner:(id)owner;

@property (readwrite, assign) IBOutlet VLCTrackingView *trackingView;
@property (readwrite, assign) IBOutlet NSTextField *songNameTextField;
@property (readwrite, assign) IBOutlet NSTextField *durationTextField;
@property (readwrite, assign) IBOutlet NSTextField *trackNumberTextField;
@property (readwrite, assign) IBOutlet NSButton *playInstantlyButton;

@property (readwrite, assign, nonatomic) VLCMediaLibraryMediaItem *representedMediaItem;

- (IBAction)playInstantly:(id)sender;

@end

NS_ASSUME_NONNULL_END
