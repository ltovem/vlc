// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * cmd_add_item.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#ifndef CMD_ADD_ITEM_HPP
#define CMD_ADD_ITEM_HPP

#include "cmd_generic.hpp"
#include <string>


/// "Add item" command
class CmdAddItem: public CmdGeneric
{
public:
    CmdAddItem( intf_thread_t *pIntf, const std::string &rName, bool playNow )
              : CmdGeneric( pIntf ), m_name( rName ), m_playNow( playNow ) { }
    virtual ~CmdAddItem() { }
    virtual void execute();
    virtual std::string getType() const { return "add item"; }

private:
    /// Name of the item to enqueue
    std::string m_name;
    /// Should we play the item immediately?
    bool m_playNow;
};

#endif
