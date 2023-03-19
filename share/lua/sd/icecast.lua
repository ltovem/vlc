--[[

 Copyright © 2010 VideoLAN and AUTHORS

 Authors: Fabio Ritrovato <sephiroth87 at videolan dot org>

 SPDX-License-Identifier: GPL-2.0-or-later
--]]

local simplexml = nil

function lazy_load()
    if simplexml == nil
    then
        simplexml = require "simplexml"
    end
end

function descriptor()
    return { title="Icecast Radio Directory" }
end

function dropnil(s)
    if s == nil then return "" else return s end
end

function main()
    lazy_load()
    local tree = simplexml.parse_url("http://dir.xiph.org/yp.xml")
    for _, station in ipairs( tree.children ) do
        simplexml.add_name_maps( station )
        if not( station.children_map["listen_url"][1].children[1] == nil )
        then
            local station_name = station.children_map["server_name"][1].children[1]
            if station_name == "Unspecified name" or station_name == "" or station_name == nil
            then
                    station_name = station.children_map["listen_url"][1].children[1]
                    if string.find( station_name, "radionomy.com" )
                    then
                            station_name = string.match( station_name, "([^/]+)$")
                            station_name = string.gsub( station_name, "-", " " )
                    end
            end
            vlc.sd.add_item( {path=station.children_map["listen_url"][1].children[1],
                              title=station_name,
                              genre=dropnil(station.children_map["genre"][1].children[1]),
                              nowplaying=dropnil(station.children_map["current_song"][1].children[1]),
                              uiddata=station.children_map["listen_url"][1].children[1]
                                      .. dropnil(station.children_map["server_name"][1].children[1]),
                              meta={
                                      ["Listing Source"]="dir.xiph.org",
                                      ["Listing Type"]="radio",
                                      ["Icecast Bitrate"]=dropnil(station.children_map["bitrate"][1].children[1]),
                                      ["Icecast Server Type"]=dropnil(station.children_map["server_type"][1].children[1])
                              }} )
        end
    end
end
