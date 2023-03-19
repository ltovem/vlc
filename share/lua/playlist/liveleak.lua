--[[

 Copyright © 2012, 2017, 2021 VideoLAN and AUTHORS

 Authors: Ludovic Fauvet <etix@videolan.org>
          Pierre Ynard

 SPDX-License-Identifier: GPL-2.0-or-later
--]]

-- Probe function.
function probe()
    return ( vlc.access == "http" or vlc.access == "https" ) and (
               string.match( vlc.path, "^www%.liveleak%.com/v%?" )
            or string.match( vlc.path, "^www%.liveleak%.com/view%?" )
           )
end

-- Parse function.
function parse()
    local p = {}
    local title
    local art
    local video

    while true do
        line = vlc.readline()
        if not line then break end

        -- Try to find the title
        if not title then
            title = string.match( line, "shareTitle: *'(.-[^\\])'" )
            if title then
                if string.match( title, "^'" ) then
                    title = nil
                else
                    -- FIXME: do this properly (see #24958)
                    title = string.gsub( title, "\\'", "'" )
                end
            end
        end

        -- Try to find the art
        if not art then
            art = string.match( line, '<meta property="og:image" content="([^"]+)"' )
        end

        -- Try to find the video
        if not video and string.match( line, '<video ' ) then
            while not string.match( line, '</video>') do
                local more = vlc.readline()
                if not more then break end
                line = line..more
            end

            -- Apparently the two formats are listed HD first, SD second
            local prefres = vlc.var.inherit( nil, 'preferred-resolution' )
            for source in string.gmatch( line, '<source( .-)>' ) do
                local src = string.match( source, ' src="([^"]+)"' )
                if src then
                    video = vlc.strings.resolve_xml_special_chars( src )

                    if prefres < 0 then
                        break
                    end
                    local height = tonumber( string.match( source, ' label="(%d+).-"' ) )
                    if ( not height ) or height <= prefres then
                        break
                    end
                end
            end
        end

    end
    if video then
        table.insert( p, { path = video; name = title; arturl = art; } )
    end
    return p
end

