// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * ContentDescription.h
 *****************************************************************************
 Copyright (C) 2010 - 2011 Klagenfurt University

 Created on: Aug 10, 2010
 Authors: Christopher Mueller <christopher.mueller@itec.uni-klu.ac.at>
          Christian Timmerer  <christian.timmerer@itec.uni-klu.ac.at>
 *****************************************************************************/

#ifndef CONTENTDESCRIPTION_H_
#define CONTENTDESCRIPTION_H_

#include <string>

namespace dash
{
    namespace mpd
    {
        class ContentDescription
        {
            public:
                const std::string&          getSchemeIdUri() const;
                void                        setSchemeIdUri( const std::string &uri );
                const std::string&          getSchemeInformation() const;
                void                        setSchemeInformation( const std::string &schemeInfo );

            private:
                std::string                 schemeIdUri;
                std::string                 schemeInformation;
        };
    }
}

#endif /* CONTENTDESCRIPTION_H_ */
