--[[

 Copyright © 2008 the VideoLAN team

 Authors: Antoine Cellerier <dionoea at videolan dot org>

 SPDX-License-Identifier: GPL-2.0-or-later
--]]

-- Probe function.
function probe()
    return vlc.access == "http"
        and string.match( vlc.path, "^www%.francetvinfo%.fr/replay%-jt/.+" )
end

-- Parse function.
function parse()
    p = {}
    while true do
        -- Try to find the video's title
        line = vlc.readline()
        if not line then break end
        if string.match( line, "class=\"editiondate\"" ) then
            _,_,editiondate = string.find( line, "<h1>(.-)</h1>" )
        end
        if string.match( line, "mms.*%.wmv" ) then
            _,_,video = string.find( line, "mms(.-%.wmv)" )
            video = "mmsh"..video
            table.insert( p, { path = video; name = editiondate } )
        end
        if string.match( line, "class=\"subjecttimer\"" ) then
            oldtime = time
            _,_,time = string.find( line, "href=\"(.-)\"" )
            if oldtime then
                table.insert( p, { path = video; name = name; duration = time - oldtime; options = { ':start-time='..tostring(oldtime); ':stop-time='..tostring(time) } } )
            end
            name = vlc.strings.resolve_xml_special_chars( string.gsub( line, "^.*>(.*)<..*$", "%1" ) )
        end
    end
    if oldtime then
        table.insert( p, { path = video; name = name; options = { ':start-time='..tostring(time) } } )
    end
    return p
end
