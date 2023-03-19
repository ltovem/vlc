// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * async_queue.cpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#include "async_queue.hpp"
#include "../src/os_factory.hpp"
#include "../src/os_timer.hpp"

#include <new>

AsyncQueue::AsyncQueue( intf_thread_t *pIntf ): SkinObject( pIntf ),
    m_cmdFlush( this )
{
    // Create a timer
    OSFactory *pOsFactory = OSFactory::instance( pIntf );
    m_pTimer.reset(pOsFactory->createOSTimer(m_cmdFlush));

    // Flush the queue every 10 ms
    m_pTimer->start( 10, false );
}

AsyncQueue *AsyncQueue::instance( intf_thread_t *pIntf )
{
    if( ! pIntf->p_sys->p_queue )
    {
        AsyncQueue *pQueue;
        pQueue = new (std::nothrow) AsyncQueue( pIntf );
        if( pQueue )
        {
             // Initialization succeeded
             pIntf->p_sys->p_queue = pQueue;
        }
     }
     return pIntf->p_sys->p_queue;
}


void AsyncQueue::destroy( intf_thread_t *pIntf )
{
    delete pIntf->p_sys->p_queue;
    pIntf->p_sys->p_queue = NULL;
}


void AsyncQueue::push( const CmdGenericPtr &rcCommand, bool removePrev )
{
    vlc::threads::mutex_locker guard {m_lock};

    if( removePrev )
    {
        // Remove the commands of the same type
        remove( rcCommand.get()->getType(), rcCommand );
    }
    m_cmdList.push_back( rcCommand );
}


void AsyncQueue::remove( const std::string &rType, const CmdGenericPtr &rcCommand )
{
    cmdList_t::iterator it;
    for( it = m_cmdList.begin(); it != m_cmdList.end(); /* nothing */ )
    {
        // Remove the command if it is of the given type and the command
        // doesn't disagree. Note trickery to avoid skipping entries
        // while maintaining iterator validity.

        if( (*it).get()->getType() == rType &&
            rcCommand.get()->checkRemove( (*it).get() ) )
        {
            cmdList_t::iterator itNew = it;
            ++itNew;
            m_cmdList.erase( it );
            it = itNew;
        }
        else ++it;
    }
}


void AsyncQueue::flush()
{
    while (!m_cmdList.empty())
    {
        CmdGenericPtr cCommand;
        {
            vlc::threads::mutex_locker guard {m_lock};
            // Pop the first command from the queue
            cCommand = m_cmdList.front();
            m_cmdList.pop_front();
        }

        // Unlock the mutex to avoid deadlocks if another thread wants to
        // enqueue/remove a command while this one is processed

        // Execute the command
        cCommand.get()->execute();
    }
}


void AsyncQueue::CmdFlush::execute()
{
    // Flush the queue
    m_pParent->flush();
}
