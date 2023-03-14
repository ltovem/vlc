/*****************************************************************************
 * EPGChannels.hpp : EPGChannels
 ****************************************************************************
 * Copyright © 2009-2010 VideoLAN
 *
 * Authors: Adrien Maglo <magsoft@videolan.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef EPGCHANNELS_HPP
#define EPGCHANNELS_HPP

#include "qt.hpp"

#include <QWidget>

class EPGView;
class EPGProgram;

class EPGChannels : public QWidget
{
    Q_OBJECT
public:
    EPGChannels( QWidget *parent, EPGView *m_epgView );

public slots:
    void setOffset( int offset );
    void addProgram( const EPGProgram * );
    void reset();

protected:
    void paintEvent( QPaintEvent *event ) override;

private:
    EPGView *m_epgView;
    int m_offset;
    QList<const EPGProgram *> programsList;
};

#endif // EPGCHANNELS_HPP
