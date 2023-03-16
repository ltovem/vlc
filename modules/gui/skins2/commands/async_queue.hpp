/*****************************************************************************
 * async_queue.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef ASYNC_QUEUE_HPP
#define ASYNC_QUEUE_HPP

#include "cmd_generic.hpp"

#include <list>
#include <string>
#include <memory>

#include <vlc_cxx_helpers.hpp>

class OSTimer;


/// Asynchronous queue for commands
class AsyncQueue: public SkinObject
{
public:
    /// Get the instance of AsyncQueue
    /// Returns NULL if initialization failed.
    static AsyncQueue *instance( intf_thread_t *pIntf );

    /// Destroy the instance of AsyncQueue
    static void destroy( intf_thread_t *pIntf );

    /// Add a command in the queue, after having removed the commands
    /// of the same type already in the queue if needed
    void push( const CmdGenericPtr &rcCommand, bool removePrev = true );

    /// Remove the commands of the given type
    void remove( const std::string &rType , const CmdGenericPtr &rcCommand );

    /// Flush the queue and execute the commands
    void flush();

private:
    /// Command queue
    using cmdList_t = std::list<CmdGenericPtr>;
    cmdList_t m_cmdList;
    /// Timer
    std::unique_ptr<OSTimer> m_pTimer;
    /// Mutex
    vlc::threads::mutex m_lock;

    // Private because it is a singleton
    AsyncQueue( intf_thread_t *pIntf );

    // Callback to flush the queue
    DEFINE_CALLBACK( AsyncQueue, Flush );
};


#endif
