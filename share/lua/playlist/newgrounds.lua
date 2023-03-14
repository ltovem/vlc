--[[

 Copyright © 2016 the VideoLAN team

 Authors: Pierre Ynard

 SPDX-License-Identifier: GPL-2.0-or-later
--]]

-- Probe function.
function probe()
    return ( vlc.access == "http" or vlc.access == "https" )
        and string.match( vlc.path, "^www%.newgrounds%.com/.*/%d+" )
end

-- Parse function.
function parse()
    local path, title, description, arturl, author
    while true do
        local line = vlc.readline()
        if not line then break end

        if not title then
            title = string.match( line, "<meta property=\"og:title\" content=\"(.-)\"" )
            if title then
                title = vlc.strings.resolve_xml_special_chars( title )
            end
        end

        if not description then
            description = string.match( line, "<meta property=\"og:description\" content=\"(.-)\"" )
            if description then
                description = vlc.strings.resolve_xml_special_chars( description )
            end
        end

        if not arturl then
            arturl = string.match( line, "<meta property=\"og:image\" content=\"(.-)\"" )
            if arturl then
                arturl = vlc.strings.resolve_xml_special_chars( arturl )
            end
        end

        if not author then
            author = string.match( line, "<em>Author <a [^>]*>([^<]+)</a></em>" )
            if author then
                author = vlc.strings.resolve_xml_special_chars( author )
            end
        end

        if not path then
            path = string.match( line, 'new embedController%(%[{"url":"([^"]+)"' )
            if path then
                path = string.gsub( path, "\\/", "/" )
            end
        end
    end

    if not path then
        vlc.msg.err( "Couldn't extract newgrounds media URL" )
        return { }
    end

    return { { path = path, name = title, description = description, arturl = arturl, artist = author } }
end
