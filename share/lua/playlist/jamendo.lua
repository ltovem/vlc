--[[

 Copyright © 2010 VideoLAN and AUTHORS

 Authors: Fabio Ritrovato <sephiroth87 at videolan dot org>

 SPDX-License-Identifier: GPL-2.0-or-later
--]]

local simplexml = require "simplexml"

-- Probe function.
function probe()
    return vlc.access == "http"
        and string.match( vlc.path, "^api%.jamendo%.com/" )
        and string.match( vlc.path, "get2" )
        and string.match( vlc.path, "track" )
        and string.match( vlc.path, "xml" )
end

-- Parse function.
function parse()
    local page = ""
    while true do
        local line = vlc.readline()
        if line == nil then break end
        page = page .. line
    end
    local tracks = {}
    local tree = simplexml.parse_string( page )
    for _, track in ipairs( tree.children ) do
        simplexml.add_name_maps( track )
        if track.children_map["id"] == nil and
           track.children_map["stream"] == nil then
            vlc.msg.err( "No track id or stream URL, not enough info to add tracks..." )
            return {}
        end
        local stream_url
        if track.children_map["id"][1].children[1] then
            stream_url = "http://api.jamendo.com/get2/stream/track/redirect/?id=" .. track.children_map["id"][1].children[1]
        else
            stream_url = track.children_map["stream"][1].children[1]
        end
        table.insert( tracks, {path=stream_url,
                               arturl=track.children_map["album_image"] and track.children_map["album_image"][1].children[1] or ( track.children_map["album_id"] and "http://imgjam.com/albums/".. track.children_map["album_id"][1].children[1] .. "/covers/1.500.jpg" or nil ),
                               title=track.children_map["name"] and track.children_map["name"][1].children[1] or nil,
                               artist=track.children_map["artist_name"] and track.children_map["artist_name"][1].children[1] or nil,
                               album=track.children_map["album_name"] and track.children_map["album_name"][1].children[1] or nil,
                               genre=track.children_map["album_genre"] and track.children_map["album_genre"][1].children[1] or nil,
                               duration=track.children_map["duration"] and track.children_map["duration"][1].children[1] or nil,
                               date=track.children_map["album_dates"] and track.children_map["album_dates"][1].children_map["year"][1].children[1] or nil} )
    end
    return tracks
end
