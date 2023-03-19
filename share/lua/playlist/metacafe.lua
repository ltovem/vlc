--[[

 Copyright © 2007 the VideoLAN team

 SPDX-License-Identifier: GPL-2.0-or-later
--]]

-- Probe function.
function probe()
    local path = vlc.path:gsub("^www%.", "")
    return vlc.access == "http"
        and string.match( vlc.path, "^metacafe%.com/" )
        and (  string.match( vlc.path, "watch/" )
            or string.match( vlc.path, "mediaURL=" ) )
end

-- Parse function.
function parse()
    vlc.msg.warn("FIXME")
    if string.match( vlc.path, "watch/" )
    then -- This is the HTML page's URL
        while true do
            -- Try to find the video's title
            line = vlc.readline()
            if not line then break end
            if string.match( line, "<meta name=\"title\"" ) then
                _,_,name = string.find( line, "content=\"Metacafe %- (.-)\"" )
            end
            if string.match( line, "<meta name=\"description\"" ) then
                _,_,description = string.find( line, "content=\"(.-)\"" )
            end
            if string.match( line, "<link rel=\"image_src\"" ) then
                _,_,arturl = string.find( line, "href=\"(.-)\"" )
            end
            if name and description and arturl then break end
        end
        return { { path = string.gsub( vlc.path, "^.*watch/(.*[^/])/?$", "http://www.metacafe.com/fplayer/%1.swf" ); name = name; description = description; arturl = arturl;  } }
    else -- This is the flash player's URL
        local _,_,path = string.find( vlc.path, "mediaURL=([^&]*)" )
        return { { path = path } }
    end
end
