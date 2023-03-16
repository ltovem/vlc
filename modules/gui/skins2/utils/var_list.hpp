/*****************************************************************************
 * var_list.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef VAR_LIST_HPP
#define VAR_LIST_HPP

#include <list>

#include "variable.hpp"
#include "observer.hpp"
#include "ustring.hpp"
#include "var_percent.hpp"


/// List variable
class VarList: public Variable, public Subject<VarList>
{
public:
    VarList( intf_thread_t *pIntf );
    virtual ~VarList();

    /// Get the variable type
    virtual const std::string &getType() const { return m_type; }

    /// Add a pointer on a string in the list
    virtual void add( const UStringPtr &rcString );

    /// Remove the selected elements from the list
    virtual void delSelected();

    /// Remove all the elements from the list
    virtual void clear();

    /// Get the number of items in the list
    int size() const { return m_list.size(); }

    /// Type of an element in the list
    struct Elem_t
    {
        UStringPtr m_cString;
        bool m_selected;
        bool m_playing;

        Elem_t( const UStringPtr &rcString,
                bool selected = false, bool playing = false )
              : m_cString( rcString ),
                m_selected( selected ), m_playing( playing ) { }
    };

    /// Iterators
    typedef std::list<Elem_t>::iterator Iterator;
    typedef std::list<Elem_t>::const_iterator ConstIterator;

    /// Beginning of the list
    Iterator begin() { return m_list.begin(); }
    ConstIterator begin() const { return m_list.begin(); }

    /// End of the list
    Iterator end() { return m_list.end(); }
    ConstIterator end() const { return m_list.end(); }

    /// Return an iterator on the n'th element of the list
    Iterator operator[]( int n );
    ConstIterator operator[]( int n ) const;

    /// Execute the action associated to this item
    virtual void action( Elem_t *pItem ) { (void)pItem; }

    /// Get a reference on the position variable
    VarPercent &getPositionVar() const
        { return *((VarPercent*)m_cPosition.get()); }

    /// Get a counted pointer on the position variable
    const VariablePtr &getPositionVarPtr() const { return m_cPosition; }

protected:
    /// List of elements
    std::list<Elem_t> m_list;

private:
    /// Variable type
    static const std::string m_type;
    /// Position variable
    VariablePtr m_cPosition;
};


#endif
