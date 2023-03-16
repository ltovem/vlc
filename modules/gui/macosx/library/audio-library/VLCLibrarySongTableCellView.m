// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCLibrarySongTableCellView.m: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <claudio.cambra@gmail.com>
 *****************************************************************************/

#import "VLCLibrarySongTableCellView.h"

#import "extensions/NSColor+VLCAdditions.h"
#import "extensions/NSFont+VLCAdditions.h"
#import "extensions/NSString+Helpers.h"
#import "extensions/NSView+VLCAdditions.h"
#import "views/VLCImageView.h"
#import "views/VLCTrackingView.h"
#import "main/VLCMain.h"
#import "library/VLCLibraryController.h"
#import "library/VLCLibraryDataTypes.h"
#import "library/VLCLibraryTableCellView.h"
#import "library/audio-library/VLCLibraryAlbumTracksDataSource.h"

NSString *VLCAudioLibrarySongCellIdentifier = @"VLCAudioLibrarySongCellIdentifier";

@interface VLCLibrarySongTableCellView ()
{
    VLCLibraryController *_libraryController;
}
@end

@implementation VLCLibrarySongTableCellView

+ (instancetype)fromNibWithOwner:(id)owner
{
    return (VLCLibrarySongTableCellView*)[NSView fromNibNamed:@"VLCLibrarySongTableCellView"
                                                     withClass:[VLCLibrarySongTableCellView class]
                                                     withOwner:owner];
}

- (void)awakeFromNib
{
    if(@available(macOS 10.14, *)) {
        self.playInstantlyButton.contentTintColor = [NSColor VLCAccentColor];
    }

    self.playInstantlyButton.target = self;
    self.playInstantlyButton.action = @selector(playInstantly:);

    self.trackingView.viewToHide = self.playInstantlyButton;
    self.trackingView.viewToShow = self.trackNumberTextField;
    [self prepareForReuse];
}

- (void)prepareForReuse
{
    [super prepareForReuse];
    self.songNameTextField.stringValue = @"";
    self.durationTextField.stringValue = @"";
    self.trackNumberTextField.stringValue = @"";
    self.playInstantlyButton.hidden = YES;
    self.trackNumberTextField.hidden = NO;
}

- (IBAction)playInstantly:(id)sender
{
    if(_representedMediaItem == nil) {
        return;
    }

    if (!_libraryController) {
        _libraryController = [[VLCMain sharedInstance] libraryController];
    }

    BOOL playImmediately = YES;
    [_libraryController appendItemToPlaylist:_representedMediaItem playImmediately:playImmediately];
}

- (void)setRepresentedMediaItem:(VLCMediaLibraryMediaItem *)representedMediaItem
{
    _representedMediaItem = representedMediaItem;
    self.songNameTextField.stringValue = representedMediaItem.displayString;
    self.durationTextField.stringValue = representedMediaItem.durationString;
    self.trackNumberTextField.stringValue = [NSString stringWithFormat:@"%d", representedMediaItem.trackNumber];

}

@end
