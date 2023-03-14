/*
 * IDownloadRateObserver.h
 *****************************************************************************
 * Copyright (C) 2010 - 2011 Klagenfurt University
 *
 * Created on: Aug 10, 2010
 * Authors: Christopher Mueller <christopher.mueller@itec.uni-klu.ac.at>
 *          Christian Timmerer  <christian.timmerer@itec.uni-klu.ac.at>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef IDOWNLOADRATEOBSERVER_H_
#define IDOWNLOADRATEOBSERVER_H_

#include <vlc_common.h>

namespace adaptive
{
    class ID;

    class IDownloadRateObserver
    {
        public:
            virtual void updateDownloadRate(const ID &, size_t,
                                            vlc_tick_t, vlc_tick_t) = 0;
            virtual ~IDownloadRateObserver(){}
    };
}

#endif /* IDOWNLOADRATEOBSERVER_H_ */
