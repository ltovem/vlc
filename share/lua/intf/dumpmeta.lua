--[==========================================================================[
dumpmeta.lua: dump a file's meta data on stdout/stderr
--[==========================================================================[
 Copyright (C) 2010 the VideoLAN team

 Authors: Antoine Cellerier <dionoea at videolan dot org>

 SPDX-License-Identifier: GPL-2.0-or-later
--]==========================================================================]

--[[ to dump meta data information in the debug output, run:
       vlc -I luaintf --lua-intf dumpmeta coolmusic.mp3
     Additional options can improve performance and output readability:
       -V dummy -A dummy --no-video-title --no-media-library -q
--]]

local item
repeat
    item = vlc.player.item()
until (item and item:is_preparsed())

-- preparsing doesn't always provide all the information we want (like duration)
repeat
until item:stats()["demux_read_bytes"] > 0

vlc.msg.info("name: "..item:name())
vlc.msg.info("uri: "..vlc.strings.decode_uri(item:uri()))
vlc.msg.info("duration: "..tostring(item:duration()))

vlc.msg.info("meta data:")
local meta = item:metas()
if meta then
    for key, value in pairs(meta) do
        vlc.msg.info("  "..key..": "..value)
    end
else
    vlc.msg.info("  no meta data available")
end

vlc.msg.info("info:")
for cat, data in pairs(item:info()) do
    vlc.msg.info("  "..cat)
    for key, value in pairs(data) do
        vlc.msg.info("    "..key..": "..value)
    end
end

vlc.misc.quit()
