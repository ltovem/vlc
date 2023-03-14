--[[

 Copyright © 2010 VideoLAN and AUTHORS

 Authors: Fabio Ritrovato <sephiroth87 at videolan dot org>

 SPDX-License-Identifier: GPL-2.0-or-later
--]]

function descriptor()
    return { title="French TV" }
end

function main()
    node = vlc.sd.add_node( {title="Canal +"} )
    node:add_subitem( {title="Le SAV des émissions ",path="http://www.canalplus.fr/index.php?pid=1782",options={"http-forward-cookies"}} )
    node:add_subitem( {title="Les Guignols",path="http://www.canalplus.fr/index.php?pid=1784",options={"http-forward-cookies"}} )
    node:add_subitem( {title="La Météo de Pauline Lefevre",path="http://www.canalplus.fr/index.php?pid=2834",options={"http-forward-cookies"}} )
end
