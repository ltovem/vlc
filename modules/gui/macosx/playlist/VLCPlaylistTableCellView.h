/*****************************************************************************
 * VLCPlaylistTableViewCell.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan -dot- org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

@class VLCImageView;
@class VLCPlaylistItem;

NS_ASSUME_NONNULL_BEGIN

@interface VLCPlaylistTableCellView : NSTableCellView

@property (readwrite, assign, nonatomic) VLCPlaylistItem *representedPlaylistItem;
@property (readwrite, nonatomic) BOOL representsCurrentPlaylistItem;

@property (readwrite, assign) IBOutlet NSTextField *artistTextField;
@property (readwrite, assign) IBOutlet NSTextField *secondaryMediaTitleTextField;
@property (readwrite, assign) IBOutlet NSTextField *mediaTitleTextField;
@property (readwrite, assign) IBOutlet NSTextField *durationTextField;
@property (readwrite, assign) IBOutlet VLCImageView *mediaImageView;
@property (readwrite, assign) IBOutlet NSImageView *audioMediaTypeIndicator;
@property (readwrite, assign) IBOutlet VLCImageView *audioArtworkImageView;

@end

NS_ASSUME_NONNULL_END
