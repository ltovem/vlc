--[[
 Gets an artwork for french TV channels

 Copyright © 2007 the VideoLAN team

 SPDX-License-Identifier: GPL-2.0-or-later
--]]

function descriptor()
    return { scope="local" }
end

function trim (s)
  return (string.gsub(s, "^%s*(.-)%s*$", "%1"))
end

function read_meta()
    local metas = vlc.item:metas()

    -- Don't do anything if there is already a title
    if metas["title"] then
        return
    end

    local name = metas["filename"];
    if not name then
        return
    end

    -- Find "Show.Name.S01E12-blah.avi"
    local title, seasonNumber
    _, _, showName, seasonNumber, episodeNumber = string.find(name, "(.+)S(%d+)E(%d+).*")
    if not showName then
        return
    end

    -- Remove . in showName
    showName = trim(string.gsub(showName, "%.", " "))
    vlc.item:set_meta("title", showName.." S"..seasonNumber.."E"..episodeNumber)
    vlc.item:set_meta("showName", showName)
    vlc.item:set_meta("episodeNumber", episodeNumber)
    vlc.item:set_meta("seasonNumber", seasonNumber)
end
