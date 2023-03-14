/*
 * StreamFormat.hpp
 *****************************************************************************
 * Copyright (C) 2015 - VideoLAN and VLC authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifndef STREAMFORMAT_HPP
#define STREAMFORMAT_HPP

#include <string>

namespace adaptive
{

    class StreamFormat
    {
        public:
            enum class Type
            {
                Unsupported,
                MPEG2TS,
                MP4,
                WebM,
                Ogg,
                WebVTT,
                TTML,
                PackedAAC,
                PackedMP3,
                PackedAC3,
                Unknown,
            };
            static const unsigned PEEK_SIZE   = 4096;

            StreamFormat( Type = Type::Unsupported );
            explicit StreamFormat( const std::string &mime );
            StreamFormat( const void *, size_t );
            ~StreamFormat();
            operator Type() const;
            std::string str() const;
            bool operator==(Type) const;
            bool operator!=(Type) const;
            bool operator==(const StreamFormat &) const;
            bool operator!=(const StreamFormat &) const;

        private:
            Type type;
    };

}

#endif // STREAMFORMAT_HPP
