// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * test.cpp
 *****************************************************************************
 * Copyright (C) 2018-2020 VideoLabs, VideoLAN and VLC Authors
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include <vlc_plugin.h>
#include <vlc_demux.h>

#include "test.hpp"

#include <iostream>

extern const char vlc_module_name[] = "foobar";

#define TEST(func) []() { std::cerr << "Testing "#func << std::endl;\
                          return func##_test(); }()

int main()
{
    return
    TEST(FakeEsOut) ||
    TEST(Inheritables) ||
    TEST(SegmentBase) ||
    TEST(SegmentList) ||
    TEST(SegmentTemplate) ||
    TEST(Timeline) ||
    TEST(Conversions) ||
    TEST(TemplatedUri) ||
    TEST(BufferingLogic) ||
    TEST(CommandsQueue) ||
    TEST(M3U8MasterPlaylist) ||
    TEST(M3U8Playlist) ||
    TEST(SegmentTracker)
    ;
}
