// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * os_loop.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#ifndef OS_LOOP_HPP
#define OS_LOOP_HPP

#include "skin_common.hpp"


/// Abstract class for the main event loop
class OSLoop: public SkinObject
{
public:
    /// Enter the main loop
    virtual void run() = 0;

    /// Exit the main loop
    virtual void exit() = 0;

protected:
    OSLoop( intf_thread_t *pIntf ): SkinObject( pIntf ) { }
};


#endif
