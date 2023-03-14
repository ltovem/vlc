--[[

 Copyright © 2007 the VideoLAN team

 SPDX-License-Identifier: GPL-2.0-or-later
--]]

-- Probe function.
function probe()
    local path = vlc.path:gsub("^www%.", "")
    return vlc.access == "http"
           and string.match( path, "^break%.com/video/.+" )
end

-- Parse function.
function parse()
    filepath = ""
    filename = ""
    filetitle = ""
    arturl = ""
    while true do
        line = vlc.readline()
        if not line then break end
        if string.match( line, "sGlobalContentFilePath=" ) then
            _,_,filepath= string.find( line, "sGlobalContentFilePath='(.-)'" )
        end
        if string.match( line, "sGlobalFileName=" ) then
            _,_,filename = string.find( line, ".*sGlobalFileName='(.-)'")
        end
        if string.match( line, "sGlobalContentTitle=" ) then
            _,_,filetitle = string.find( line, "sGlobalContentTitle='(.-)'")
        end
        if string.match( line, "el=\"videothumbnail\" href=\"" ) then
            _,_,arturl = string.find( line, "el=\"videothumbnail\" href=\"(.-)\"" )
        end
        if string.match( line, "videoPath" ) then
            _,_,videopath = string.find( line, ".*videoPath', '(.-)'" )
            return { { path = videopath..filepath.."/"..filename..".flv"; title = filetitle; arturl = arturl } }
        end
    end
end
