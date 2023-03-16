/*****************************************************************************
 * var_string.hpp
 *****************************************************************************
 * Copyright (C) 2010 the VideoLAN team
 *
 * Author: Erwan Tulou      <erwan10 aT videolan DoT org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef VAR_STRING
#define VAR_STRING

#include "variable.hpp"
#include "observer.hpp"

class VarString;


/// String variable
class VarString: public Variable, public Subject<VarString>
{
public:
    VarString( intf_thread_t *pIntf ): Variable( pIntf ) { }
    virtual ~VarString() { }

    /// Get the variable type
    virtual const std::string &getType() const { return m_type; }

    /// Set the internal value
    virtual void set( std::string str );
    virtual std::string get() const { return m_value; }

private:
    /// Variable type
    static const std::string m_type;
    /// string value
    std::string m_value;
};

#endif
