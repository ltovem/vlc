/*****************************************************************************
 * xmlparser.hpp
 *****************************************************************************
 * Copyright (C) 2004 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef XMLPARSER_HPP
#define XMLPARSER_HPP

#include "../src/skin_common.hpp"
#include <vlc_block.h>
#include <vlc_stream.h>
#include <vlc_xml.h>
#include <map>

// Current DTD version
#define SKINS_DTD_VERSION "2.0"

/// XML parser using libxml2 text reader API
class XMLParser: public SkinObject
{
public:
    XMLParser( intf_thread_t *pIntf, const std::string &rFileName );
    virtual ~XMLParser();

    /// Parse the file. Returns true on success
    bool parse();

protected:
    // Key comparison function for type "const char*"
    struct ltstr
    {
        bool operator()(const char* s1, const char* s2) const
        {
            return strcmp(s1, s2) < 0;
        }
    };
    /// Type for attribute lists
    typedef std::map<const char*, const char*, ltstr> AttrList_t;

    /// Flag for validation errors
    bool m_errors;

    /// Callbacks
    virtual void handleBeginElement( const std::string &rName, AttrList_t &attr )
        { (void)rName; (void)attr; }
    virtual void handleEndElement( const std::string &rName ) { (void)rName; }

private:
    void LoadCatalog();

    /// Reader context
    xml_t *m_pXML;
    xml_reader_t *m_pReader;
    stream_t *m_pStream;
};

#endif
