/*****************************************************************************
 * VLCPlaylistTableViewCell.m: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan -dot- org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import "VLCPlaylistTableCellView.h"

#import "extensions/NSString+Helpers.h"
#import "extensions/NSFont+VLCAdditions.h"

#import "library/VLCLibraryImageCache.h"

#import "main/VLCMain.h"

#import "playlist/VLCPlaylistItem.h"

#import "views/VLCImageView.h"

@interface VLCPlaylistTableCellView ()
{
    NSFont *_displayedFont;
    NSFont *_displayedBoldFont;
}
@end

@implementation VLCPlaylistTableCellView

- (void)awakeFromNib
{
    [self updateFontsBasedOnSetting:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(updateFontsBasedOnSetting:)
                                                 name:VLCConfigurationChangedNotification
                                               object:nil];
}

- (void)dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (void)setRepresentsCurrentPlaylistItem:(BOOL)representsCurrentPlaylistItem
{
    _representsCurrentPlaylistItem = representsCurrentPlaylistItem;
    NSFont *displayedFont = _representsCurrentPlaylistItem ? _displayedBoldFont : _displayedFont;
    self.mediaTitleTextField.font = displayedFont;
    self.secondaryMediaTitleTextField.font = displayedFont;
    self.artistTextField.font = _displayedFont;
    self.durationTextField.font = _displayedFont;
}

- (void)setRepresentedPlaylistItem:(VLCPlaylistItem *)item
{
    dispatch_async(dispatch_get_global_queue(QOS_CLASS_USER_INTERACTIVE, 0), ^{
        NSImage *image = [VLCLibraryImageCache thumbnailForPlaylistItem:item];
        dispatch_async(dispatch_get_main_queue(), ^{
            self.audioArtworkImageView.image = image;
            self.mediaImageView.image = image;
        });
    });

    NSString *artist = item.artistName;
    if (artist && artist.length > 0) {
        self.mediaTitleTextField.hidden = YES;
        self.secondaryMediaTitleTextField.hidden = NO;
        self.artistTextField.hidden = NO;
        self.secondaryMediaTitleTextField.stringValue = item.title;
        self.artistTextField.stringValue = artist;
        self.audioMediaTypeIndicator.hidden = NO;

        self.audioArtworkImageView.hidden = NO;
        self.mediaImageView.hidden = YES;
    } else {
        self.mediaTitleTextField.hidden = NO;
        self.secondaryMediaTitleTextField.hidden = YES;
        self.artistTextField.hidden = YES;
        self.mediaTitleTextField.stringValue = item.title;
        self.audioMediaTypeIndicator.hidden = YES;

        self.audioArtworkImageView.hidden = YES;
        self.mediaImageView.hidden = NO;
    }

    self.durationTextField.stringValue = [NSString stringWithTimeFromTicks:item.duration];

    _representedPlaylistItem = item;
}

- (void)updateFontsBasedOnSetting:(NSNotification *)aNotification
{
    BOOL largeText = config_GetInt("macosx-large-text");
    if (largeText) {
        _displayedFont = [NSFont VLCplaylistLabelFont];
        _displayedBoldFont = [NSFont VLCplaylistSelectedItemLabelFont];
    } else {
        _displayedFont = [NSFont VLCsmallPlaylistLabelFont];
        _displayedBoldFont = [NSFont VLCsmallPlaylistSelectedItemLabelFont];
    }
    [self setRepresentsCurrentPlaylistItem:_representsCurrentPlaylistItem];
}

@end
