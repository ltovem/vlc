/*****************************************************************************
 * ini_file.hpp
 *****************************************************************************
 * Copyright (C) 2006 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef INI_FILE_HPP
#define INI_FILE_HPP

#include "skin_common.hpp"
#include <string>
#include <map>


/// INI file parser
class IniFile: public SkinObject
{
public:
    IniFile( intf_thread_t *pIntf, const std::string &rName,
             const std::string &rPath );
    virtual ~IniFile() { }

    /// Parse the INI file and fill the VarManager
    void parseFile();

private:
    std::string m_name;
    std::string m_path;
};


#endif
