/*****************************************************************************
 * VLCLibraryAlbumTableCellView.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan -dot- org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@class VLCImageView;
@class VLCTrackingView;
@class VLCMediaLibraryAlbum;

@interface VLCLibraryAlbumTableCellView : NSTableCellView

extern NSString *VLCAudioLibraryCellIdentifier;
extern NSString *VLCLibraryAlbumTableCellTableViewColumnIdentifier;

+ (instancetype)fromNibWithOwner:(id)owner;
+ (CGFloat)defaultHeight;

@property (readwrite, assign) IBOutlet VLCTrackingView *trackingView;
@property (readwrite, assign) IBOutlet VLCImageView *representedImageView;
@property (readwrite, assign) IBOutlet NSTextField *albumNameTextField;
@property (readwrite, assign) IBOutlet NSTextField *artistNameTextField;
@property (readwrite, assign) IBOutlet NSTextField *summaryTextField;
@property (readwrite, assign) IBOutlet NSTextField *yearTextField;
@property (readwrite, assign) IBOutlet NSButton *playInstantlyButton;

@property (readonly) CGFloat height;
@property (readwrite, assign, nonatomic) VLCMediaLibraryAlbum *representedAlbum;

- (IBAction)playInstantly:(id)sender;

@end

NS_ASSUME_NONNULL_END
