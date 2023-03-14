--[[

 Parse list of available streams on Anevia Toucan servers.
 The URI http://ipaddress:554/list_streams.idp describes a list of
 available streams on the server.

 Copyright © 2009 M2X BV

 Authors: Jean-Paul Saman <jpsaman@videolan.org>

 SPDX-License-Identifier: GPL-2.0-or-later
--]]

-- Probe function.
function probe()
    return vlc.access == "http"
        and string.match( vlc.path, "/list_streams%.idp" )
end

-- Parse function.
function parse()
    p = {}
    _,_,server = string.find( vlc.path, "(.*)/list_streams.idp" )
    while true do
        line = vlc.readline()
        if not line then break end

        if string.match( line, "<streams[-]list> </stream[-]list>" ) then
            break
        elseif string.match( line, "<streams[-]list xmlns=\"(.*)\">" ) then
            while true do
                line = vlc.readline()
                if not line then break end
                if string.match( line, "<streamer name=\"(.*)\"> </streamer>" ) then
                    break;
                elseif string.match( line, "<streamer name=\"(.*)\">" ) then
                    _,_,streamer = string.find( line, "name=\"(.*)\"" )
                    while true do
                        line = vlc.readline()
                        if not line then break end
                        -- ignore <host name=".." /> tags
                        -- ignore <port number=".." /> tags
                        if string.match( line, "<input name=\"(.*)\">" ) then
                            _,_,path = string.find( line, "name=\"(.*)\"" )
                            while true do
                                line = vlc.readline()
                                if not line then break end
                                if string.match( line, "<stream id=\"(.*)\" />" ) then
                                    _,_,media = string.find( line, "id=\"(.*)\"" )
                                    video = "rtsp://" .. tostring(server) .. "/" .. tostring(path) .. "/" .. tostring(media)
                                    vlc.msg.dbg( "adding to playlist " .. tostring(video) )
                                    table.insert( p, { path = video; name = media, url = video } )
                                end
--                              end of input tag found
                                if string.match( line, "</input>" ) then
                                    break
                                end
                            end
                        end
                    end
                    if not line then break end
--                  end of streamer tag found
                    if string.match( line, "</streamer>" ) then
                        break
                    end
                end
                if not line then break end
--              end of streams-list tag found
                if string.match( line, "</streams-list>" ) then
                    break
                end
            end
        end

    end
    return p
end
