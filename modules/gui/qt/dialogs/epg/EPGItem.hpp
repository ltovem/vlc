/*****************************************************************************
 * EPGItem.hpp : EPGItem
 ****************************************************************************
 * Copyright © 2009-2010 VideoLAN
 *
 * Authors: Ludovic Fauvet <etix@l0cal.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef EPGITEM_H
#define EPGITEM_H

#include "qt.hpp"

#include <vlc_epg.h>
#include <QGraphicsItem>
#include <QDateTime>

class QPainter;
class QString;
class EPGView;
class EPGProgram;

class EPGItem : public QGraphicsItem
{
public:
    EPGItem( const vlc_epg_event_t *data, EPGView *view, EPGProgram * );

    QRectF boundingRect() const override;
    void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0 ) override;

    const QDateTime& start() const;
    QDateTime end() const;

    uint32_t duration() const;
    uint16_t eventID() const;
    const QString& name() const { return m_name; }
    QString description() const;
    int rating() const { return m_rating; }
    bool setData( const vlc_epg_event_t * );
    void setDuration( uint32_t duration );
    void setRating( uint8_t i_rating );
    void updatePos();
    bool endsBefore( const QDateTime & ) const;
    bool playsAt( const QDateTime & ) const;
    const QList<QPair<QString, QString>> &descriptionItems() const;

protected:
    void focusInEvent( QFocusEvent * event ) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * ) override;
    void hoverEnterEvent ( QGraphicsSceneHoverEvent * ) override;
    void hoverLeaveEvent ( QGraphicsSceneHoverEvent * ) override;

private:
    EPGProgram *program;
    EPGView     *m_view;
    QRectF      m_boundingRect;

    QDateTime   m_start;
    uint32_t    m_duration;
    uint16_t    m_id;
    QString     m_name;
    QString     m_description;
    QString     m_shortDescription;
    QList<QPair<QString, QString>> m_descitems;
    uint8_t     m_rating;
};

#endif // EPGITEM_H
