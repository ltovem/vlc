// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * cmd_on_top.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#ifndef CMD_ON_TOP_HPP
#define CMD_ON_TOP_HPP

#include "cmd_generic.hpp"


/// "Always on top" command
DEFINE_COMMAND( OnTop, "always on top" )

class CmdSetOnTop: public CmdGeneric
{
public:
    CmdSetOnTop( intf_thread_t *pIntf, bool b_ontop )
        : CmdGeneric( pIntf ), m_ontop( b_ontop ) { }
    virtual ~CmdSetOnTop() { }
    virtual void execute();
    virtual std::string getType() const { return "set on top"; }

private:
    bool m_ontop;
};


#endif
