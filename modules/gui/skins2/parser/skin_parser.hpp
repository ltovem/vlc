/*****************************************************************************
 * skin_parser.hpp
 *****************************************************************************
 * Copyright (C) 2004 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef SKIN_PARSER_HPP
#define SKIN_PARSER_HPP

#include "xmlparser.hpp"
#include "builder_data.hpp"
#include <set>


/// Parser for the skin DTD
class SkinParser: public XMLParser
{
public:

    enum {
        POS_UNDEF  = 0,
        POS_CENTER = 1,
        POS_LEFT   = 2,
        POS_RIGHT  = 4,
        POS_TOP    = 8,
        POS_BOTTOM = 16,
    };

    SkinParser( intf_thread_t *pIntf, const std::string &rFileName,
                const std::string &rPath, BuilderData *pData = NULL );
    virtual ~SkinParser();

    const BuilderData &getData() const { return *m_pData; }

    static int convertColor( const char *transcolor );

private:
    /// Path of the theme
    const std::string m_path;
    /// Container for mapping data from the XML
    BuilderData *m_pData;
    /// Indicate whether the class owns the data
    bool m_ownData;
    /// Current IDs
    std::string m_curBitmapId;
    std::string m_curWindowId;
    std::string m_curLayoutId;
    std::string m_curPopupId;
    std::string m_curListId;
    std::string m_curTreeId;
    /// Current position of menu items in the popups
    std::list<int> m_popupPosList;
    /// Current offset of the controls
    int m_xOffset, m_yOffset;
    std::list<int> m_xOffsetList, m_yOffsetList;
    /// Stack of panel ids
    std::list<std::string> m_panelStack;
    /// Layer of the current control in the layout
    int m_curLayer;
    /// Set of used id
    std::set<std::string> m_idSet;

    /// Callbacks
    virtual void handleBeginElement( const std::string &rName,
                                     AttrList_t &attr );
    virtual void handleEndElement( const std::string &rName );

    /// Helper functions
    //@{
    bool convertBoolean( const char *value ) const;
    /// Transform to int, and check that it is in the given range (if not,
    /// the closest range boundary will be used)
    int convertInRange( const char *value, int minValue, int maxValue,
                        const std::string &rAttribute ) const;
    //@}

    /// Generate a new id
    const std::string generateId() const;

    /// Check if the id is unique, and if not generate a new one
    const std::string uniqueId( const std::string &id );

    /// Management of relative positions
    void getRefDimensions( int &rWidth, int &rHeight, bool toScreen );
    int getDimension( std::string value, int refDimension );
    int getPosition( std::string value );
    void updateWindowPos( int width, int height );

    void convertPosition( std::string position,
                          std::string xOffset, std::string yOffset,
                          std::string xMargin, std::string yMargin,
                          int width, int height, int refWidth, int refHeight,
                          int* p_x, int* p_y );

    /// Helper for handleBeginElement: Provide default attribute if missing.
    static void DefaultAttr( AttrList_t &attr, const char *a, const char *b )
    {
        if( attr.find(a) == attr.end() ) attr[strdup(a)] = strdup(b);
    }
    /// Helper for handleBeginElement: Complain if a named attribute is missing.
    bool MissingAttr( AttrList_t &attr, const std::string &name, const char *a );

};

#endif
