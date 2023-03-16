// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * EPGProgram.hpp:
 *****************************************************************************
 * Copyright © 2016 VideoLAN Authors
 *****************************************************************************/
#ifndef EPGPROGRAM_HPP
#define EPGPROGRAM_HPP

#include "qt.hpp"

#include <vlc_epg.h>
#include <QObject>
#include <QMap>
#include <QHash>
#include <QDateTime>

class EPGView;
class EPGItem;

class EPGProgram : public QObject
{
    Q_OBJECT

    public:
        EPGProgram( EPGView *, const vlc_epg_t * );
        virtual ~EPGProgram();

        void pruneEvents( const QDateTime & );
        void updateEvents( const vlc_epg_event_t * const *, size_t, const vlc_epg_event_t *,
                           QDateTime * );
        void updateEventPos();
        size_t getPosition() const;
        void setPosition( size_t );
        void activate();
        const EPGItem * getCurrent() const;
        const QString & getName() const;
        static bool lessThan( const EPGProgram *, const EPGProgram * );

        QHash<uint32_t, EPGItem *> eventsbyid;
        QMap<QDateTime, const EPGItem *> eventsbytime;

    private:
        const EPGItem *current;
        EPGView *view;
        size_t pos;

        QString name;
        uint32_t id;
        uint16_t sourceid;
};

#endif // EPGPROGRAM_HPP
