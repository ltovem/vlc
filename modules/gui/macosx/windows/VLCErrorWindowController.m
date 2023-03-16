// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * HelpWindowController.m
 *****************************************************************************
 * Copyright (C) 2017 VLC authors and VideoLAN
 *
 * Authors: Derk-Jan Hartman <thedj@users.sourceforge.net>
 *          Felix Paul Kühne <fkuehne -at- videolan.org>
 *****************************************************************************/

#import "VLCErrorWindowController.h"

#import "extensions/NSString+Helpers.h"

@interface VLCErrorWindowController()
{
    NSMutableArray *_errors;
    NSMutableArray *_icons;
}
@end

@implementation VLCErrorWindowController

- (id)init
{
    self = [super initWithWindowNibName:@"ErrorPanel"];
    if (self) {
        /* init data sources */
        _errors = [[NSMutableArray alloc] init];
        _icons = [[NSMutableArray alloc] init];
    }

    return self;
}

- (void)windowDidLoad
{
    /* init strings */
    [self.window setTitle: _NS("Errors and Warnings")];
    [self.cleanupButton setTitle: _NS("Clean up")];

    if ([[NSApplication sharedApplication] userInterfaceLayoutDirection] == NSUserInterfaceLayoutDirectionRightToLeft) {
        [self.errorTable moveColumn:0 toColumn:1];
    }
}

- (void)addError:(NSString *)title withMsg:(NSString *)message;
{
    /* format our string as desired */
    NSMutableAttributedString * ourError;
    ourError = [[NSMutableAttributedString alloc] initWithString:
                [NSString stringWithFormat:@"%@\n%@", title, message]
                                                      attributes:@{NSFontAttributeName : [NSFont systemFontOfSize:11]}];
    [ourError addAttribute: NSFontAttributeName
                     value: [NSFont boldSystemFontOfSize:11]
                     range: NSMakeRange(0, [title length])];
    [_errors addObject: ourError];

    [_icons addObject: [[NSWorkspace sharedWorkspace] iconForFileType:NSFileTypeForHFSTypeCode(kAlertStopIcon)]];

    [self.errorTable reloadData];
}

-(IBAction)cleanupTable:(id)sender
{
    [_errors removeAllObjects];
    [_icons removeAllObjects];
    [self.errorTable reloadData];
}

/*----------------------------------------------------------------------------
 * data source methods
 *---------------------------------------------------------------------------*/
- (NSInteger)numberOfRowsInTableView:(NSTableView *)theDataTable
{
    return [_errors count];
}

- (id)tableView:(NSTableView *)theDataTable objectValueForTableColumn:(NSTableColumn *)theTableColumn
            row: (NSInteger)row
{
    if ([[theTableColumn identifier] isEqualToString: @"error_msg"])
        return [_errors objectAtIndex:row];

    if ([[theTableColumn identifier] isEqualToString: @"icon"])
        return [_icons objectAtIndex:row];

    return @"";
}

@end
