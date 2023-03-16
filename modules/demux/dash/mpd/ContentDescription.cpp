// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * ContentDescription.cpp
 *****************************************************************************
 Copyright (C) 2010 - 2011 Klagenfurt University

 Created on: Aug 10, 2010
 Authors: Christopher Mueller <christopher.mueller@itec.uni-klu.ac.at>
          Christian Timmerer  <christian.timmerer@itec.uni-klu.ac.at>
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "ContentDescription.h"

using namespace dash::mpd;

void ContentDescription::setSchemeIdUri(const std::string &uri)
{
    if ( uri.empty() == false )
        this->schemeIdUri = uri;
}

const std::string&      ContentDescription::getSchemeIdUri() const
{
    return this->schemeIdUri;
}

const std::string& ContentDescription::getSchemeInformation() const
{
    return this->schemeInformation;
}

void                    ContentDescription::setSchemeInformation( const std::string &schemeInfo )
{
    if ( schemeInfo.empty() == false )
        this->schemeInformation = schemeInfo;
}
