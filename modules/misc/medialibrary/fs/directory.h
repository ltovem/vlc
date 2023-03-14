/*****************************************************************************
 * directory.h: Media library network directory
 *****************************************************************************
 * Copyright (C) 2018 VLC authors, VideoLAN and VideoLabs
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef SD_DIRECTORY_H
#define SD_DIRECTORY_H

#include <medialibrary/filesystem/IDirectory.h>
#include <medialibrary/filesystem/IFile.h>

#include "fs.h"

namespace vlc {
  namespace medialibrary {

using namespace ::medialibrary::fs;

class SDDirectory : public IDirectory
{
public:
    explicit SDDirectory(const std::string &mrl, SDFileSystemFactory &fs);
    const std::string &mrl() const override;
    const std::vector<std::shared_ptr<fs::IFile>> &files() const override;
    const std::vector<std::shared_ptr<fs::IDirectory>> &dirs() const override;
    std::shared_ptr<fs::IDevice> device() const override;
    std::shared_ptr<fs::IFile> file( const std::string& mrl ) const override;
    bool contains( const std::string& file ) const override;

private:
    void read() const;
    void addFile( std::string mrl, fs::IFile::LinkedFileType, std::string linkedWith ) const;

    std::string m_mrl;
    SDFileSystemFactory &m_fs;

    mutable bool m_read_done = false;
    mutable std::vector<std::shared_ptr<fs::IFile>> m_files;
    mutable std::vector<std::shared_ptr<fs::IDirectory>> m_dirs;
    mutable std::shared_ptr<IDevice> m_device;
};

  } /* namespace medialibrary */
} /* namespace vlc */

#endif
