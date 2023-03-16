/*****************************************************************************
 * theme_loader.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef THEME_LOADER_HPP
#define THEME_LOADER_HPP

#include "skin_common.hpp"
#include <string>

class ThemeLoader: public SkinObject
{
public:
    ThemeLoader( intf_thread_t *pIntf ): SkinObject( pIntf ) { }
    virtual ~ThemeLoader() { }

    /// The expected fileName must be an UTF-8 string
    bool load( const std::string &fileName );

private:
    /// Extract files from an archive (handles tar.gz and zip)
    /**
     * Expects a string from the current locale.
     */
    bool extract( const std::string &fileName );

    bool unarchive( const std::string &fileName, const std::string &tempPath );

    void deleteTempFiles( const std::string &path );

    /// Get a unique temporary directory
    std::string getTmpDir( );

    /// Parse the XML file given as a parameter and build the skin
    /**
     * Expects UTF8 strings
     */
    bool parse( const std::string &path, const std::string &xmlFile );

    /// Recursively look for the XML file from rootDir.
    /**
     * The first corresponding file found will be chosen and themeFilePath
     * will be updated accordingly.
     * The method returns true if a theme file was found, false otherwise.
     * rootDir and rFilename must both be strings in the current locale,
     * whereas themeFilePath will be in UTF8.
     */
    bool findFile( const std::string &rootDir, const std::string &rFileName,
                   std::string &themeFilePath );

    /// Get the base path of a file
    std::string getFilePath( const std::string &rFullPath );
};

#endif
