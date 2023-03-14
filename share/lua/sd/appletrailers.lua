--[[

 Copyright © 2010 VideoLAN and AUTHORS

 Authors: Ilkka Ollakka <ileoo at videolan dot org >

 SPDX-License-Identifier: GPL-2.0-or-later
--]]

function descriptor()
    return { title="Apple Trailers" }
end

function find( haystack, needle )
    local _,_,r = string.find( haystack, needle )
    return r
end

function main()
    fd = vlc.stream( "http://trailers.apple.com/trailers/home/feeds/just_hd.json" )
    if not fd then return nil end
    line = fd:readline()
    while line ~= nil
    do
         if string.match( line, "title" ) then
            title = vlc.strings.resolve_xml_special_chars( find( line, "title\":\"(.-)\""))
            art = find( line, "poster\":\"(.-)\"")
            if string.match( art, "http://" ) then
            else
                art = "http://trailers.apple.com"..art
            end

            url = find( line, "location\":\"(.-)\"")
            node = vlc.sd.add_item( {title  = title,
                                     path   = "http://trailers.apple.com"..url.."includes/playlists/web.inc",
                                     arturl = art})

         end
         line = fd:readline()
    end
end
