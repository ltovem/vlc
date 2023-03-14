/*
 * Helper.h
 *****************************************************************************
 * Copyright (C) 2010 - 2012 Klagenfurt University
 *
 * Created on: Feb 20, 2012
 * Authors: Christopher Mueller <christopher.mueller@itec.uni-klu.ac.at>
 *          Christian Timmerer  <christian.timmerer@itec.uni-klu.ac.at>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef HELPER_H_
#define HELPER_H_

#include <string>
#include <list>

namespace adaptive
{
    class Helper
    {
        public:
            static std::string combinePaths     (const std::string &path1, const std::string &path2);
            static std::string getDirectoryPath (const std::string &path);
            static std::string getFileExtension (const std::string &uri);
            static bool        icaseEquals     (std::string str1, std::string str2);
            static bool        ifind            (std::string haystack, std::string needle);
            static std::list<std::string> tokenize(const std::string &, char);
            static std::string & rtrim(std::string &, const std::string &);
            static std::string & ltrim(std::string &, const std::string &);
            static std::string & trim(std::string &, const std::string &);
            static std::string unescape(const std::string &);
    };
}

#endif /* HELPER_H_ */
