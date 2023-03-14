--[[

 Copyright © 2009 the VideoLAN team

 Authors: Konstantin Pavlov (thresh@videolan.org)

 SPDX-License-Identifier: GPL-2.0-or-later
--]]

-- Probe function.
function probe()
    local path = vlc.path:gsub("^www%.", "")
    return vlc.access == "http"
        and string.match( path, "^pinkbike%.com/video/%d+" )
end

-- Parse function.
function parse()
    p = {}

	if string.match ( vlc.path, "pinkbike.com/video/%d+" ) then
		while true do
			line = vlc.readline()
			if not line then break end
			-- Try to find video id
			if string.match( line, "video_src.+swf.id=(.*)\"") then
				_,_,videoid = string.find( line, "video_src.+swf.id=(.*)\"")
				catalog = math.floor( tonumber( videoid ) / 10000 )
			end
			-- Try to find the video's title
			if string.match( line, "<title>(.*)</title>" ) then
				_,_,name = string.find (line, "<title>(.*)</title>")
			end
			-- Try to find server which has our video
			if string.match( line, "<link rel=\"videothumbnail\" href=\"http://(.*)/vt/svt-") then
				_,_,server = string.find (line, '<link rel="videothumbnail" href="http://(.*)/vt/svt-' )
			end
			if string.match( line, "<link rel=\"videothumbnail\" href=\"(.*)\" type=\"image/jpeg\"") then
				_,_,arturl = string.find (line, '<link rel="videothumbnail" href="(.*)" type="image/jpeg"')
			end
		end

	end
	table.insert( p, { path = "http://" .. server .. "/vf/" .. catalog .. "/pbvid-" .. videoid .. ".flv"; name = name; arturl = arturl } )
	return p
end
