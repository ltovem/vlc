/*****************************************************************************
 * animators.cpp: Object animators
 ****************************************************************************
 * Copyright (C) 2006-2014 the VideoLAN team
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#include "animators.hpp"
#include "qt.hpp"
#include "util/imagehelper.hpp"

#include <QAbstractItemView>

class QWidget;

BasicAnimator::BasicAnimator( QObject *parent )
    : QAbstractAnimation( parent ), current_frame( 0 )
{
    setFps( 15 );
    setLoopCount( -1 );
}

void BasicAnimator::updateCurrentTime( int msecs )
{
    msecs += (interval / 2);
    int i = ( msecs / interval );
    if ( i != current_frame )
    {
        current_frame = i;
        emit frameChanged();
    }
}

PixmapAnimator::PixmapAnimator(QWidget *parent, QList<QString> frames, int width , int height)
    : BasicAnimator( parent )
{
    foreach( QString name, frames )
        pixmaps.append( ImageHelper::loadSvgToPixmap( name, width, height ) );
    currentPixmap = pixmaps.at( 0 );
    setFps( frames.count() ); /* default to 1 sec loop */
}

PixmapAnimator::~PixmapAnimator()
{
}

void PixmapAnimator::updateCurrentTime( int msecs )
{
    int i = msecs / interval;
    if ( i >= pixmaps.count() ) i = pixmaps.count() - 1; /* roundings */
    if ( i != current_frame )
    {
        current_frame = i;
        currentPixmap = pixmaps.at( current_frame );
        emit pixmapReady( currentPixmap );
    }
}

DelegateAnimationHelper::DelegateAnimationHelper( QAbstractItemView *view_,
                                                  BasicAnimator *animator_ )
    : QObject( view_ ), view( view_ ), animator( animator_ )
{
    if ( !animator )
    {
        animator = new BasicAnimator( this );
        animator->setFps( 15 );
        animator->setLoopCount( -1 );
    }
    setIndex( QModelIndex() );
    connect( animator, &BasicAnimator::frameChanged, this, &DelegateAnimationHelper::updateDelegate );
}

void DelegateAnimationHelper::setIndex( const QModelIndex &index_ )
{
    index = QPersistentModelIndex( index_ );
}

void DelegateAnimationHelper::run( bool b_run )
{
    if ( b_run )
    {
        if ( ! isRunning() ) animator->start();
    }
    else
        animator->stop();
}

bool DelegateAnimationHelper::isRunning() const
{
    return ( animator->state() == QAbstractAnimation::Running );
}

const QPersistentModelIndex & DelegateAnimationHelper::getIndex() const
{
    return index;
}

void DelegateAnimationHelper::updateDelegate()
{
    /* Prevent running indefinitely if removed from model */
    if ( !index.isValid() )
        run( false );
    else
    {
        if ( view->viewport() )
            view->viewport()->update();
        else
            view->update( index );
    }
}
