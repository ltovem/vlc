/*****************************************************************************
 * var_percent.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef VAR_PERCENT_HPP
#define VAR_PERCENT_HPP

#include "variable.hpp"
#include "observer.hpp"

class VarPercent;


/// Percentage variable
class VarPercent: public Variable, public Subject<VarPercent>
{
public:
    VarPercent( intf_thread_t *pIntf ) :
        Variable( pIntf ), m_value( 0 ), m_step( .05f ) {}
    virtual ~VarPercent() { }

    /// Get the variable type
    virtual const std::string &getType() const { return m_type; }

    /// Set the internal value
    virtual void set( float percentage );
    virtual float get() const { return m_value; }

    /// Get the variable preferred step
    virtual float getStep() const { return m_step; }
    virtual void setStep( float val ) { m_step = val; }

    /// Increment or decrement variable
    void increment( int num ) { return set( m_value + num * m_step ); }

private:
    /// Variable type
    static const std::string m_type;
    /// Percent value
    float m_value;
    /// preferred step (for scrolling)
    float m_step;
};

#endif
