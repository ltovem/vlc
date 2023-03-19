--[[

 Copyright © 2009, 2016 the VideoLAN team

 SPDX-License-Identifier: GPL-2.0-or-later
--]]

-- Parser script from Rockbox FM radio presets
-- See http://www.rockbox.org/wiki/FmPresets

local MRL_base = "v4l2c:///dev/radio0:tuner-frequency="

function probe()
	if not string.match( vlc.path, "%.[fF][mM][rR]$" ) then return false end
	local line = vlc.peek(256)
	local freq = tonumber(string.match( line, "^[^%d]?[^%d]?[^%d]?[^%d]?(%d+):" )) -- handle BOM
	return freq and freq > 80000000 and freq < 110000000
end

function parse()
	local p = {}
	while true do
		local line = vlc.readline()
		if not line then break end
		local freq, name = string.match( line, "(%d+):(.*)" )
                if freq then
			table.insert( p, { path = MRL_base..freq, name = name } )
		end
	end
	return p
end
