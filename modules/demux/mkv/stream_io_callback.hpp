/*****************************************************************************
 * stream_io_callback.hpp : matroska demuxer
 *****************************************************************************
 * Copyright (C) 2003-2004 VLC authors and VideoLAN
 *
 * Authors: Laurent Aimar <fenrir@via.ecp.fr>
 *          Steve Lhomme <steve.lhomme@free.fr>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef VLC_MKV_STREAM_IO_CALLBACK_HPP_
#define VLC_MKV_STREAM_IO_CALLBACK_HPP_

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_demux.h>

#include "ebml/IOCallback.h"

using namespace LIBEBML_NAMESPACE;

namespace mkv {

/*****************************************************************************
 * Stream management
 *****************************************************************************/
class vlc_stream_io_callback: public IOCallback
{
  private:
    stream_t       *s;
    bool           mb_eof;
    bool           b_owner;

  public:
    vlc_stream_io_callback( stream_t *, bool owner );

    virtual ~vlc_stream_io_callback()
    {
        if( b_owner )
            vlc_stream_Delete( s );
    }

    bool IsEOF() const { return mb_eof; }

    virtual uint32   read            ( void *p_buffer, size_t i_size);
    virtual void     setFilePointer  ( int64_t i_offset, seek_mode mode = seek_beginning );
    virtual size_t   write           ( const void *p_buffer, size_t i_size);
    virtual uint64   getFilePointer  ( void );
    virtual void     close           ( void ) { return; }
    uint64           toRead          ( void );
};

} // namespace

#endif
