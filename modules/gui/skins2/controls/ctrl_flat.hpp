// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * ctrl_flat.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#ifndef CTRL_FLAT_HPP
#define CTRL_FLAT_HPP

#include "ctrl_generic.hpp"


/// Base class for "mover controls" and images
class CtrlFlat: public CtrlGeneric
{
protected:
    CtrlFlat( intf_thread_t *pIntf, const UString &rHelp, VarBool *pVisible )
            : CtrlGeneric( pIntf, rHelp, pVisible ) { }

    virtual ~CtrlFlat() { }
};

#endif
