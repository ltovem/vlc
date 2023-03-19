--[[
 Gets an artwork from the Cover Art Archive or Amazon

 Copyright © 2007-2010 the VideoLAN team

 SPDX-License-Identifier: GPL-2.0-or-later
--]]

function descriptor()
    return { scope="network" }
end

function try_query(mbid)
    local relquery = "http://mb.videolan.org/ws/2/release/" .. mbid
    local s = vlc.stream( relquery )
    if not s then return nil end
    local page = s:read( 65653 )

    found, _ = string.find( page, "<artwork>true</artwork>" )
    if found then
        front, _ = string.find( page, "<front>true</front>" )
        if front then
            return "http://coverartarchive.org/release/"..mbid.."/front-500"
        end
    end

    -- FIXME: multiple results may be available
    _, _, asin = string.find( page, "<asin>(%w+)</asin>" )
    if asin then
        return "http://images.amazon.com/images/P/"..asin..".01._SCLZZZZZZZ_.jpg"
    end
    vlc.msg.dbg("Neither coverartarchive.org nor amazon have cover art for this release")
    return nil
end

-- Return the mbid for the first release returned by the MusicBrainz search server for query
function get_releaseid(query)
    local s = vlc.stream( query )
    if not s then return nil end
    local page = s:read( 65653 )

    -- FIXME: multiple results may be available and the first one is not
    -- guaranteed to have asin, so if it doesnt, we wouldnt get any art
    _, _, releaseid = string.find( page, "<release id=\"([%x%-]-)\"" )
    if releaseid then
        return releaseid
    end
    return nil
end

-- Return the artwork
function fetch_art()
    local meta = vlc.item:metas()

    if meta["Listing Type"] == "radio"
    or meta["Listing Type"] == "tv"
    then return nil end

    local releaseid = nil

    if meta["MB_ALBUMID"] then
        releaseid = meta["MB_ALBUMID"]
    end

    if not releaseid and meta["artist"] and meta["album"] then
        query = "artist:\"" .. meta["artist"] .. "\" AND release:\"" .. meta["album"] .. "\""
        relquery = "http://mb.videolan.org/ws/2/release/?query=" .. vlc.strings.encode_uri_component( query )
        releaseid = get_releaseid( relquery )
    end
    if not releaseid and meta["artist"] and meta["title"] then
        query = "artist:\"" .. meta["artist"] .. "\" AND recording:\"" .. meta["title"] .. "\""
        recquery = "http://mb.videolan.org/ws/2/recording/?query=" .. vlc.strings.encode_uri_component( query )
        releaseid = get_releaseid( recquery )
    end
    if releaseid then
        return try_query( releaseid )
    else
        return nil
    end
end
