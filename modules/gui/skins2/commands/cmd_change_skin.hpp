// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * cmd_change_skin.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#ifndef CMD_CHANGE_SKIN_HPP
#define CMD_CHANGE_SKIN_HPP

#include "cmd_generic.hpp"


/// "Change Skin" command
class CmdChangeSkin: public CmdGeneric
{
public:
    CmdChangeSkin( intf_thread_t *pIntf, const std::string &rFile ):
        CmdGeneric( pIntf ), m_file( rFile ) { }
    virtual ~CmdChangeSkin() { }
    virtual void execute();
    virtual std::string getType() const { return "change skin"; }

private:
    /// Skin file to load
    std::string m_file;
};

#endif
