// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * volume.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#ifndef VOLUME_HPP
#define VOLUME_HPP


#include "../utils/var_percent.hpp"
#include <string>


/// Variable for VLC volume
class Volume: public VarPercent
{
public:
    Volume( intf_thread_t *pIntf );
    virtual ~Volume() { }

    virtual void set( float percentage, bool updateVLC );
    virtual void set( float percentage ) { set( percentage, true ); }

    virtual float getVolume() const;
    virtual void setVolume( float volume, bool updateVLC );

    virtual float getStep() const { return m_step; }

    virtual std::string getAsStringPercent() const;

private:
    // preferred volume step on [0., 1.]
    float m_step;
};


#endif
