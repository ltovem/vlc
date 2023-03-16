/*****************************************************************************
 * observer.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include <set>

// Forward declaration
template <class S, class ARG> class Observer;


/// Template for subjects in the Observer design pattern
template <class S, class ARG = void> class Subject
{
private:
    typedef std::set<Observer<S, ARG>*> observers_t;

public:
    ~Subject() { }

#if 0
    /// Remove all observers; should only be used for debugging purposes
    void clearObservers()
    {
        m_observers.clear();
    }
#endif

    /// Add an observer to this subject. Ignore NULL observers.
    /// Note: adding the same observer twice is not harmful.
    void addObserver( Observer<S, ARG>* pObserver )
    {
        if( pObserver ) m_observers.insert( pObserver );
    }

    /// Remove an observer from this subject. Ignore NULL observers.
    /// Note: removing the same observer twice is not harmful.
    void delObserver( Observer<S, ARG>* pObserver )
    {
        if( pObserver ) m_observers.erase( pObserver );
    }

    /// Notify the observers when the status has changed
    void notify( ARG *arg )
    {
        typename observers_t::const_iterator iter;
        for( iter = m_observers.begin(); iter != m_observers.end(); ++iter )
            (*iter)->onUpdate( *this , arg );
    }

    /// Notify without any argument
    void notify() { notify( NULL ); }

protected:
    Subject() { }

private:
    /// Set of observers for this subject
    observers_t m_observers;
};


/// Template for observers in the Observer design pattern
template <class S, class ARG = void> class Observer
{
public:
    virtual ~Observer() { }

    /// Method called when the subject is modified
    virtual void onUpdate( Subject<S, ARG> &rSubject, ARG *arg) = 0;

protected:
    Observer() { }
};


#endif
