/*****************************************************************************
 * cmd_audio.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef CMD_AUDIO_HPP
#define CMD_AUDIO_HPP

#include "cmd_generic.hpp"

/// Command to enable/disable the equalizer
class CmdSetEqualizer: public CmdGeneric
{
public:
    CmdSetEqualizer( intf_thread_t *pIntf, bool iEnable )
                   : CmdGeneric( pIntf ), m_enable( iEnable ) { }
    virtual ~CmdSetEqualizer() { }
    virtual void execute();
    virtual std::string getType() const { return "set equalizer"; }

private:
    /// Enable or disable the equalizer
    bool m_enable;
};


#endif
