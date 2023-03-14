/*****************************************************************************
 * devicelister.h: Media library network file system
 *****************************************************************************
 * Copyright (C) 2018 VLC authors, VideoLAN and VideoLabs
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef ML_DEVICELISTER_H
#define ML_DEVICELISTER_H

#include <medialibrary/IDeviceLister.h>

#include <vlc_media_source.h>
#include <vlc_threads.h>
#include <vlc_cxx_helpers.hpp>

#include <memory>

namespace vlc
{
namespace medialibrary
{

namespace ml = ::medialibrary;

class DeviceLister : public ml::IDeviceLister
{
public:
    DeviceLister(vlc_object_t* parent);

private:
    /*
     * Only used by the media library through the IDeviceLister interface, so
     * it's fine to keep those private for the implementation
     */
    virtual void refresh() override;
    virtual bool start( ml::IDeviceListerCb* cb ) override;
    virtual void stop() override;

    static void onChildrenReset( vlc_media_tree_t* tree, input_item_node_t* node,
                                 void* data );
    static void onChildrenAdded( vlc_media_tree_t* tree, input_item_node_t* node,
                                 input_item_node_t* const children[], size_t count,
                                 void *data );
    static void onChildrenRemoved( vlc_media_tree_t* tree, input_item_node_t* node,
                                   input_item_node_t* const children[], size_t count,
                                   void* data );


    void onChildrenReset( vlc_media_tree_t* tree, input_item_node_t* node );
    void onChildrenAdded( vlc_media_tree_t* tree, input_item_node_t* node,
                          input_item_node_t* const children[], size_t count );
    void onChildrenRemoved( vlc_media_tree_t* tree, input_item_node_t* node,
                            input_item_node_t* const children[], size_t count );

private:
    vlc_object_t* m_parent;
    ml::IDeviceListerCb* m_cb;
    vlc::threads::mutex m_mutex;
    struct MediaSource
    {
        MediaSource( vlc_media_source_t* );
        ~MediaSource();
        MediaSource( const MediaSource& ) = delete;
        MediaSource& operator=( const MediaSource& ) = delete;
        MediaSource( MediaSource&& ) noexcept;
        MediaSource& operator=( MediaSource&& ) noexcept;
        vlc_media_source_t* s;
        vlc_media_tree_listener_id* l;
    };
    std::vector<MediaSource> m_mediaSources;
};

}
}


#endif // ML_DEVICELISTER_H
