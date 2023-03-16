// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * DBMHelper.hpp: Decklink SDI Helpers
 *****************************************************************************
 * Copyright © 2014-2016 VideoLAN and VideoLAN Authors
 *             2018-2019 VideoLabs
 *****************************************************************************/
#ifndef DBMHELPER_HPP
#define DBMHELPER_HPP

namespace Decklink
{
    class Helper
    {
        public:
            static IDeckLinkDisplayMode * MatchDisplayMode(vlc_object_t *,
                                                           IDeckLinkOutput *,
                                                           const video_format_t *,
                                                           BMDDisplayMode = bmdModeUnknown);
            static const char *ErrorToString(long i_code);
    };
}

#endif
