// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * logger.cpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#include "logger.hpp"


Logger::Logger( intf_thread_t *pIntf ): SkinObject( pIntf )
{
}


Logger::~Logger()
{
}


Logger *Logger::instance( intf_thread_t *pIntf )
{
    if( ! pIntf->p_sys->p_logger )
    {
        Logger *pLogger = new Logger( pIntf );
        if( pLogger )
        {
            // Initialization succeeded
            pIntf->p_sys->p_logger = pLogger;
        }
    }
    return pIntf->p_sys->p_logger;
}


void Logger::destroy( intf_thread_t *pIntf )
{
    delete pIntf->p_sys->p_logger;
    pIntf->p_sys->p_logger = NULL;
}


void Logger::error( const std::string &rMsg )
{
    msg_Err( getIntf(), "%s", rMsg.c_str() );
}


void Logger::warn( const std::string &rMsg )
{
    msg_Warn( getIntf(), "%s", rMsg.c_str() );
}

