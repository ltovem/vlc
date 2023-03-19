// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * var_string.cpp
 *****************************************************************************
 * Copyright (C) 2010 the VideoLAN team
 *
 * Author: Erwan Tulou      <erwan10 aT videolan DoT org>
 *****************************************************************************/

#include "var_string.hpp"


const std::string VarString::m_type = "string";


void VarString::set( std::string str )
{
    // If the value has changed, notify the observers
    if( m_value != str )
    {
        m_value = str;
        notify( NULL );
    }
}

