/*
 * ICanonicalUrl.hpp
 *****************************************************************************
 * Copyright (C) 2014 - VideoLAN and VLC Authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifndef CANONICALURL_HPP
#define CANONICALURL_HPP

#include "Url.hpp"

namespace adaptive
{
    namespace playlist
    {
        class ICanonicalUrl
        {
            public:
                ICanonicalUrl( const ICanonicalUrl *parent = nullptr ) { setParent(parent); }
                virtual ~ICanonicalUrl() = default;
                virtual Url getUrlSegment() const = 0;
                void setParent( const ICanonicalUrl *parent ) { parentUrlMember = parent; }

            protected:
                Url getParentUrlSegment() const {
                    return (parentUrlMember) ? parentUrlMember->getUrlSegment()
                                             : Url();
                }

            private:
                const ICanonicalUrl *parentUrlMember;
        };
    }
}

#endif // CANONICALURL_HPP
