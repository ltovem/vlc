/*
 * CodecParameters.hpp
 *****************************************************************************
 * Copyright (C) 2021 - VideoLabs, VideoLAN and VLC Authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifndef CODECPARAMETERS_HPP
#define CODECPARAMETERS_HPP

#include <vlc_es.h>
#include <vlc_codecs.h>

#include <string>
#include <vector>

namespace smooth
{
    namespace playlist
    {
        class CodecParameters
        {
            public:
                CodecParameters();
                ~CodecParameters();
                void setFourCC(const std::string &);
                void setWaveFormatEx(const std::string &);
                void setCodecPrivateData(const std::string &);
                void setChannels(uint16_t);
                void setPacketSize(uint16_t);
                void setSamplingRate(uint32_t);
                void setBitsPerSample(uint16_t);
                void setAudioTag(uint16_t);
                void initAndFillEsFmt(es_format_t *) const;

                std::vector<uint8_t> extradata;
                WAVEFORMATEX formatex;
                vlc_fourcc_t fourcc;
                enum es_format_category_e es_type;

            private:
                void fromWaveFormatEx(const std::vector<uint8_t> &);
                void fromVideoInfoHeader(const std::vector<uint8_t> &);
        };
    }
}

#endif // CODECPARAMETERS_HPP
