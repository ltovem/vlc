/*****************************************************************************
 * animators.hpp: Object animators
 ****************************************************************************
 * Copyright (C) 2006-2014 the VideoLAN team
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef ANIMATORS_HPP
#define ANIMATORS_HPP

#include "qt.hpp"

#include <QObject>
#include <QList>
#include <QString>
#include <QAbstractAnimation>
#include <QPixmap>
#include <QPersistentModelIndex>

class QWidget;
class QAbstractItemView;

class BasicAnimator : public QAbstractAnimation
{
    Q_OBJECT

public:
    BasicAnimator( QObject *parent = 0 );
    void setFps( int _fps ) { fps = _fps; interval = 1000.0 / fps; }
    int duration() const override { return 1000; }

signals:
    void frameChanged();

protected:
    void updateCurrentTime ( int msecs ) override;
    int fps;
    int interval;
    int current_frame;
};

/** An animated pixmap
     * Use this widget to display an animated icon based on a series of
     * pixmaps. The pixmaps will be stored in memory and should be kept small.
     * First, create the widget, add frames and then start playing. Looping
     * is supported.
     **/
class PixmapAnimator : public BasicAnimator
{
    Q_OBJECT

public:
    PixmapAnimator(QWidget *parent, QList<QString> _frames , int width, int height);
    int duration() const override { return interval * pixmaps.count(); }
    virtual ~PixmapAnimator();
    const QPixmap& getPixmap() { return currentPixmap; }
protected:
    void updateCurrentTime ( int msecs ) override;
    QList<QPixmap> pixmaps;
    QPixmap currentPixmap;
signals:
    void pixmapReady( const QPixmap & );
};

class DelegateAnimationHelper : public QObject
{
    Q_OBJECT

public:
    DelegateAnimationHelper( QAbstractItemView *view, BasicAnimator *animator = 0 );
    void setIndex( const QModelIndex &index );
    bool isRunning() const;
    const QPersistentModelIndex & getIndex() const;

public slots:
    void run( bool );

protected slots:
    void updateDelegate();

private:
    QAbstractItemView *view;
    BasicAnimator *animator;
    QPersistentModelIndex index;
};

#endif // ANIMATORS_HPP
