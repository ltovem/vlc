--[==========================================================================[
 telnet.lua: wrapper for legacy telnet configuration
--[==========================================================================[
 Copyright (C) 2011 the VideoLAN team

 Authors: Pierre Ynard

 SPDX-License-Identifier: GPL-2.0-or-later
--]==========================================================================]

if config.hosts == nil and config.host == nil then
    config.host = "telnet://localhost:4212"
else
    if config.hosts == nil then
        config.hosts = { config.host }
    end

    for i,host in pairs(config.hosts) do
        if host ~= "*console" then
            local proto = string.match(host, "^%s*(.-)://")
            if proto == nil or proto ~= "telnet" then
                local newhost
                if proto == nil then
                    newhost = "telnet://"..host
                else
                    newhost = string.gsub(host, "^%s*.-://", "telnet://")
                end
                --vlc.msg.warn("Replacing host `"..host.."' with `"..newhost.."'")
                config.hosts[i] = newhost
            end
        end
    end
end

--[[ Launch vlm ]]
vlm = vlc.vlm()

dofile(wrapped_file)

