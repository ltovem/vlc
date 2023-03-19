// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCTrackingView.m: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan -dot- org>
 *****************************************************************************/

#import "VLCTrackingView.h"

@interface VLCTrackingView ()
{
    NSTrackingArea *_trackingArea;
}
@end

@implementation VLCTrackingView

- (void)mouseExited:(NSEvent *)event
{
    if (self.animatesTransition) {
        [self.viewToHide setAlphaValue:1.0];
        [self.viewToShow setAlphaValue:.0];
        [self.viewToShow setHidden:NO];

        __weak typeof(self.viewToHide) weakViewToHide = self.viewToHide;
        __weak typeof(self.viewToShow) weakViewToShow = self.viewToShow;

        [NSAnimationContext runAnimationGroup:^(NSAnimationContext *context){
            [[NSAnimationContext currentContext] setDuration:0.9];
            [[weakViewToHide animator] setAlphaValue:0.0];
            [[weakViewToShow animator] setAlphaValue:1.0];
        } completionHandler:^{
            [weakViewToHide setHidden:YES];
        }];
    } else {
        self.viewToHide.hidden = YES;
        self.viewToShow.hidden = NO;
    }
}

- (void)mouseEntered:(NSEvent *)event
{
    if (self.animatesTransition) {
        [self.viewToHide setAlphaValue:.0];
        [self.viewToHide setHidden:NO];

        __weak typeof(self.viewToHide) weakViewToHide = self.viewToHide;
        __weak typeof(self.viewToShow) weakViewToShow = self.viewToShow;

        [NSAnimationContext runAnimationGroup:^(NSAnimationContext *context){
            [[NSAnimationContext currentContext] setDuration:0.9];
            [[weakViewToHide animator] setAlphaValue:1.0];
            [[weakViewToShow animator] setAlphaValue:.0];
        } completionHandler:^{
            [weakViewToShow setHidden:YES];
        }];
    } else {
        self.viewToHide.hidden = NO;
        self.viewToShow.hidden = YES;
    }
}

- (void)updateTrackingAreas
{
    [super updateTrackingAreas];
    if(_trackingArea != nil) {
        [self removeTrackingArea:_trackingArea];
    }

    NSTrackingAreaOptions trackingAreaOptions = (NSTrackingMouseEnteredAndExited | NSTrackingActiveAlways);
    _trackingArea = [[NSTrackingArea alloc] initWithRect:[self bounds]
                                                 options:trackingAreaOptions
                                                   owner:self
                                                userInfo:nil];
    [self addTrackingArea:_trackingArea];
}

@end
