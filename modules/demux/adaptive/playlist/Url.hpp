/*
 * Url.hpp
 *****************************************************************************
 * Copyright (C) 2014 - VideoLAN and VLC Authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifndef URL_HPP
#define URL_HPP

#include <string>
#include <vector>
#include <vlc_common.h>

namespace adaptive
{
    namespace playlist
    {
        class SegmentTemplate;
        class BaseRepresentation;

        class Url
        {
            public:
                class Component
                {
                    friend class Url;
                    public:
                        Component(const std::string &, const SegmentTemplate * = nullptr);

                    protected:
                        std::string component;
                        const SegmentTemplate *templ;

                    private:
                        bool b_scheme;
                        bool b_dir;
                        bool b_absolute;
                };

                Url();
                Url(const Component &);
                explicit Url(const std::string &);
                bool hasScheme() const;
                bool empty() const;
                Url & prepend(const Component &);
                Url & append(const Component &);
                Url & append(const Url &);
                Url & prepend(const Url &);
                std::string toString(size_t, const BaseRepresentation *) const;
                std::string toString() const;

            private:
                std::vector<Component> components;
        };
    }
}

#endif // URL_HPP
