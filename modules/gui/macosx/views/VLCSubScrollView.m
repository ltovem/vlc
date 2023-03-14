/*****************************************************************************
 * VLCSubScrollView.h: MacOS X interface module
 *****************************************************************************
 *
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <claudio.cambra@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import "VLCSubScrollView.h"

@implementation VLCSubScrollView

- (instancetype)init
{
    self = [super init];

    if(self) {
        _scrollParentX = NO;
        _scrollParentY = NO;
        _scrollSelf = YES;
    }

    return self;
}

- (void)scrollWheel:(NSEvent *)event
{
    if (!_scrollSelf) {
        [_parentScrollView scrollWheel:event];
        return;
    }

    [super scrollWheel:event];

    if(_parentScrollView == nil || (!_scrollParentX && !_scrollParentY)) {
        return;
    }

    // Sometimes scroll views initialise with the Y value being almost 0, but not quite (e.g. 0.000824)
    const BOOL isViewAtYStartAndScrollUp = self.verticalScroller.floatValue <= 0.01 && event.deltaY > 0;
    const BOOL isViewAtYEndAndScrollDown = self.verticalScroller.floatValue >= 0.99 && event.deltaY < 0;
    const BOOL isViewAtXStartAndScrollLeft = self.horizontalScroller.floatValue <= 0.01 && event.deltaX > 0;
    const BOOL isViewAtXEndAndScrollRight = self.horizontalScroller.floatValue >= 0.99 && event.deltaX < 0;

    const BOOL isSubScrollViewScrollableY = self.documentView.frame.size.height > self.documentVisibleRect.size.height;
    const BOOL isSubScrollViewScrollableX = self.documentView.frame.size.width > self.documentVisibleRect.size.width;

    const BOOL shouldScrollParentY = _scrollParentY && (!isSubScrollViewScrollableY || isViewAtYStartAndScrollUp || isViewAtYEndAndScrollDown);
    const BOOL shouldScrollParentX = _scrollParentX && (!isSubScrollViewScrollableX || isViewAtXStartAndScrollLeft || isViewAtXEndAndScrollRight);

    if(shouldScrollParentY || shouldScrollParentX) {
        [_parentScrollView scrollWheel:event];
    }
}

- (BOOL)hasVerticalScroller
{
    if (_forceHideVerticalScroller) {
        return NO;
    }

    return [super hasVerticalScroller];
}

- (BOOL)hasHorizontalScroller
{
    if (_forceHideHorizontalScroller) {
        return NO;
    }

    return [super hasHorizontalScroller];
}

@end
