/*****************************************************************************
 * VLCMediaSource.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan -dot- org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import <Foundation/Foundation.h>

#import <vlc_media_source.h>

@class VLCInputItem;
@class VLCInputNode;

NS_ASSUME_NONNULL_BEGIN

extern NSString *VLCMediaSourceChildrenReset;
extern NSString *VLCMediaSourceChildrenAdded;
extern NSString *VLCMediaSourceChildrenRemoved;
extern NSString *VLCMediaSourcePreparsingEnded;

@interface VLCMediaSource : NSObject

- (instancetype)initForLocalDevices:(libvlc_int_t *)p_libvlcInstance;
- (instancetype)initWithMediaSource:(vlc_media_source_t *)p_mediaSource
                  andLibVLCInstance:(libvlc_int_t *)p_libvlcInstance
                        forCategory:(enum services_discovery_category_e)category;

- (void)preparseInputNodeWithinTree:(VLCInputNode *)inputNode;
- (void)clearChildNodesForNode:(input_item_node_t*)inputNode;

@property (nonatomic, readonly) NSString *mediaSourceDescription;
@property (nonatomic, readonly) VLCInputNode *rootNode;
@property (nonatomic, readonly) enum services_discovery_category_e category;

@end

NS_ASSUME_NONNULL_END
