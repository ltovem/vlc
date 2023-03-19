// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * cmd_vars.hpp
 *****************************************************************************
 * Copyright (C) 2004 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *****************************************************************************/

#ifndef CMD_VARS_HPP
#define CMD_VARS_HPP

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include "cmd_generic.hpp"
#include "../utils/ustring.hpp"

class EqualizerBands;
class EqualizerPreamp;
class VarText;

/// Command to notify the playtree of an item update
class CmdItemUpdate: public CmdGeneric
{
public:
    CmdItemUpdate( intf_thread_t *pIntf, int pos ):
        CmdGeneric( pIntf ), m_pos( pos ) {}
    virtual ~CmdItemUpdate() {}
    virtual void execute();
    virtual std::string getType() const { return "playtree update"; }

    /// Only accept removal of command if they concern the same item
    virtual bool checkRemove( CmdGeneric * ) const;

private:
    int m_pos;
};

/// Command to notify the playtree of a change in item playing
class CmdItemPlaying: public CmdGeneric
{
public:
    CmdItemPlaying( intf_thread_t *pIntf, int pos ):
        CmdGeneric( pIntf ), m_pos( pos ) {}
    virtual ~CmdItemPlaying() {}
    virtual void execute();
    virtual std::string getType() const { return "playtree playing"; }

private:
    int m_pos;
};

/// Command to notify the playtree of an item append
class CmdPlaytreeAppend: public CmdGeneric
{
public:
    CmdPlaytreeAppend( intf_thread_t *pIntf, int pos ):
        CmdGeneric( pIntf ), m_pos( pos ) { }
    virtual ~CmdPlaytreeAppend() { }
    virtual void execute();
    virtual std::string getType() const { return "playtree append"; }

private:
    int m_pos;
};

/// Command to notify the playtree of an item deletion
class CmdPlaytreeDelete: public CmdGeneric
{
public:
    CmdPlaytreeDelete( intf_thread_t *pIntf, int pos ):
        CmdGeneric( pIntf ), m_pos( pos ) { }
    virtual ~CmdPlaytreeDelete() { }
    virtual void execute();
    virtual std::string getType() const { return "playtree append"; }

private:
    int m_pos;
};


/// Command to set a text variable
class CmdSetText: public CmdGeneric
{
public:
    CmdSetText( intf_thread_t *pIntf, VarText &rText, const UString &rValue ):
        CmdGeneric( pIntf ), m_rText( rText ), m_value( rValue ) { }
    virtual ~CmdSetText() { }
    virtual void execute();
    virtual std::string getType() const { return "set text"; }

private:
    /// Text variable to set
    VarText &m_rText;
    /// Value to set
    const UString m_value;
};


/// Command to set the equalizer preamp
class CmdSetEqPreamp: public CmdGeneric
{
public:
    CmdSetEqPreamp( intf_thread_t *I, EqualizerPreamp &P, float v )
                  : CmdGeneric( I ), m_rPreamp( P ), m_value( v ) { }
    virtual ~CmdSetEqPreamp() { }
    virtual void execute();
    virtual std::string getType() const { return "set equalizer preamp"; }

private:
    /// Preamp variable to set
    EqualizerPreamp &m_rPreamp;
    /// Value to set
    float m_value;
};


/// Command to set the equalizerbands
class CmdSetEqBands: public CmdGeneric
{
public:
    CmdSetEqBands( intf_thread_t *I, EqualizerBands &B, const std::string &V )
                 : CmdGeneric( I ), m_rEqBands( B ), m_value( V ) { }
    virtual ~CmdSetEqBands() { }
    virtual void execute();
    virtual std::string getType() const { return "set equalizer bands"; }

private:
    /// Equalizer variable to set
    EqualizerBands &m_rEqBands;
    /// Value to set
    const std::string m_value;
};


#endif
