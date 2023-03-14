/*****************************************************************************
 * VLCPlaylistTableView.m: table view subclass for the playlist
 *****************************************************************************
 * Copyright (C) 2003-2019 VLC authors and VideoLAN
 *
 * Authors: Derk-Jan Hartman <hartman at videola/n dot org>
 *          Benjamin Pracht <bigben at videolab dot org>
 *          Felix Paul Kühne <fkuehne at videolan dot org>
 *          David Fuhrmann <dfuhrmann # videolan.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import "VLCPlaylistTableView.h"

#import "main/VLCMain.h"
#import "playlist/VLCPlaylistController.h"
#import "playlist/VLCPlaylistMenuController.h"

@interface VLCPlaylistTableView ()
{
    VLCPlaylistMenuController *_menuController;
}
@end

@implementation VLCPlaylistTableView

- (NSMenu *)menuForEvent:(NSEvent *)event
{
    if (!_menuController) {
        _menuController = [[VLCPlaylistMenuController alloc] init];
        _menuController.playlistTableView = self;
    }

    NSPoint pt = [self convertPoint: [event locationInWindow] fromView: nil];
    NSInteger row = [self rowAtPoint:pt];
    if (row != -1 && ![[self selectedRowIndexes] containsIndex: row])
        [self selectRowIndexes:[NSIndexSet indexSetWithIndex:row] byExtendingSelection:NO];

    return _menuController.playlistMenu;
}

- (void)keyDown:(NSEvent *)event
{
    unichar key = 0;

    if ([[event characters] length]) {
        /* we evaluate the first key only */
        key = [[event characters] characterAtIndex: 0];
    }

    size_t indexOfSelectedItem = self.selectedRow;
    NSIndexSet *selectedIndexes = [self selectedRowIndexes];

    switch(key) {
        case NSDeleteCharacter:
        case NSDeleteFunctionKey:
        case NSDeleteCharFunctionKey:
        case NSBackspaceCharacter:
        {
            VLCPlaylistController *playlistController = [[VLCMain sharedInstance] playlistController];
            [playlistController removeItemsAtIndexes:selectedIndexes];
            break;
        }

        case NSEnterCharacter:
        case NSCarriageReturnCharacter:
            [[[VLCMain sharedInstance] playlistController] playItemAtIndex:indexOfSelectedItem];
            break;

        default:
            [super keyDown: event];
            break;
    }
}

- (BOOL)acceptsFirstResponder
{
    return YES;
}

- (BOOL)becomeFirstResponder
{
    [self setNeedsDisplay:YES];
    return YES;
}

- (BOOL)resignFirstResponder
{
    [self setNeedsDisplay:YES];
    return YES;
}

@end
