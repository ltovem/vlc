// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * CodecDescription.hpp
 *****************************************************************************
 Copyright (C) 2021 - VideoLabs, VideoLAN and VLC Authors
 *****************************************************************************/
#ifndef CODECDESCRIPTION_HPP
#define CODECDESCRIPTION_HPP

#include "../tools/Properties.hpp"
#include <vlc_es.h>
#include <string>
#include <list>

namespace adaptive
{
    namespace playlist
    {
        class CodecDescription
        {
            public:
                CodecDescription();
                CodecDescription(const std::string &);
                CodecDescription(const CodecDescription &) = delete;
                void operator=(const CodecDescription&) = delete;
                virtual ~CodecDescription();
                const es_format_t *getFmt() const;
                void setDimensions(unsigned, unsigned);
                void setDescription(const std::string &);
                void setLanguage(const std::string &);
                void setAspectRatio(const AspectRatio &);
                void setFrameRate(const Rate &);
                void setSampleRate(const Rate &);
                void setChannelsCount(unsigned);

            protected:
                es_format_t fmt;
        };

        class CodecDescriptionList : public std::list<CodecDescription *>
        {
            public:
                CodecDescriptionList();
                ~CodecDescriptionList();
                CodecDescriptionList(const CodecDescriptionList &) = delete;
                void operator=(const CodecDescriptionList&) = delete;
        };
    }
}

#endif // CODECDESCRIPTION_HPP
