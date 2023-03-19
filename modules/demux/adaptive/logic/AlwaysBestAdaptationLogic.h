// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * AlwaysBestAdaptationLogic.h
 *****************************************************************************
 Copyright (C) 2010 - 2011 Klagenfurt University

 Created on: Aug 10, 2010
 Authors: Christopher Mueller <christopher.mueller@itec.uni-klu.ac.at>
          Christian Timmerer  <christian.timmerer@itec.uni-klu.ac.at>
 *****************************************************************************/

#ifndef ALWAYSBESTADAPTATIONLOGIC_H_
#define ALWAYSBESTADAPTATIONLOGIC_H_

#include "AbstractAdaptationLogic.h"

namespace adaptive
{
    namespace logic
    {
        class AlwaysBestAdaptationLogic : public AbstractAdaptationLogic
        {
            public:
                AlwaysBestAdaptationLogic           (vlc_object_t *);

                virtual BaseRepresentation *getNextRepresentation(BaseAdaptationSet *,
                                                                  BaseRepresentation *) override;
        };
    }
}

#endif /* ALWAYSBESTADAPTATIONLOGIC_H_ */
