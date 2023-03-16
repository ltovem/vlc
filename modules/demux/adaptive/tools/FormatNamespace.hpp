// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * FormatNameSpace.hpp
 *****************************************************************************
 Copyright © 2019 VideoLabs, VideoLAN and VLC Authors
 *****************************************************************************/
#ifndef FORMATNAMESPACE_HPP_
#define FORMATNAMESPACE_HPP_

#include <vlc_common.h>
#include <vlc_es.h>
#include <vector>
#include <string>

namespace adaptive
{
    class FormatNamespace
    {
        public:
            FormatNamespace(const std::string &);
            ~FormatNamespace();
            const es_format_t * getFmt() const;

        private:
            void ParseString(const std::string &);
            void Parse(vlc_fourcc_t, const std::vector<std::string> &);
            void ParseMPEG4Elements(const std::vector<std::string> &);
            es_format_t fmt;
    };
}

#endif
