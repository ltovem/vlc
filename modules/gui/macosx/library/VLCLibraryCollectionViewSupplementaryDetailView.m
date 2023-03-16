// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCLibraryCollectionViewSupplementaryDetailView.m: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <claudio.cambra@gmail.com>
 *****************************************************************************/

#import "VLCLibraryCollectionViewSupplementaryDetailView.h"
#import "views/VLCSubScrollView.h"

static const CGFloat kArrowHeight = 20.;
static const CGFloat kArrowWidth = 50.;
static const CGFloat kArrowTipRadius = 2.5;
static const CGFloat kBackgroundCornerRadius = 10.;

@interface VLCLibraryCollectionViewSupplementaryDetailView ()
{
    NSSize _arrowSize;
}
@end

@implementation VLCLibraryCollectionViewSupplementaryDetailView

- (void)drawRect:(NSRect)dirtyRect {
    if(NSEqualSizes(_arrowSize, NSZeroSize)) {
        _arrowSize = NSMakeSize(kArrowWidth, kArrowHeight);
    }

    const NSRect selectedItemFrame = _selectedItem.view.frame;

    const NSPoint itemCenterPoint = NSMakePoint(NSMinX(selectedItemFrame) + NSWidth(selectedItemFrame) / 2,
                                                NSMinY(selectedItemFrame) + NSHeight(selectedItemFrame) / 2);
    const NSRect backgroundRect = NSMakeRect(NSMinX(self.bounds),
                                             NSMinY(self.bounds), NSWidth(self.bounds) + 2,
                                             NSHeight(self.bounds) - _arrowSize.height);
    const CGFloat backgroundTop = NSMaxY(backgroundRect);
    const CGFloat backgroundLeft = NSMinX(backgroundRect);

    const NSPoint arrowLeftPoint = NSMakePoint(itemCenterPoint.x - _arrowSize.width / 2, backgroundTop);
    const NSPoint arrowTopPoint = NSMakePoint(itemCenterPoint.x, backgroundTop + kArrowHeight - 1);
    const NSPoint arrowRightPoint = NSMakePoint(itemCenterPoint.x + _arrowSize.width / 2, backgroundTop);

    const NSPoint topLeftCorner = NSMakePoint(backgroundLeft, backgroundTop);
    const NSPoint topLeftCornerAfterCurve = NSMakePoint(backgroundLeft + kBackgroundCornerRadius, backgroundTop);

    const NSBezierPath *backgroundPath = [NSBezierPath bezierPathWithRoundedRect:backgroundRect xRadius:kBackgroundCornerRadius yRadius:kBackgroundCornerRadius];

    [backgroundPath moveToPoint:topLeftCornerAfterCurve];
    [backgroundPath lineToPoint:arrowLeftPoint];
    [backgroundPath curveToPoint:arrowTopPoint
                   controlPoint1:NSMakePoint(itemCenterPoint.x - _arrowSize.width / 6, backgroundTop)
                   controlPoint2:NSMakePoint(itemCenterPoint.x - kArrowTipRadius, backgroundTop + _arrowSize.height)];
    [backgroundPath curveToPoint:arrowRightPoint
                   controlPoint1:NSMakePoint(itemCenterPoint.x + kArrowTipRadius, backgroundTop + _arrowSize.height)
                   controlPoint2:NSMakePoint(itemCenterPoint.x + _arrowSize.width / 6, backgroundTop)];

    [backgroundPath closePath];

    //[[NSColor.gridColor colorWithAlphaComponent:self.container.alphaValue] setFill];
    [NSColor.gridColor setFill];
    [backgroundPath fill];

    //[[NSColor.gridColor colorWithAlphaComponent:self.container.alphaValue] setStroke];
    [NSColor.gridColor setStroke];
    [backgroundPath stroke];
}

- (NSScrollView *)parentScrollView
{
    if(_internalScrollView == nil) {
        return nil;
    }

    return _internalScrollView.parentScrollView;
}

- (void)setParentScrollView:(NSScrollView *)parentScrollView
{
    if(_internalScrollView == nil) {
        NSLog(@"Library collection view supplementary view has no internal scroll view -- cannot set parent scrollview.");
        return;
    }

    _internalScrollView.parentScrollView = parentScrollView;
}

@end

