// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *****************************************************************************/

#ifndef MLNETWORKSOURCELISTENER_HPP
#define MLNETWORKSOURCELISTENER_HPP

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <vlc_media_library.h>
#include <vlc_media_source.h>
#include <vlc_threads.h>
#include <vlc_cxx_helpers.hpp>

#include <memory>
#include <functional>

class MediaTreeListener
{
public:
    using MediaTreePtr = vlc_shared_data_ptr_type(vlc_media_tree_t,
                                                  vlc_media_tree_Hold,
                                                  vlc_media_tree_Release);

    using ListenerPtr = std::unique_ptr<vlc_media_tree_listener_id,
                                        std::function<void(vlc_media_tree_listener_id*)>>;

    class MediaTreeListenerCb
    {
    public:
        virtual ~MediaTreeListenerCb() = default;
        virtual void onItemCleared( MediaTreePtr tree, input_item_node_t* node ) = 0;
        virtual void onItemAdded( MediaTreePtr tree, input_item_node_t* parent, input_item_node_t *const children[], size_t count ) = 0;
        virtual void onItemRemoved( MediaTreePtr tree, input_item_node_t* node, input_item_node_t *const children[], size_t count ) = 0;
        virtual void onItemPreparseEnded( MediaTreePtr tree, input_item_node_t* node, enum input_item_preparse_status status ) = 0;
    };

public:
    MediaTreeListener( MediaTreePtr tree, std::unique_ptr<MediaTreeListenerCb> &&cb );

    MediaTreeListener( MediaTreeListener&& ) = default;
    MediaTreeListener& operator=( MediaTreeListener&& ) = default;

    MediaTreeListener( const MediaTreeListener& ) = delete;
    MediaTreeListener& operator=( const MediaTreeListener& ) = delete;

    MediaTreePtr tree;
    ListenerPtr listener = nullptr;
    std::unique_ptr<MediaTreeListenerCb> cb;
};
#endif // MLNETWORKSOURCELISTENER_HPP
