/*****************************************************************************
 * decoder_device.h: Decoder device and video context
 *****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
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

#ifndef VLC_DECODER_DEVICE_INTERNAL_H
#define VLC_DECODER_DEVICE_INTERNAL_H 1

#include "../modules/modules.h"

void
vlc_decoder_device_GetModuleDesc(vlc_decoder_device *device,
                                 struct vlc_module_desc *desc);

const char *
vlc_video_context_GetStringType(const vlc_video_context *vctx);

#endif
