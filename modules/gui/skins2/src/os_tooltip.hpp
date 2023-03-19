// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * os_tooltip.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#ifndef OS_TOOLTIP_HPP
#define OS_TOOLTIP_HPP

#include "skin_common.hpp"

class OSGraphics;


/// Base class for OS specific Tooltip Windows
class OSTooltip: public SkinObject
{
public:
    virtual ~OSTooltip() { }

    /// Show the tooltip
    virtual void show( int left, int top, OSGraphics &rText ) = 0;

    /// Hide the tooltip
    virtual void hide() = 0;

protected:
    OSTooltip( intf_thread_t *pIntf ): SkinObject( pIntf ) { }
};

#endif
