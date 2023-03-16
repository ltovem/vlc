// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * ProgramInformation.cpp
 *****************************************************************************
 Copyright (C) 2010 - 2011 Klagenfurt University

 Created on: Aug 10, 2010
 Authors: Christopher Mueller <christopher.mueller@itec.uni-klu.ac.at>
          Christian Timmerer  <christian.timmerer@itec.uni-klu.ac.at>
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "ProgramInformation.h"

using namespace dash::mpd;

const std::string &ProgramInformation::getSource() const
{
    return this->source;
}

void ProgramInformation::setSource(const std::string &source)
{
    this->source = source;
}

const std::string &ProgramInformation::getCopyright() const
{
    return this->copyright;
}

void ProgramInformation::setCopyright(const std::string &copyright)
{
    this->copyright = copyright;
}

void ProgramInformation::setMoreInformationUrl(const std::string &url)
{
    this->moreInformationUrl = url;
}

const std::string & ProgramInformation::getMoreInformationUrl() const
{
    return moreInformationUrl;
}

const std::string &ProgramInformation::getTitle() const
{
    return this->title;
}

void        ProgramInformation::setTitle                (const std::string &title)
{
    this->title = title;
}
