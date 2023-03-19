// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * Downloader.hpp
 *****************************************************************************
 Copyright (C) 2015 - VideoLAN Authors
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "Downloader.hpp"

#include <vlc_threads.h>

#include <atomic>

using namespace adaptive::http;

Downloader::Downloader()
{
    killed = false;
    thread_handle_valid = false;
    current = nullptr;
    cancel_current = false;
}

bool Downloader::start()
{
    if(!thread_handle_valid &&
       vlc_clone(&thread_handle, downloaderThread, static_cast<void *>(this)))
    {
        return false;
    }
    thread_handle_valid = true;
    return true;
}

Downloader::~Downloader()
{
    kill();

    if(thread_handle_valid)
        vlc_join(thread_handle, nullptr);
}

void Downloader::kill()
{
    vlc::threads::mutex_locker locker {lock};
    killed = true;
    wait_cond.signal();
}

void Downloader::schedule(HTTPChunkBufferedSource *source)
{
    vlc::threads::mutex_locker locker {lock};
    source->hold();
    chunks.push_back(source);
    wait_cond.signal();
}

void Downloader::cancel(HTTPChunkBufferedSource *source)
{
    vlc::threads::mutex_locker locker {lock};
    while (current == source)
    {
        cancel_current = true;
        updated_cond.wait(lock);
    }

    if(!source->isDone())
    {
        chunks.remove(source);
        source->release();
    }
}

void * Downloader::downloaderThread(void *opaque)
{
    vlc_thread_set_name("vlc-adapt-dl");
    Downloader *instance = static_cast<Downloader *>(opaque);
    instance->Run();
    return nullptr;
}

void Downloader::Run()
{
    while(1)
    {
        lock.lock();

        while(chunks.empty() && !killed)
            wait_cond.wait(lock);

        if(killed)
        {
            lock.unlock();
            break;
        }

        current = chunks.front();
        lock.unlock();
        current->bufferize(HTTPChunkSource::CHUNK_SIZE);
        lock.lock();
        if(current->isDone() || cancel_current)
        {
            chunks.pop_front();
            current->release();
        }
        cancel_current = false;
        current = nullptr;
        updated_cond.signal();
        lock.unlock();
    }
}
