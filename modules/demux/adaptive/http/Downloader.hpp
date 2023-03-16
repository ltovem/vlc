// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * Downloader.hpp
 *****************************************************************************
 Copyright (C) 2015 - VideoLAN Authors
 *****************************************************************************/
#ifndef DOWNLOADER_HPP
#define DOWNLOADER_HPP

#include "Chunk.h"

#include <vlc_common.h>
#include <vlc_cxx_helpers.hpp>
#include <list>

namespace adaptive
{

    namespace http
    {

        class Downloader
        {
            public:
                Downloader();
                ~Downloader();
                bool start();
                void schedule(HTTPChunkBufferedSource *);
                void cancel(HTTPChunkBufferedSource *);

            private:
                static void * downloaderThread(void *);
                void Run();
                void kill();
                vlc_thread_t thread_handle;
                vlc::threads::mutex lock;
                vlc::threads::condition_variable wait_cond;
                vlc::threads::condition_variable updated_cond;
                bool         thread_handle_valid;
                bool         killed;
                bool         cancel_current;
                std::list<HTTPChunkBufferedSource *> chunks;
                HTTPChunkBufferedSource *current;
        };

    }

}

#endif // DOWNLOADER_HPP
