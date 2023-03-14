--[[

 Copyright © 2011 the VideoLAN team

 Authors: Konstantin Pavlov (thresh@videolan.org)

 SPDX-License-Identifier: GPL-2.0-or-later
--]]

-- Probe function.
function probe()
    local path = vlc.path:gsub("^www%.", "")
    return vlc.access == "http"
        and ( string.match( vlc.path, "^zapiks%.fr/(.*)%.html" )
            or string.match( vlc.path, "^zapiks%.fr/view/.+" )
            or string.match( vlc.path, "^26in%.fr/videos/.+" ) )
end

-- Parse function.
function parse()
    if string.match ( vlc.path, "zapiks.fr/(.+).html" ) or string.match( vlc.path, "26in.fr/videos/" ) then
        while true do
            line = vlc.readline()
            if not line then break end
            -- Try to find video id number
            if string.match( line, "video_src(.+)file=(%d+)\"" ) then
                _,_,id = string.find( line, "file=(%d+)")
            end
            -- Try to find title
            if string.match( line, "(.*)</title>" ) then
                _,_,name = string.find( line, "(.*)</title>" )
            end
        end
        return { { path = "http://www.zapiks.fr/view/index.php?file=" .. id, name = name } }
    end

    if string.match ( vlc.path, "zapiks.fr/view/." ) then
        prefres = vlc.var.inherit(nil, "preferred-resolution")
        while true do
            line = vlc.readline()
            if not line then break end
            -- Try to find URL of video
            if string.match( line, "<file>(.*)</file>" ) then
                _,_,path = string.find ( line, "<file>(.*)</file>" )
            end
            -- Try to find image for arturl
            if string.match( line, "<image>(.*)</image>" ) then
                _,_,arturl = string.find( line, "<image>(.*)</image>" )
            end
            if string.match( line, "title=\"(.*)\"" ) then
                _,_,name = string.find( line, "title=\"(.*)\"" )
            end
            -- Try to find whether video is HD actually
            if( prefres <0 or prefres >= 720 ) then
                if string.match( line, "<hd.file>(.*)</hd.file>" ) then
                    _,_,path = string.find( line, "<hd.file>(.*)</hd.file>" )
                end
            end
        end
        return { { path = path; name = name; arturl = arturl } }
    end
    vlc.msg.err( "Could not extract the video URL from zapiks.fr" )
    return {}
end
