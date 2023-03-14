/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
#ifndef VLCTICK_HPP
#define VLCTICK_HPP

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <QObject>
#include <vlc_common.h>
#include <vlc_tick.h>

class VLCTick
{
    Q_GADGET
public:
    VLCTick();
    VLCTick(vlc_tick_t ticks);

    operator vlc_tick_t() const;

    Q_INVOKABLE bool valid() const;

    /**
     * @brief formatHMS
     * @return time as HH:MM:SS
     */
    Q_INVOKABLE QString formatHMS() const;

    /**
     * @brief formatLong
     * @return time in literal form
     * 1h 2min
     * 5 min
     * 10 sec
     * 43 ms
     */
    Q_INVOKABLE QString formatLong() const;

    /**
     * @brief formatShort
     * @return time in literal form
     * 1h02
     * 02:42
     * 43 ms
     */
    Q_INVOKABLE QString formatShort() const;


    Q_INVOKABLE VLCTick scale(float) const;

    Q_INVOKABLE int toMinutes() const;
    Q_INVOKABLE int toSeconds() const;
    Q_INVOKABLE int toHours()   const;


    static VLCTick fromMS(int64_t ms);
private:
    vlc_tick_t m_ticks;
};

Q_DECLARE_METATYPE(VLCTick)

#endif // VLCTICK_HPP
