// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * var_percent.cpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#include "var_percent.hpp"


const std::string VarPercent::m_type = "percent";


void VarPercent::set( float percentage )
{
    if( percentage < 0 )
    {
        percentage = 0;
    }
    if( percentage > 1 )
    {
        percentage = 1;
    }

    // If the value has changed, notify the observers
    if( m_value != percentage )
    {
        m_value = percentage;
        notify( NULL );
    }
}

