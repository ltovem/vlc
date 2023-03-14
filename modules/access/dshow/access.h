/*****************************************************************************
 * access.h : DirectShow access module for vlc
 * access_sys_t definition
 *****************************************************************************
 * Copyright (C) 2002, 2004, 2010-2011 VLC authors and VideoLAN
 *
 * Author: Gildas Bazin <gbazin@videolan.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

/*****************************************************************************
 * Preamble
 *****************************************************************************/

#include <dshow.h>

#include <vector>

#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

typedef struct demux_sys_t demux_sys_t;

namespace dshow {

/****************************************************************************
 * Crossbar stuff
 ****************************************************************************/
#define MAX_CROSSBAR_DEPTH 10

struct CrossbarRoute
{
    ComPtr<IAMCrossbar> pXbar;
    LONG        VideoInputIndex;
    LONG        VideoOutputIndex;
    LONG        AudioInputIndex;
    LONG        AudioOutputIndex;
};

struct access_sys_t;

void DeleteCrossbarRoutes( access_sys_t * );
HRESULT FindCrossbarRoutes( vlc_object_t *, access_sys_t *,
                            ComPtr<IPin> &, LONG, int = 0 );

/****************************************************************************
 * Access descriptor declaration
 ****************************************************************************/
struct access_sys_t
{
    /* These 2 must be left at the beginning */
    vlc_mutex_t lock;
    vlc_cond_t  wait;

    ComPtr<IFilterGraph>            p_graph;
    ComPtr<ICaptureGraphBuilder2>   p_capture_graph_builder2;
    ComPtr<IMediaControl>           p_control;

    int                     i_crossbar_route_depth;
    CrossbarRoute           crossbar_routes[MAX_CROSSBAR_DEPTH];

    /* list of elementary streams */
    std::vector<struct dshow_stream_t*> pp_streams;
    int            i_current_stream;

    /* misc properties */
    int            i_width;
    int            i_height;
    int            i_chroma;
    vlc_tick_t     i_start;
};

}

