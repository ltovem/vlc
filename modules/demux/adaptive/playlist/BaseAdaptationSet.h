// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * AdaptationSet.h
 *****************************************************************************
 Copyright (C) 2010 - 2011 Klagenfurt University

 Created on: Aug 10, 2010
 Authors: Christopher Mueller <christopher.mueller@itec.uni-klu.ac.at>
          Christian Timmerer  <christian.timmerer@itec.uni-klu.ac.at>
 *****************************************************************************/

#ifndef BASEADAPTATIONSET_H_
#define BASEADAPTATIONSET_H_

#include <vector>
#include <string>

#include "CommonAttributesElements.h"
#include "SegmentInformation.hpp"
#include "Role.hpp"
#include "../StreamFormat.hpp"

namespace adaptive
{
    class ID;

    namespace playlist
    {
        class BaseRepresentation;
        class BasePeriod;

        class BaseAdaptationSet : public CommonAttributesElements,
                                  public SegmentInformation
        {
            public:
                BaseAdaptationSet(BasePeriod *);
                virtual ~BaseAdaptationSet();

                virtual StreamFormat            getStreamFormat() const; /*reimpl*/
                const std::vector<BaseRepresentation *>&  getRepresentations() const;
                BaseRepresentation *            getRepresentationByID(const ID &) const;
                void                            setSegmentAligned(bool);
                bool                            isSegmentAligned() const;
                void                            setBitswitchAble(bool);
                bool                            isBitSwitchable() const;
                void                            setRole(const Role &);
                const Role &                    getRole() const;
                void                            addRepresentation( BaseRepresentation *rep );
                const std::string&              getLang() const;
                void                            setLang( const std::string &lang );
                void                            debug(vlc_object_t *,int = 0) const;
                Property<std::string>           description;

            protected:
                Role                            role;
                std::vector<BaseRepresentation *>   representations;
                std::string                     lang;
                Undef<bool>                     segmentAligned;
                Undef<bool>                     bitswitchAble;
        };
    }
}

#endif /* BASEADAPTATIONSET_H_ */
