/*****************************************************************************
 * VLCOpenInputMetadata.h: macOS interface
 *****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan dot org>
 *          Marvin Scholz <epirat07 at videolan dot org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# import <config.h>
#endif

#import <vlc_common.h>
#import <vlc_url.h>

#import "VLCOpenInputMetadata.h"

@implementation VLCOpenInputMetadata

+ (instancetype)inputMetaWithPath:(NSString *)path
{
    return [[VLCOpenInputMetadata alloc] initWithPath:path];
}

- (instancetype)initWithPath:(NSString *)path
{
    if (!path)
        return nil;

    self = [super init];
    if (!self)
        return nil;

    char *vlc_uri_psz = vlc_path2uri([path UTF8String], "file");
    if (!vlc_uri_psz)
        return nil;

    NSString *vlc_uri = [NSString stringWithUTF8String:vlc_uri_psz];
    FREENULL(vlc_uri_psz);

    if (!vlc_uri)
        return nil;

    self.MRLString = vlc_uri;
    return self;
}

@end
