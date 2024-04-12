/*****************************************************************************
 * FileHandler.cpp
 *****************************************************************************
 * Copyright © 2024 VLC authors and VideoLAN
 *
 * Authors: Alaric Senat <alaric@videolabs.io>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <ctime>
#include <sstream>

#include <vlc_common.h>

#include <vlc_addons.h>
#include <vlc_cxx_helpers.hpp>
#include <vlc_fourcc.h>
#include <vlc_interface.h>
#include <vlc_player.h>
#include <vlc_rand.h>
#include <vlc_stream.h>
#include <vlc_stream_extractor.h>
#include <vlc_url.h>

#include "FileHandler.hpp"
#include "ml.hpp"
#include "utils.hpp"

// static constexpr char DLNA_TRANSFER_MODE[] = "transfermode.dlna.org";
static constexpr char DLNA_CONTENT_FEATURE[] = "contentfeatures.dlna.org";
static constexpr char DLNA_TIME_SEEK_RANGE[] = "timeseekrange.dlna.org";

/// Convenient C++ replacement of vlc_ml_file_t to not have to deal with allocations
struct MLFile
{
    std::string mrl;
    int64_t size;
    time_t last_modification;
};

/// Usual filesystem FileHandler implementation, this is the most commonly used FileHandler, for
/// non-transcoded local medias, thumbnails and subs.
class MLFileHandler : public FileHandler
{
  public:
    MLFile file;
    utils::MimeType mime_type;

    std::unique_ptr<stream_t, decltype(&vlc_stream_Delete)> stream = {nullptr, &vlc_stream_Delete};

    MLFileHandler(MLFile &&file, utils::MimeType &&mime_type) :
        file(std::move(file)),
        mime_type(std::move(mime_type))
    {}

    bool get_info(UpnpFileInfo &info) final
    {
        UpnpFileInfo_set_ContentType(&info, mime_type.combine().c_str());
        UpnpFileInfo_set_FileLength(&info, file.size);
        UpnpFileInfo_set_LastModified(&info, file.last_modification);

        // const_cast is expected by the design of the upnp api as it only serves const list heads
        // FIXME: see if there's no way to patch that in libupnp, we shouldn't have to break const
        // to do something so usual.
        auto *head = const_cast<UpnpListHead *>(UpnpFileInfo_get_ExtraHeadersList(&info));
        utils::http::add_response_hdr(head, {DLNA_CONTENT_FEATURE, "DLNA.ORG_OP=01"});
        return true;
    }

    bool open(vlc_object_t *parent) final
    {
        stream = vlc::wrap_cptr(vlc_stream_NewMRL(parent, file.mrl.c_str()), &vlc_stream_Delete);
        return stream != nullptr;
    }

    size_t read(uint8_t buffer[], size_t buffer_len) noexcept final
    {
        return vlc_stream_Read(stream.get(), buffer, buffer_len);
    }

    bool seek(SeekType type, off_t offset) noexcept final
    {
        uint64_t real_offset;
        switch (type)
        {
            case SeekType::Current:
                real_offset = vlc_stream_Tell(stream.get()) + offset;
                break;
            case SeekType::End:
                if (vlc_stream_GetSize(stream.get(), &real_offset) != VLC_SUCCESS)
                {
                    return false;
                }
                real_offset += offset;
                break;
            case SeekType::Set:
                real_offset = offset;
                break;
            default:
                return false;
        }

        return vlc_stream_Seek(stream.get(), real_offset) == 0;
    }
};

//
// Url parsing and FileHandler Factory
//

template <typename MLHelper>
auto get_ml_object(const std::string &token,
                   std::string &extension,
                   const ml::MediaLibraryContext &ml)
{
    const auto extension_idx = token.find('.');
    extension = token.substr(extension_idx + 1);

    try
    {
        const int64_t ml_id = std::stoll(token.substr(0, extension_idx));
        return MLHelper::get(ml, ml_id);
    }
    catch (const std::exception &)
    {
        return typename MLHelper::Ptr{nullptr, nullptr};
    }
}

static std::unique_ptr<FileHandler> parse_media_url(std::stringstream &ss,
                                                    const ml::MediaLibraryContext &ml)
{
    std::string token;
    std::getline(ss, token, '/');

    if (token != "native")
    {
        // TODO Select a transcode profile
    }

    std::getline(ss, token);

    std::string extension;
    const auto media = get_ml_object<ml::Media>(token, extension, ml);
    if (media == nullptr)
    {
        return nullptr;
    }

    const auto main_files = utils::get_media_files(*media, VLC_ML_FILE_TYPE_MAIN);
    if (main_files.empty())
    {
        return nullptr;
    }
    const vlc_ml_file_t &main_file = main_files.front();

    auto mime_type = utils::get_mimetype(media->i_type, extension);
    auto ret = std::make_unique<MLFileHandler>(
        MLFile{main_file.psz_mrl, main_file.i_size, main_file.i_last_modification_date},
        std::move(mime_type));
    return ret;
}

std::unique_ptr<FileHandler> parse_url(const char *url, const ml::MediaLibraryContext &ml)
{
    std::stringstream ss(url);

    std::string token;
    std::getline(ss, token, '/');
    if (!token.empty())
        return nullptr;

    std::getline(ss, token, '/');
    if (token == "media")
        return parse_media_url(ss, ml);
    return nullptr;
}
