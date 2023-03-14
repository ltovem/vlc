/*****************************************************************************
 *
 *****************************************************************************
 * Copyright (C) 2020 VideoLabs, VideoLAN and VLC Authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "../../tools/Conversions.hpp"

#include "../test.hpp"

#include <limits>

int Conversions_test()
{
    UTCTime utc("1970-01-01T00:00:00.000+00:00");
    Expect(utc.mtime() == 0);
    utc = UTCTime("1970-01-01T10:00:00.000+10:00");
    Expect(utc.mtime() == 0);
    utc = UTCTime("1970-01-01T02:00:00.000+01:02");
    Expect(utc.mtime() == vlc_tick_from_sec(7200 - 3720));
    utc = UTCTime("1970-01-01T02:00:00.000+0102");
    Expect(utc.mtime() == vlc_tick_from_sec(7200 - 3720));
    utc = UTCTime("1970-01-01T01:10:00.000+1");
    Expect(utc.mtime() == vlc_tick_from_sec(10*60));
    utc = UTCTime("2019-06-11Z");
    Expect(utc.mtime() == vlc_tick_from_sec(1560211200));
    utc = UTCTime("2019-06-11T16:52:05.100Z");
    Expect(utc.mtime() == vlc_tick_from_sec(1560271925) + VLC_TICK_FROM_MS(100));
    utc = UTCTime("2019-06-11T16:52:05.012Z");
    Expect(utc.mtime() == vlc_tick_from_sec(1560271925) + VLC_TICK_FROM_MS(12));
    utc = UTCTime("T16:52:05.012Z");

    /* Check non ms fractional */
    utc = UTCTime("2021-05-28T12:51:32+00:00");
    Expect(utc.mtime() == vlc_tick_from_sec(1622206292));
    utc = UTCTime("2021-05-28T12:51:32.996000+00:00");
    Expect(utc.mtime() == vlc_tick_from_sec(1622206292)+VLC_TICK_FROM_MS(996));

    IsoTime isotime("PT0H9M56.46S");
    Expect(isotime == (vlc_tick_from_sec(9*60+56)+VLC_TICK_FROM_MS(460)));
    isotime = IsoTime("HELLO");
    Expect(isotime == -1);
    isotime = IsoTime("P1D");
    Expect(isotime == vlc_tick_from_sec(86400));
    isotime = IsoTime("PT2.5M");
    Expect(isotime == vlc_tick_from_sec(150));
    isotime = IsoTime("PT");
    Expect(isotime == 0);
    isotime = IsoTime("PT.5S");
    Expect(isotime == VLC_TICK_FROM_MS(500));
    isotime = IsoTime("PT.010S");
    Expect(isotime == VLC_TICK_FROM_MS(10));

    return 0;
}
