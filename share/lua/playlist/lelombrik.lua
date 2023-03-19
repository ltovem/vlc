--[[
 French humor site: http://lelombrik.net


 Copyright © 2007 the VideoLAN team

 SPDX-License-Identifier: GPL-2.0-or-later
--]]

-- Probe function.
function probe()
    local path = vlc.path:gsub("^www%.", "")
    return vlc.access == "http"
        and string.match( path, "^lelombrik%.net/videos" )
end

-- Parse function.
function parse()
    while true do
        line = vlc.readline()
        if not line then
            vlc.msg.err("Couldn't extract the video URL from lelombrik")
            return { }
        end

        if string.match( line, "id=\"nom_fichier\">" ) then
            title = string.gsub( line, ".*\"nom_fichier\">([^<]*).*", "%1" )
            if title then
                title = vlc.strings.from_charset( "ISO_8859-1", title )
            end
        elseif string.match( line, "'file'" ) then
            _,_,path = string.find( line, "'file', *'([^']*)")
        elseif string.match( line, "flashvars=" ) then
            path = string.gsub( line, "flashvars=.*&file=([^&]*).*", "%1" )
            arturl = string.gsub( line, "flashvars=.*&image=([^&]*).*", "%1" )
        elseif string.match( line, "'image'" ) then
            _,_,arturl = string.find( line, "'image', *'([^']*)")
        end

        if path and arturl and title then
            return { { path = path; arturl = arturl; title = title } }
        end
    end
end
