// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * test.hpp
 *****************************************************************************
 * Copyright (C) 2020 VideoLabs, VideoLAN and VLC Authors
 *****************************************************************************/
#ifndef ADAPTIVE_TEST_H
#define ADAPTIVE_TEST_H
#include <exception>
#include <iostream>

#define DoExpect(testcond, testcontext, testline) \
    try {\
        if (!(testcond)) \
            throw 1;\
    }\
    catch (...)\
    {\
        std::cerr << testcontext << ": failed " \
            " line " << testline << std::endl;\
        std::rethrow_exception(std::current_exception());\
    }

#define Expect(testcond) DoExpect((testcond), __FUNCTION__, __LINE__)

int Inheritables_test();
int TemplatedUri_test();
int SegmentBase_test();
int SegmentList_test();
int SegmentTemplate_test();
int Timeline_test();
int Conversions_test();
int M3U8MasterPlaylist_test();
int M3U8Playlist_test();
int CommandsQueue_test();
int BufferingLogic_test();
int FakeEsOut_test();
int SegmentTracker_test();

#endif
