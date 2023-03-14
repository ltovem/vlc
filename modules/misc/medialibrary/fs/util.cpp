/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "util.h"

#include <stdexcept>

namespace vlc {
  namespace medialibrary {
    namespace utils {

#ifdef _WIN32
# define DIR_SEPARATORS "\\/"
#else
# define DIR_SEPARATORS "/"
#endif

std::string
extension(const std::string &fileName)
{
    auto pos = fileName.find_last_of('.');
    if (pos == std::string::npos)
        return {};
    return fileName.substr(pos + 1);
}

std::string
fileName(const std::string &filePath)
{
    auto pos = filePath.find_last_of(DIR_SEPARATORS);
    if (pos == std::string::npos)
        return filePath;
    return filePath.substr(pos + 1);
}

std::string
directory(const std::string &filePath)
{
    auto pos = filePath.find_last_of(DIR_SEPARATORS);
    if (pos == std::string::npos)
        return filePath;
    return filePath.substr(0, pos);
}
    } /* namespace utils */
  } /* namespace medialibrary */
} /* namespace vlc */
