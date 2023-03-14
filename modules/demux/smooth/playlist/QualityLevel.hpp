/*
 * QualityLevel.hpp
 *****************************************************************************
 * Copyright (C) 2015 - VideoLAN Authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifndef SMOOTHQUALITYLEVEL_HPP
#define SMOOTHQUALITYLEVEL_HPP

#include "../../adaptive/playlist/SegmentBaseType.hpp"
#include "../../adaptive/playlist/BaseRepresentation.h"
#include "../../adaptive/playlist/CodecDescription.hpp"
#include "CodecParameters.hpp"

namespace adaptive
{
    namespace playlist
    {
        class BaseAdaptationSet;
    }
}

namespace smooth
{
    namespace playlist
    {
        using namespace adaptive;
        using namespace adaptive::playlist;

        class SmoothCodecDescription : public CodecDescription
        {
            public:
                SmoothCodecDescription(const CodecParameters &);
                virtual ~SmoothCodecDescription();
        };

        class QualityLevel : public BaseRepresentation,
                               public Initializable<InitSegment>
        {
            public:
                QualityLevel(BaseAdaptationSet *);
                virtual ~QualityLevel ();

                virtual InitSegment * getInitSegment() const override;
                virtual StreamFormat getStreamFormat() const override;

                /* for segment templates */
                virtual std::string contextualize(size_t, const std::string &,
                                                  const SegmentTemplate *) const override;

                void setCodecParameters( const CodecParameters & );
                const CodecParameters & getCodecParameters() const;

            protected:
                virtual CodecDescription * makeCodecDescription(const std::string &) const override;
                CodecParameters codecParameters;
        };
    }
}
#endif // SMOOTHQUALITYLEVEL_HPP
