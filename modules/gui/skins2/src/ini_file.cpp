// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * ini_file.cpp
 *****************************************************************************
 * Copyright (C) 2006 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *****************************************************************************/

#include "ini_file.hpp"
#include "var_manager.hpp"
#include <fstream>


IniFile::IniFile( intf_thread_t *pIntf, const std::string &rName,
                  const std::string &rPath ):
    SkinObject( pIntf ), m_name( rName ), m_path( rPath )
{
}


void IniFile::parseFile()
{
    VarManager *pVarManager = VarManager::instance( getIntf() );

    // Open the file
    std::fstream fs( m_path.c_str(), std::fstream::in );
    if( fs.is_open() )
    {
        std::string section;
        std::string line;
        while( !fs.eof() )
        {
            // Read the next line
            fs >> line;

            switch( line[0] )
            {
            // "[section]" line ?
            case '[':
                section = line.substr( 1, line.size() - 2);
                break;

            // Comment
            case ';':
            case '#':
                break;

            // Variable declaration
            default:
                size_t eqPos = line.find( '=' );
                std::string var = line.substr( 0, eqPos );
                std::string val = line.substr( eqPos + 1, line.size() - eqPos - 1);

                std::string name = m_name + "." + section + "." + var;

                // Convert to lower case because of some buggy winamp2 skins
                for( size_t i = 0; i < name.size(); i++ )
                {
                    name[i] = tolower( (unsigned char)name[i] );
                }

                // Register the value in the var manager
                pVarManager->registerConst( name, val );
            }
        }
        fs.close();
    }
    else
    {
        msg_Err( getIntf(), "Failed to open INI file %s", m_path.c_str() );
    }
}

