--[[

 Copyright © 2008 the VideoLAN team

 Authors: Dominique Leuenberger <dominique-vlc.suse@leuenberger.net>

 SPDX-License-Identifier: GPL-2.0-or-later
--]]

-- Probe function.
function probe()
    local path = vlc.path:gsub("^www%.", "")
    return vlc.access == "http"
        and string.match( path, "^bbc%.co%.uk/iplayer/.+" )
end

-- Parse function.
function parse()
    p = {}
    while true do
        -- Try to find the video's title
        line = vlc.readline()
        if not line then break end
        if string.match( line, "title: " ) then
            _,_,name = string.find( line, "title: \"(.*)\"" )
        end
        if string.match( line, "metaFile: \".*%.ram\"" ) then
            _,_,video = string.find( line, "metaFile: \"(.-)\"" )
            table.insert( p, { path = video; name = name } )
        end
    end
    return p
end
