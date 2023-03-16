// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * ID.hpp
 *****************************************************************************
 Copyright (C) 2015 - VideoLAN and VLC authors
 *****************************************************************************/
#ifndef ID_HPP
#define ID_HPP

#include <cstdint>
#include <string>

namespace adaptive
{
    class ID
    {
        public:
            ID(const std::string &);
            ID(uint64_t = 0);
            bool operator==(const ID &) const;
            bool operator<(const ID &) const;
            std::string str() const;
            bool isValid() const;

        private:
            std::string id;
    };

    class Unique
    {
        public:
            const ID & getID() const;
            void       setID(const ID &);

        protected:
            ID id;
    };
}

#endif // ID_HPP
