/*****************************************************************************
 * Templates.hpp
 *****************************************************************************
 * Copyright (C) 2014-2015 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifndef TEMPLATES_HPP
#define TEMPLATES_HPP

namespace adaptive
{
    namespace playlist
    {

        template<class T> class Initializable
        {
        public:
            Initializable()
            {
                initialisationSegment.Set(nullptr);
            }
            ~Initializable()
            {
                delete initialisationSegment.Get();
            }
            Property<T *> initialisationSegment;
        };

        template<class T> class Indexable
        {
        public:
            Indexable()
            {
                indexSegment.Set(nullptr);
            }
            ~Indexable()
            {
                delete indexSegment.Get();
            }
            Property<T *> indexSegment;
        };

    }
}

#endif // TEMPLATES_HPP

