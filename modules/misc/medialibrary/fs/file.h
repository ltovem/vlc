/*****************************************************************************
 * file.h: Media library network file
 *****************************************************************************
 * Copyright (C) 2018 VLC authors, VideoLAN and VideoLabs
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef SD_FILE_H
#define SD_FILE_H

#include <medialibrary/filesystem/IFile.h>

namespace vlc {
  namespace medialibrary {

using namespace ::medialibrary::fs;

class SDFile : public IFile
{
public:
    SDFile( std::string mrl, int64_t, time_t );
    SDFile( std::string mrl, LinkedFileType, std::string linkedFile, int64_t, time_t );

    virtual ~SDFile() = default;
    const std::string& mrl() const override;
    const std::string& name() const override;
    const std::string& extension() const override;
    const std::string& linkedWith() const override;
    LinkedFileType linkedType() const override;
    bool isNetwork() const override;
    int64_t size() const override;
    time_t lastModificationDate() const override;

private:
    std::string m_mrl;
    std::string m_name;
    std::string m_extension;
    std::string m_linkedFile;
    LinkedFileType m_linkedType = LinkedFileType::None;
    bool m_isNetwork;
    int64_t m_size = 0;
    time_t m_lastModificationTime = 0;
};

  } /* namespace medialibrary */
} /* namespace vlc */

#endif
