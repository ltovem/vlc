/*****************************************************************************
* VLCCustomWindowButton.h: MacOS X interface module
*****************************************************************************
* Copyright (C) 2011-2019 VLC authors and VideoLAN
*
* Authors: Felix Paul Kühne <fkuehne # videolan -dot- org>
*          David Fuhrmann <dfuhrmann at videolan dot org>
*
* SPDX-License-Identifier: GPL-2.0-or-later
*****************************************************************************/

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface VLCWindowButtonCell : NSButtonCell

@end

@interface VLCCustomWindowButtonPrototype : NSButton

- (void)performDefaultButtonAction:(id)sender;

@end

@interface VLCCustomWindowCloseButton : VLCCustomWindowButtonPrototype

@end


@interface VLCCustomWindowMinimizeButton : VLCCustomWindowButtonPrototype

@end


@interface VLCCustomWindowZoomButton : VLCCustomWindowButtonPrototype

@end

@interface VLCCustomWindowFullscreenButton : VLCCustomWindowButtonPrototype

@end

@interface VLCCustomEmptyLibraryBrowseButton : NSButton

@end

NS_ASSUME_NONNULL_END
