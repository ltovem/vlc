// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * AlwaysLowestAdaptationLogic.hpp
 *****************************************************************************
 * Copyright (C) 2014 - VideoLAN and VLC authors
 *****************************************************************************/
#ifndef ALWAYSLOWESTADAPTATIONLOGIC_HPP
#define ALWAYSLOWESTADAPTATIONLOGIC_HPP

#include "AbstractAdaptationLogic.h"

namespace adaptive
{
    namespace logic
    {
        class AlwaysLowestAdaptationLogic : public AbstractAdaptationLogic
        {
            public:
                AlwaysLowestAdaptationLogic(vlc_object_t *);

                virtual BaseRepresentation* getNextRepresentation(BaseAdaptationSet *,
                                                                  BaseRepresentation *) override;
        };
    }
}

#endif // ALWAYSLOWESTADAPTATIONLOGIC_HPP
