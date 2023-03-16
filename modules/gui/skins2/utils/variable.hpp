// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * variable.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#ifndef VARIABLE_HPP
#define VARIABLE_HPP

#include "../src/skin_common.hpp"
#include "pointer.hpp"
#include <string>


/// Base class for variable objects
class Variable: public SkinObject
{
public:
    virtual ~Variable() { }

    /// Get the variable type
    virtual const std::string &getType() const = 0;

protected:
    Variable( intf_thread_t *pIntf ): SkinObject( pIntf ) { }
};


typedef CountedPtr<Variable> VariablePtr;


#endif
