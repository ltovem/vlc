--[[

 Copyright © 2016, 2019-2020, 2022 the VideoLAN team

 Authors: Pierre Ynard

 SPDX-License-Identifier: GPL-2.0-or-later
--]]

-- Probe function.
function probe()
    return ( vlc.access == "http" or vlc.access == "https" )
        and string.match( vlc.path, "^vocaroo%.com/." )
end

-- Parse function.
function parse()
    -- The HTML page contains no metadata and is not worth parsing
    local id = string.match( vlc.path, "^vocaroo%.com/([^?]+)" )

    -- Dispatch media to correct CDN server
    -- function Ic(e){return function(e){if(e.length){if(11==e.length)return ControlConfig.mediaMp3FileUrl;if(12==e.length&&"1"==e[0])return ControlConfig.mediaMp3FileUrl1;if(10==e.length)return ControlConfig.mediaMp3FileUrl1}return ControlConfig.mediaMp3FileUrl}(e)+e}
    local cdn = ( string.len( id ) == 10 or
                  ( string.len( id ) == 12 and string.match( id, "^1" ) ) )
        and "//media1.vocaroo.com/mp3/"
        or "//media.vocaroo.com/mp3/"

    local path = vlc.access..":"..cdn..id
    return { { path = path; options = { ":http-referrer="..vlc.access.."://"..vlc.path } } }
end

