--[[

 Copyright © 2011 the VideoLAN team

 Authors: Konstantin Pavlov (thresh@videolan.org)

 SPDX-License-Identifier: GPL-2.0-or-later
--]]

-- Probe function.
function probe()
    local path = vlc.path:gsub("^www%.", "")
    return vlc.access == "http"
        and ( string.match( vlc.path, "^extreme%.com/.+" )
        or string.match( vlc.path, "^freecaster%.tv/.+" )
        or string.match( vlc.path, "^player%.extreme%.com/info/.+") )
end

-- Parse function.
function parse()
    if (string.match( vlc.path, "extreme%.com/." ) or string.match( vlc.path, "freecaster%.tv/." )) and not string.match( vlc.path, "player%.extreme%.com/info/") then
        while true do
            line = vlc.readline()
            if not line then break end
            -- Try to find id of the video
            if string.match( line, "http://player.extreme.com/FCPlayer.swf" ) then
                _,_,vid = string.find( line, "id=(.*)\"" )
                break
            end
        end
        return { { path = "http://player.extreme.com/info/" .. vid; name = "extreme.com video"; } }
    end

    if string.match( vlc.path, "player%.extreme%.com/info/." ) then
        prefres = vlc.var.inherit(nil, "preferred-resolution")
        gostraight = true
        while true do
            line = vlc.readline()
            if not line then break end
            -- Try to find the video's title
            if string.match( line, "title>(.*)<" ) then
                _,_,name = string.find( line, "<title>(.*)<" )
            end

            -- Try to find image for thumbnail
            if string.match( line, "<path>(*.)</path>" ) then
                _,_,arturl = string.find( line, "<path>(*.)</path>" )
            end

            -- Try to find out if its a freecaster streaming or just a link to some
            -- other video streaming website
            -- We assume freecaster now streams in http
            if string.match( line, "<streams type=\"5\" server=\"(.*)\">" )
                then
                _,_,videoserver = string.find( line, "<streams type=\"5\" server=\"(.*)\">" )
                gostraight = false
            end

            -- if we're going outside, we need to find out the path
            if gostraight then
                if string.match( line, ">(.*)</stream>" ) then
                    _,_,path = string.find( line, "bitrate=\"0\" duration=\"\">(.*)</stream>" )
                end
            end

            -- and if we're using freecaster, use appropriate resolution
            if not gostraight then
                if string.match( line, "height=\"(.*)\" duration" ) then
                    _,_,height = string.find( line, "height=\"(%d+)\" duration" )
                    _,_,playpath  = string.find( line, "\">(.*)</stream>" )
                    if ( prefres < 0 or tonumber( height ) <= prefres ) then
                        path = videoserver .. playpath
                    end
                end
            end
        end

        return { { path = path; name = name; arturl = arturl } }

    end

end
