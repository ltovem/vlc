// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * var_manager.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#ifndef VAR_MANAGER_HPP
#define VAR_MANAGER_HPP

#include "../utils/var_text.hpp"
#include <list>
#include <map>


class VarManager: public SkinObject
{
public:
    /// Get the instance of VarManager
    static VarManager *instance( intf_thread_t *pIntf );

    /// Delete the instance of VarManager
    static void destroy( intf_thread_t *pIntf );

    /// Register a named variable in the manager
    void registerVar( const VariablePtr &rcVar, const std::string &rName );

    /// Register an anonymous variable in the manager
    void registerVar( const VariablePtr &rcVar );

    /// Get a variable by its name (NULL if not found)
    Variable *getVar( const std::string &rName );

    /// Get a variable by its name and check the type (NULL if not found)
    Variable *getVar( const std::string &rName, const std::string &rType );

    /// Get the tooltip text variable
    VarText &getTooltipText() { return *m_pTooltipText; }

    /// Get the help text variable
    VarText &getHelpText() { return *m_pHelpText; }

    /// Register a constant value
    void registerConst( const std::string &rName, const std::string &rValue);

    /// Get a constant value by its name
    std::string getConst( const std::string &rName );

private:
    /// Tooltip text
    VarText *m_pTooltipText;
    /// Help text
    VarText *m_pHelpText;
    /// Map of named registered variables
    std::map<std::string, VariablePtr> m_varMap;
    /// List of named registered variables
    std::list<std::string> m_varList;
    /// List of anonymous registered variables
    std::list<VariablePtr> m_anonVarList;
    /// Map of constant values
    std::map<std::string, std::string> m_constMap;

    /// Private because it is a singleton
    VarManager( intf_thread_t *pIntf );
    virtual ~VarManager();
};


#endif
