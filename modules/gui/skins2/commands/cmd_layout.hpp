// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * cmd_layout.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#ifndef CMD_LAYOUT_HPP
#define CMD_LAYOUT_HPP

#include "cmd_generic.hpp"

class TopWindow;
class GenericLayout;

/// "Change layout" command
class CmdLayout: public CmdGeneric
{
public:
    CmdLayout( intf_thread_t *pIntf, TopWindow &rWindow,
               GenericLayout &rLayout );
    virtual ~CmdLayout() { }
    virtual void execute();
    virtual std::string getType() const { return "change layout"; }

private:
    TopWindow &m_rWindow;
    GenericLayout &m_rLayout;
};

#endif
