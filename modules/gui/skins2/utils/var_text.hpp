// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * var_text.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#ifndef VAR_TEXT_HPP
#define VAR_TEXT_HPP

#include "variable.hpp"
#include "var_percent.hpp"
#include "observer.hpp"
#include "ustring.hpp"


/// String variable
class VarText: public Variable, public Subject<VarText>,
               public Observer<VarPercent>,
               public Observer<VarText>
{
public:
    // Set substVars to true to replace "$X" variables in the text
    VarText( intf_thread_t *pIntf, bool substVars = true );
    virtual ~VarText();

    /// Get the variable type
    virtual const std::string &getType() const { return m_type; }

    /// Set the internal value
    virtual void set( const UString &rText );
    virtual const UString get() const;

    /// Methods called when an observed variable is modified
    virtual void onUpdate( Subject<VarPercent> &rVariable, void* );
    virtual void onUpdate( Subject<VarText> &rVariable, void* );

private:
    /// Stop observing other variables
    void delObservers();

    /// Variable type
    static const std::string m_type;
    /// The text of the variable
    UString m_text;
    /// Actual text after having replaced the variables
    UString m_lastText;
    /// Flag to activate or not "$X" variables substitution
    bool m_substVars;
};

#endif
