/*
 * SourceStream.hpp
 *****************************************************************************
 * Copyright © 2015 - VideoLAN and VLC Authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifndef SOURCESTREAM_HPP
#define SOURCESTREAM_HPP

#include <vlc_common.h>
#include <vlc_block_helper.h>

namespace adaptive
{
    class AbstractSource;

    class AbstractSourceStream
    {
        public:
            virtual ~AbstractSourceStream() {}
            virtual stream_t *makeStream() = 0;
            virtual void Reset() = 0;
            virtual size_t Peek(const uint8_t **, size_t) = 0;
    };

    class AbstractChunksSourceStream : public AbstractSourceStream
    {
        public:
            AbstractChunksSourceStream(vlc_object_t *, AbstractSource *);
            virtual ~AbstractChunksSourceStream();
            virtual void Reset() override;
            virtual stream_t *makeStream() override;

        protected:
            virtual ssize_t Read(uint8_t *, size_t) = 0;
            virtual int     Seek(uint64_t) = 0;
            bool b_eof;
            vlc_object_t *p_obj;
            AbstractSource *source;

        private:
            static ssize_t read_Callback(stream_t *, void *, size_t);
            static int seek_Callback(stream_t *, uint64_t);
            static int control_Callback( stream_t *, int i_query, va_list );
            static void delete_Callback( stream_t * );
    };

    class ChunksSourceStream : public AbstractChunksSourceStream
    {
        public:
            ChunksSourceStream(vlc_object_t *, AbstractSource *);
            virtual ~ChunksSourceStream();
            virtual void Reset() override;

        protected:
            virtual ssize_t Read(uint8_t *, size_t) override;
            virtual int     Seek(uint64_t) override;
            virtual size_t  Peek(const uint8_t **, size_t) override;

        private:
            block_t *p_block;
    };

    class BufferedChunksSourceStream : public AbstractChunksSourceStream
    {
        public:
            BufferedChunksSourceStream(vlc_object_t *, AbstractSource *);
            virtual ~BufferedChunksSourceStream();
            virtual void Reset() override;

        protected:
            virtual ssize_t Read(uint8_t *, size_t) override;
            virtual int     Seek(uint64_t) override;
            virtual size_t  Peek(const uint8_t **, size_t) override;

        private:
            ssize_t doRead(uint8_t *, size_t);
            void fillByteStream(size_t);
            void invalidatePeek();
            static const int MAX_BACKEND = 5 * 1024 * 1024;
            static const int MIN_BACKEND_CLEANUP = 50 * 1024;
            uint64_t i_global_offset;
            size_t i_bytestream_offset;
            block_bytestream_t bs;
            block_t *p_peekdata;
    };
}
#endif // SOURCESTREAM_HPP
