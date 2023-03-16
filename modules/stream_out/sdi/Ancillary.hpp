// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * Ancillary.hpp: SDI Ancillary
 *****************************************************************************
 * Copyright © 2014-2016 VideoLAN and VideoLAN Authors
 *                  2018 VideoLabs
 *****************************************************************************/
#ifndef ANCILLARY_HPP
#define ANCILLARY_HPP

#include <vlc_common.h>
#include <vector>

namespace sdi
{

    class Ancillary
    {
        public:
            virtual void FillBuffer(uint8_t *, size_t) = 0;

            template <typename T>
            class AbstractPacket
            {
                public:
                    std::size_t size() const
                        { return payload.size() * sizeof(payload[0]); }
                    const uint8_t * data() const
                        { return reinterpret_cast<const uint8_t *>(payload.data()); }

                protected:
                    std::vector<T> payload;
            };

            class Data10bitPacket : public AbstractPacket<uint16_t>
            {
                public:
                    Data10bitPacket(uint8_t did, uint8_t sdid,
                                    const AbstractPacket<uint8_t> &);
                    void pad();
            };
    };

    class AFD : public Ancillary
    {
        public:
            AFD(uint8_t afdcode, uint8_t ar);
            virtual ~AFD();
            virtual void FillBuffer(uint8_t *, size_t);

        private:
            class AFDData : public AbstractPacket<uint8_t>
            {
                public:
                    AFDData(uint8_t afdcode, uint8_t ar);
            };
            uint8_t afdcode;
            uint8_t ar;
    };

    class Captions : public Ancillary
    {
        public:
            Captions(const uint8_t *, size_t, unsigned, unsigned);
            virtual ~Captions();
            virtual void FillBuffer(uint8_t *, size_t);

        private:
            class CDP : public AbstractPacket<uint8_t>
            {
                public:
                    CDP(const uint8_t *, std::size_t, uint8_t, uint16_t);
            };
            const uint8_t *p_data;
            size_t i_data;
            unsigned rate;
            uint16_t cdp_counter;
    };
}

#endif // ANCILLARY_HPP
