// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * util.h: Media library utils
 *****************************************************************************
 * Copyright (C) 2018 VLC authors, VideoLAN and VideoLabs
 *****************************************************************************/

#ifndef SD_UTIL_H
#define SD_UTIL_H

#include <string>

namespace vlc {
  namespace medialibrary {
    namespace utils {

std::string fileName(const std::string& filePath);
std::string extension(const std::string& fileName);
std::string directory(const std::string& fileName);

    } /* namespace utils */
  } /* namespace medialibrary */
} /* namespace vlc */

#endif
