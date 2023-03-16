/*****************************************************************************
 * equalizer.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef EQUALIZER_HPP
#define EQUALIZER_HPP

#include "../utils/var_percent.hpp"
#include <string>


/// Variable for graphical equalizer
class EqualizerBands: public SkinObject, public Observer<VarPercent>
{
public:
    /// Number of bands
    static const int kNbBands = 10;

    EqualizerBands( intf_thread_t *pIntf );
    virtual ~EqualizerBands();

    /// Set the equalizer bands from a configuration string,
    /// e.g. "1 5.2 -3.6 0 0 2.5 0 0 0 0"
    void set( std::string bands );

    /// Return the variable for a specific band
    VariablePtr getBand( int band );

private:
    /// Array of equalizer bands
    VariablePtr m_cBands[kNbBands];
    /// Flag set when an update is in progress
    bool m_isUpdating;

    /// Callback for band updates
    virtual void onUpdate( Subject<VarPercent> &rBand , void *);
};


/// Variable for equalizer preamp
class EqualizerPreamp: public VarPercent
{
public:
    EqualizerPreamp( intf_thread_t *pIntf );
    virtual ~EqualizerPreamp() { }

    virtual void set( float percentage, bool updateVLC );

    void set( float percentage ) { set( percentage, true ); }
};


#endif
