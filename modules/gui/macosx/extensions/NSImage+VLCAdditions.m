/*****************************************************************************
 * NSImage+VLCAdditions.m: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2023 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <developer@claudiocambra.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import "NSImage+VLCAdditions.h"

#import <QuickLook/QuickLook.h>

@implementation NSImage(VLCAdditions)

+ (instancetype)quickLookPreviewForLocalPath:(NSString *)path withSize:(NSSize)size
{
    NSURL *pathUrl = [NSURL fileURLWithPath:path];
    return [self quickLookPreviewForLocalURL:pathUrl withSize:size];
}

+ (instancetype)quickLookPreviewForLocalURL:(NSURL *)url withSize:(NSSize)size
{
    NSDictionary *dict = @{(NSString*)kQLThumbnailOptionIconModeKey : [NSNumber numberWithBool:NO]};
    CFDictionaryRef dictRef = CFBridgingRetain(dict);
    if (dictRef == NULL) {
        NSLog(@"Got null dict for quickLook preview");
        return nil;
    }

    CFURLRef urlRef = CFBridgingRetain(url);
    if (urlRef == NULL) {
        NSLog(@"Got null url ref for quickLook preview");
        CFRelease(dictRef);
        return nil;
    }

    CGImageRef qlThumbnailRef = QLThumbnailImageCreate(kCFAllocatorDefault,
                                                       urlRef,
                                                       size,
                                                       dictRef);

    CFRelease(dictRef);
    CFRelease(urlRef);

    if (qlThumbnailRef == NULL) {
        return nil;
    }

    NSBitmapImageRep *bitmapImageRep = [[NSBitmapImageRep alloc] initWithCGImage:qlThumbnailRef];
    if (bitmapImageRep == nil) {
        CFRelease(qlThumbnailRef);
        return nil;
    }

    NSImage *image = [[NSImage alloc] initWithSize:[bitmapImageRep size]];
    [image addRepresentation:bitmapImageRep];
    CFRelease(qlThumbnailRef);
    return image;
}

@end
