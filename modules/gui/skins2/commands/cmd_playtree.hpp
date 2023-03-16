/*****************************************************************************
 * cmd_playtree.hpp
 *****************************************************************************
 * Copyright (C) 2005 the VideoLAN team
 *
 * Authors: Antoine Cellerier <dionoea@videolan.org>
 *          Clément Stenac <zorglub@videolan.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef CMD_PLAYTREE_HPP
#define CMD_PLAYTREE_HPP

#include "cmd_generic.hpp"
#include "../utils/var_tree.hpp"

// TODO : implement branch specific stuff

/// Command to delete the selected items from a tree
class CmdPlaytreeDel: public CmdGeneric
{
public:
    CmdPlaytreeDel( intf_thread_t *pIntf, VarTree &rTree )
                  : CmdGeneric( pIntf ), m_rTree( rTree ) { }
    virtual ~CmdPlaytreeDel() { }
    virtual void execute();
    virtual std::string getType() const { return "playtree del"; }

private:
    /// Tree
    VarTree &m_rTree;
};

/// Command to reset the playtree
DEFINE_COMMAND( PlaytreeReset, "playtree reset" )

/// Command to sort the playtree
DEFINE_COMMAND( PlaytreeSort, "playtree sort" )

#endif
