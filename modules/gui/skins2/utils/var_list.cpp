/*****************************************************************************
 * var_list.cpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#include "var_list.hpp"


const std::string VarList::m_type = "list";


VarList::VarList( intf_thread_t *pIntf ): Variable( pIntf )
{
    // Create the position variable
    m_cPosition = VariablePtr( new VarPercent( pIntf ) );
    getPositionVar().set( 1.0 );
}


VarList::~VarList()
{
}


void VarList::add( const UStringPtr &rcString )
{
    m_list.push_back( Elem_t( rcString ) );
    notify();
}


void VarList::delSelected()
{
    Iterator it = begin();
    while( it != end() )
    {
        if( (*it).m_selected )
        {
            Iterator oldIt = it;
            ++it;
            m_list.erase( oldIt );
        }
        else
        {
            ++it;
        }
    }
    notify();
}


void VarList::clear()
{
    m_list.clear();
}


VarList::Iterator VarList::operator[]( int n )
{
    Iterator it = begin();
    for( int i = 0; i < n; i++ )
    {
        if( it != end() )
        {
            it++;
        }
        else
        {
            break;
        }
    }
    return it;
}


VarList::ConstIterator VarList::operator[]( int n ) const
{
    ConstIterator it = begin();
    for( int i = 0; i < n; i++ )
    {
        if( it != end() )
        {
            it++;
        }
        else
        {
            break;
        }
    }
    return it;
}

