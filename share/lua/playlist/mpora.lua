--[[

 Copyright © 2009 the VideoLAN team

 Authors: Konstantin Pavlov (thresh@videolan.org)

 SPDX-License-Identifier: GPL-2.0-or-later
--]]

-- Probe function.
function probe()
    return ( vlc.access == "http" or vlc.access == "https" )
        and string.match( vlc.path, "^mpora%.com/videos/" )
end

-- Parse function.
function parse()
    p = {}
    while true do
        -- Try to find the video's title
        line = vlc.readline()
        if not line then break end
        if string.match( line, "meta name=\"title\"" ) then
            _,_,name = string.find( line, "content=\"(.*)\" />" )
        end
        if string.match( line, "image_src" ) then
            _,_,arturl = string.find( line, "image_src\" href=\"(.*)\" />" )
        end
        if string.match( line, "video_src" ) then
            _,_,video = string.find( line, 'href="http://video%.mpora%.com/ep/(.*)%.swf" />' )
        end

    end

    if not name or not arturl or not video then return nil end

    -- Try and get URL for SD video.
    sd = vlc.stream("http://api.mpora.com/tv/player/playlist/vid/"..video.."/")
    if not sd then return nil end
    page = sd:read( 65653 )
    sdurl = string.match( page, "url=\"(.*)\" />")
    page = nil

    table.insert( p, { path = sdurl; name = name; arturl = arturl; } )

    -- Try and check if HD video is available.
    checkhd = vlc.stream("http://api.mpora.com/tv/player/load/vid/"..video.."/platform/video/domain/video.mpora.com/" )
    if not checkhd then return nil end
    page = checkhd:read( 65653 )
    hashd = tonumber( string.match( page, "<has_hd>(%d)</has_hd>" ) )
    page = nil

    if hashd then
        hd = vlc.stream("http://api.mpora.com/tv/player/playlist/vid/"..video.."/hd/true/")
        page = hd:read( 65653 )
        hdurl = string.match( page, "url=\"(.*)\" />")
        table.insert( p, { path = hdurl; name = name.." (HD)"; arturl = arturl; } )
    end

    return p
end
