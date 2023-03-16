// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * playtree.hpp
 *****************************************************************************
 * Copyright (C) 2005 the VideoLAN team
 *
 * Authors: Antoine Cellerier <dionoea@videolan.org>
 *          Clément Stenac <zorglub@videolan.org>
 *****************************************************************************/

#ifndef PLAYTREE_HPP
#define PLAYTREE_HPP

#include "../utils/var_tree.hpp"
#include <vlc_playlist.h>

#include <map>

/// Variable for VLC playlist (new tree format)
class Playtree: public VarTree
{
public:
    Playtree( intf_thread_t *pIntf );
    virtual ~Playtree();

    /// Remove the selected elements from the list
    virtual void delSelected();

    /// Execute the action associated to this item
    virtual void action( VarTree *pItem );

    /// Function called to notify playlist changes
    void onChange();

    /// Function called to notify playlist item update
    void onUpdateItem( int pos );

    /// Function called to notify about current playing item
    void onUpdatePlaying( int pos );

    /// Function called to notify playlist item append
    void onAppend( int pos );

    /// Function called to notify playlist item delete
    void onDelete( int pos );

    ///
    void onUpdateSlider();

    ///
    void insertItems( VarTree& item, const std::list<std::string>& files, bool start );

private:
    /// Current index being played back
    int m_currentIndex;

    /// Build the list from the VLC playlist
    void buildTree();

    /// Retrieve an iterator from vlc_playlist_item_t
    //Iterator findById( vlc_playlist_item_t *item );

    /// Update Node's children
    void buildNode( int pos, VarTree &m_pNode );

    /// title for an item
    UString* getTitle( input_item_t *pItem );

    /// point to the Playlist itself (pos = -1) or a child (pos >= 0)
    Iterator getPlaylistIt( int pos = -1 );
};

#endif
