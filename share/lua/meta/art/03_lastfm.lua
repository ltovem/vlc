--[[
 Gets an artwork from last.fm

 Copyright © 2010 the VideoLAN team

 SPDX-License-Identifier: GPL-2.0-or-later
--]]

function descriptor()
    return { scope="network" }
end

-- Return the artwork
function fetch_art()
    if vlc.item == nil then return nil end
    local meta = vlc.item:metas()

    if meta["Listing Type"] == "radio"
    or meta["Listing Type"] == "tv"
    then return nil end

    if meta["artist"] and meta["album"] then
        title = vlc.strings.encode_uri_component(meta["artist"]).."/"..vlc.strings.encode_uri_component(meta["album"])
    else
        return nil
    end
    -- remove -.* from string
    title = string.gsub( title, " ?%-.*", "" )
    -- remove (info..) from string
    title = string.gsub( title, "%(.*%)", "" )
    -- remove CD2 etc from string
    title = string.gsub( title, "CD%d+", "" )
    -- remove Disc \w+ from string
    title = string.gsub( title, "Disc %w+", "" )
    fd = vlc.stream( "http://www.last.fm/music/" .. title )
    if not fd then return nil end
    page = fd:read( 65653 )
    fd = nil
    _, _, arturl = string.find( page, "<meta property=\"og:image\" content=\"([^\"]+)\" />" )
    -- Don't use default album-art (not found one)
    if not arturl or string.find( arturl, "default_album_mega.png") then
       return nil
    end
    return arturl
end
