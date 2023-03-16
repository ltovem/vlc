/*****************************************************************************
 * cmd_vars.cpp
 *****************************************************************************
 * Copyright (C) 2004 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#include "cmd_vars.hpp"
#include "../src/vlcproc.hpp"
#include "../utils/var_text.hpp"
#include "../vars/equalizer.hpp"
#include "../vars/playtree.hpp"


void CmdItemUpdate::execute()
{
    VlcProc::instance( getIntf() )->getPlaytreeVar().onUpdateItem( m_pos );
}

bool CmdItemUpdate::checkRemove( CmdGeneric *pQueuedCommand ) const
{
    // We don't use RTTI - Use C-style cast
    CmdItemUpdate *pUpdateCommand = (CmdItemUpdate *)(pQueuedCommand);
    return m_pos== pUpdateCommand->m_pos;
}

void CmdItemPlaying::execute()
{
    VlcProc::instance( getIntf() )->getPlaytreeVar().onUpdatePlaying( m_pos );
}

void CmdPlaytreeAppend::execute()
{
    VlcProc::instance( getIntf() )->getPlaytreeVar().onAppend( m_pos );
}

void CmdPlaytreeDelete::execute()
{
    VlcProc::instance( getIntf() )->getPlaytreeVar().onDelete( m_pos );
}

void CmdSetText::execute()
{
    m_rText.set( m_value );
}


void CmdSetEqBands::execute()
{
    m_rEqBands.set( m_value );
}


void CmdSetEqPreamp::execute()
{
    m_rPreamp.set( m_value, false );
}
