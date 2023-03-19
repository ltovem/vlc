// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * logger.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "skin_common.hpp"
#include <string>


// helper macros
#define MSG_ERR( msg ) Logger::instance( getIntf() )->error( msg )
#define MSG_WARN( msg ) Logger::instance( getIntf() )->warn( msg )


// Logger class
class Logger: public SkinObject
{
public:
    /// Get the instance of Logger
    /// Returns NULL if initialization failed
    static Logger *instance( intf_thread_t *pIntf );

    /// Delete the instance of Logger
    static void destroy( intf_thread_t *pIntf );

    /// Print an error message
    void error( const std::string &rMsg );

    /// Print a warning
    void warn( const std::string &rMsg );

private:
    // Private because it's a singleton
    Logger( intf_thread_t *pIntf );
    ~Logger();
};


#endif
