// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * ProgramInformation.h
 *****************************************************************************
 Copyright (C) 2010 - 2011 Klagenfurt University

 Created on: Aug 10, 2010
 Authors: Christopher Mueller <christopher.mueller@itec.uni-klu.ac.at>
          Christian Timmerer  <christian.timmerer@itec.uni-klu.ac.at>
 *****************************************************************************/

#ifndef PROGRAMINFORMATION_H_
#define PROGRAMINFORMATION_H_

#include <string>

namespace dash
{
    namespace mpd
    {
        class ProgramInformation
        {
            public:
                virtual ~ProgramInformation(){}

                const std::string&  getMoreInformationUrl() const;
                void                setMoreInformationUrl( const std::string &url );
                const std::string&  getTitle() const;
                void                setTitle( const std::string &title);
                const std::string&  getSource() const;
                void                setSource( const std::string &source);
                const std::string&  getCopyright() const;
                void                setCopyright( const std::string &copyright);

            private:
                std::string         moreInformationUrl;
                std::string         title;
                std::string         source;
                std::string         copyright;
        };
    }
}

#endif /* PROGRAMINFORMATION_H_ */
