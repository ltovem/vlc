/*****************************************************************************
 * EPGView.hpp : EPGView
 ****************************************************************************
 * Copyright © 2009-2010 VideoLAN
 *
 * Authors: Ludovic Fauvet <etix@l0cal.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef EPGVIEW_H
#define EPGVIEW_H

#include "qt.hpp"

#include "EPGProgram.hpp"

#include <vlc_epg.h>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QHash>
#include <QDateTime>

class EPGItem;

#define TRACKS_HEIGHT 60

class EPGGraphicsScene : public QGraphicsScene
{
Q_OBJECT
public:
    explicit EPGGraphicsScene( QObject *parent = 0 );
protected:
    void drawBackground ( QPainter *, const QRectF &);
};

class EPGView : public QGraphicsView
{
Q_OBJECT

public:
    explicit EPGView( QWidget *parent = 0 );
    virtual ~EPGView();

    void            setScale( double scaleFactor );

    const QDateTime& startTime() const;
    QDateTime       epgTime() const;
    void            setEpgTime(const QDateTime&);

    bool            updateEPG( const vlc_epg_t * const *, size_t );
    void            reset();
    void            cleanup();
    bool            hasValidData() const;
    void            activateProgram( int );

signals:
    void            rangeChanged( const QDateTime&, const QDateTime& );
    void            itemFocused( EPGItem * );
    void            programAdded( const EPGProgram * );
    void            programActivated( int );

protected:
    void            walkItems( bool );
    QDateTime       m_epgTime;
    QDateTime       m_startTime;
    QDateTime       m_maxTime;
    QDateTime       m_updtMinTime; /* >= startTime before pruning */
    int             m_scaleFactor;
    int             m_duration;

public slots:
    void            focusItem( EPGItem * );

private:
    QHash<uint16_t, EPGProgram*> programs;
};

#endif // EPGVIEW_H
