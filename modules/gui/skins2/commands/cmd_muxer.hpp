/*****************************************************************************
 * cmd_muxer.hpp
 *****************************************************************************
 * Copyright (C) 2005 the VideoLAN team
 *
 * Authors: Olivier Teulière <ipkiss@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef CMD_MUXER_HPP
#define CMD_MUXER_HPP

#include "cmd_generic.hpp"
#include <list>


/// This command only contains other commands (composite pattern)
class CmdMuxer: public CmdGeneric
{
public:
    CmdMuxer( intf_thread_t *pIntf, const std::list<CmdGeneric*> &rList )
        : CmdGeneric( pIntf ), m_list( rList ) { }
    virtual ~CmdMuxer() { }
    virtual void execute();
    virtual std::string getType() const { return "muxer"; }

private:
    /// List of commands we will execute sequentially
    typedef std::list<CmdGeneric*> cmdList_t;
    cmdList_t m_list;
};

#endif
