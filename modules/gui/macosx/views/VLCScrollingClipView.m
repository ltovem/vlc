/*****************************************************************************
 * VLCScrollingClipView.m: NSClipView subclass that automatically scrolls
 *****************************************************************************
 * Copyright (C) 2015 VLC authors and VideoLAN
 *
 * Author: Marvin Scholz <epirat07@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import "VLCScrollingClipView.h"

@implementation VLCScrollingClipView {
    NSTimer *scrollTimer;
    NSTimeInterval startInterval;
}

- (void)startScrolling {
    // Start our timer unless running
    if (!scrollTimer) {
        scrollTimer = [NSTimer scheduledTimerWithTimeInterval:0.025
                                                       target:self
                                                     selector:@selector(scrollTick:)
                                                     userInfo:nil
                                                      repeats:YES];
    }
}

- (void)stopScrolling {
    // Stop timer unless stopped
    if (scrollTimer) {
        [scrollTimer invalidate];
        scrollTimer = nil;
    }
}

- (void)resetScrolling {
    startInterval = 0;
    [self scrollToPoint:NSMakePoint(0.0, 0.0)];
    if (_parentScrollView) {
        [_parentScrollView reflectScrolledClipView:self];
    }
}

- (void)scrollWheel:(NSEvent *)theEvent {
    // Stop auto-scrolling if the user scrolls
    [self stopScrolling];
    [super scrollWheel:theEvent];
}

- (void)scrollTick:(NSTimer *)timer {
    CGFloat maxHeight = self.documentRect.size.height;
    CGFloat scrollPosition = self.documentVisibleRect.origin.y;

    // Delay start
    if (scrollPosition == 0.0 && !startInterval) {
        startInterval = [NSDate timeIntervalSinceReferenceDate] + 2.0;
    }

    if ([NSDate timeIntervalSinceReferenceDate] >= startInterval) {
        // Reset if we are past the end
        if (scrollPosition > maxHeight) {
            [self resetScrolling];
            return;
        }

        // Scroll the view a bit on each tick
        [self scrollToPoint:NSMakePoint(0.0, scrollPosition + 0.5)];

        // Update the parent ScrollView
        if (_parentScrollView) {
            [_parentScrollView reflectScrolledClipView:self];
        }
    }
}

@end
