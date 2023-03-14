/*****************************************************************************
 * device.h: Media library network device
 *****************************************************************************
 * Copyright (C) 2018 VLC authors, VideoLAN and VideoLabs
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef SD_DEVICE_H
#define SD_DEVICE_H

#include <medialibrary/filesystem/IDevice.h>
#include <vector>

#include <vlc_common.h>
#include <vlc_url.h>
#include <vlc_cxx_helpers.hpp>

namespace vlc {
  namespace medialibrary {

using namespace ::medialibrary::fs;

class SDDevice : public IDevice
{
public:
    SDDevice(const std::string& uuid, std::string scheme,
              bool removable, bool isNetwork);

    const std::string &uuid() const override;
    bool isRemovable() const override;
    bool isPresent() const override;
    bool isNetwork() const override;
    std::vector<std::string> mountpoints() const override;
    void addMountpoint( std::string mrl ) override;
    void removeMountpoint( const std::string& mrl ) override;
    std::tuple<bool, std::string> matchesMountpoint( const std::string& mrl ) const override;
    std::string relativeMrl( const std::string& absoluteMrl ) const override;
    std::string absoluteMrl( const std::string& relativeMrl ) const override;
    const std::string& scheme() const override;

private:
    struct Mountpoint
    {
        Mountpoint( std::string m )
            : mrl( std::move( m ) )
            , url( mrl )
        {
        }
        std::string mrl;
        vlc::url url;
    };
    std::string m_uuid;
    std::vector<Mountpoint> m_mountpoints;
    std::string m_scheme;
    bool m_removable;
    bool m_isNetwork;
};

  } /* namespace medialibrary */
} /* namespace vlc */

#endif
