--[[

 Copyright © 2010 VideoLAN and AUTHORS

 Authors: Ilkka Ollakka <ileoo at videolan dot org >

 SPDX-License-Identifier: GPL-2.0-or-later
--]]

function descriptor()
    return { title="Katsomo.fi"}
end

function find( haystack, needle )
    local _,_,r = string.find( haystack, needle )
    return r
end

function main()
    vlc.sd.add_item( {title="Uutiset ja fakta", path="http://www.katsomo.fi/?treeId=1"} )
    vlc.sd.add_item( {title="Urheilu", path="http://www.katsomo.fi/?treeId=2000021"} )
    vlc.sd.add_item( {title="Viihde ja sarjat", path="http://www.katsomo.fi/?treeId=3"} )
    vlc.sd.add_item( {title="Lifestyle", path="http://www.katsomo.fi/?treeId=8"} )
end
