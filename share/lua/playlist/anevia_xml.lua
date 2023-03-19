--[[
 Parse list of available streams on Anevia servers.
 The URI http://ipaddress/ws/Mgmt/* describes a list of
 available streams on the server.

 Copyright © 2009 M2X BV

 Authors: Jean-Paul Saman <jpsaman@videolan.org>

 SPDX-License-Identifier: GPL-2.0-or-later
--]]

-- Probe function.
function probe()
    return vlc.access == "http"
        and string.match( vlc.path, "/ws/Mgmt/" )
end

-- Fake readline. Read <>(..*)</> whole, otherwise pretend a newline.
-- In lua, indices are from 1, not 0, so no +1 needed.
function readline()
    local n = string.find(vlc.peek(998),"><") -- A random large number
    return n and vlc.read(n) or vlc.readline()
end

-- Parse function.
function parse()
    local p = {}
    local line
    _,_,server = string.find( vlc.path, "(.*)/ws/Mgmt/" )
    while true do
        line = readline()
        if not line then break end
        if string.match( line, "<struct name=\"stream\">" ) then
            while true do
                line = readline()
                if not line then break end
                if string.match( line, "<field name=\"name\">" ) then
                    _,_,name = string.find( line, "name=\"name\">(.*)</field>" )
                end
                if string.match( line, "<choice name=\"destination\">" ) then
                    while true do
                        line = readline()
                        if not line then break end
                        if string.match( line, "<struct name=\"(.*)\">" ) then
                            _,_,protocol = string.find( line, "<struct name=\"(.*)\">" )
                            while true do
                                line = readline()
                                if not line then break end
                                if string.match( line, "<field name=\"address\">(.*)</field>" ) then
                                    _,_,address = string.find( line, "<field name=\"address\">(.*)</field>" )
                                end
                                if string.match( line, "<field name=\"port\">(.*)</field>" ) then
                                    _,_,port = string.find( line, "<field name=\"port\">(.*)</field>" )
                                end
                                -- end of struct tag
                                if string.match( line, "</struct>" ) then
                                    media = tostring(protocol) .. "://@" .. tostring(address) .. ":" .. tostring(port)
                                    table.insert( p, { path = media; name = name, url = media } )
                                    break
                                end
                            end
                        end
                        if not line then break end
                        -- end of choice tag
                        if string.match( line, "</choice>" ) then break end
                    end
                end
            end
        end

    end
    return p
end
