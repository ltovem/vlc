--[[
 Gets an artwork from images.google.com

 Copyright © 2007 the VideoLAN team

 SPDX-License-Identifier: GPL-2.0-or-later
--]]

function descriptor()
    return { scope="network" }
end

-- Return the artwork
function fetch_art()
    if vlc.item == nil then return nil end

    local meta = vlc.item:metas()

-- Radio Entries
    if meta["Listing Type"] == "radio"
    then
        title = meta["title"] .. " radio logo"
-- TV Entries
    elseif meta["Listing Type"] == "tv"
    then
        title = meta["title"] .. " tv logo"
-- Album entries
    elseif meta["artist"] and meta["album"] then
        title = meta["artist"].." "..meta["album"].." cover"
    elseif meta["artist"] and meta["title"] then
        title = meta["artist"].." "..meta["title"].." cover"
    else
        return nil
    end
    fd = vlc.stream( "http://images.google.com/images?q="..vlc.strings.encode_uri_component( title ) )
    if not fd then return nil end

    page = fd:read( 65653 )
    fd = nil
    _, _, _, arturl = string.find( page, "<img height=\"([0-9]+)\" src=\"([^\"]+gstatic.com[^\"]+)\"" )
    return arturl
end
