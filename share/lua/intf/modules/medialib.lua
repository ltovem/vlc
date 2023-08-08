--[==========================================================================[
 medialib.lua: Exposes the media library to lua interfaces
--[==========================================================================[
 Copyright (C) 2020 the VideoLAN team

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
--]==========================================================================]

local medialib = {}
local dkjson = require ("dkjson")

local function wrap_result(code, content, mime)
    -- Automatically encode the content in json when the request didn't fail
    -- and when no mime is provided
    if code >= 200 and code < 300 and not mime then
        content = dkjson.encode (content, { indent = true })
    end
    if not mime then
        mime = "text/plain"
    end
    local res= "Status: " .. tostring(code) .. "\r\n" ..
    "Content-Type: " .. mime .. "\r\n" ..
    "Content-Length: " .. #content ..
    "\r\n\r\n" .. content
--    print(res)
    return res
end

local function list_video(get)
    local v = vlc.ml.video(get)
    return wrap_result(200, v)
end

local function list_show_episode(get)
    local a = vlc.ml.show_episodes(get)
    return wrap_result(200, a)
end

local function list_audio(get)
    local a = vlc.ml.audio(get)
    return wrap_result(200, a)
end

local function list_albums(get)
    local a = vlc.ml.albums(get)
    return wrap_result(200, a)
end

local function get_album(get)
    local albumId = get['albumId']
    if not albumId then
        return wrap_result(400, "Missing albumId" )
    end
    local a = vlc.ml.album(albumId)
    return wrap_result(200, a)
end

local function get_album_tracks(get)
    local albumId = get['albumId']
    if not albumId then
        return wrap_result(400, "Missing albumId" )
    end
    local tracks = vlc.ml.album_tracks(albumId, get)
    return wrap_result(200, tracks)
end

local function list_artists(get)
    local a = vlc.ml.artists(get)
    return wrap_result(200, a)
end

local function get_artist(get)
    local artistId = get['artistId']
    if not artistId then
        return wrap_result(400, "Missing artistId" )
    end
    local a = vlc.ml.artist(artistId)
    return wrap_result(200, a)
end

local function get_artist_albums(get)
    local artistId = get['artistId']
    if not artistId then
        return wrap_result(400, "Missing artistId" )
    end
    local albums = vlc.ml.artist_albums(artistId, get)
    return wrap_result(200, albums)
end

local function list_genres(get)
    local a = vlc.ml.genres(get)
    return wrap_result(200, a)
end

local function get_genre(get)
    local genreId = get['genreId']
    if not genreId then
        return wrap_result(400, "Missing genreId" )
    end
    local a = vlc.ml.genre(genreId)
    return wrap_result(200, a)
end

local function get_genre_albums(get)
    local genreId = get['genreId']
    if not genreId then
        return wrap_result(400, "Missing genreId" )
    end
    local albums = vlc.ml.genre_albums(genreId, get)
    return wrap_result(200, albums)
end

local function get_genre_artists(get)
    local genreId = get['genreId']
    if not genreId then
        return wrap_result(400, "Missing genreId" )
    end
    local artists = vlc.ml.genre_artists(genreId, get)
    return wrap_result(200, artists)
end

local function wrap_thumbnail(thumbMrl)
    if not thumbMrl then
        return wrap_result(404, "Thumbnail not found")
    end
    filename = vlc.strings.make_path(thumbMrl)
    local windowsdrive = string.match(filename, "^/%a:/.+$")  --match windows drive letter
    if windowsdrive then
        filename = string.sub(filename, 2)  --remove starting forward slash before the drive letter
    end
    local size = vlc.net.stat(filename).size
    local ext = string.match(filename,"%.([^%.]-)$")
    local raw = io.open(filename, 'rb'):read("*a")
    return wrap_result(200, raw, "image/jpeg")
end

local function get_thumbnail(get)
    local mediaId = get['mediaId']
    if not mediaId then
        return wrap_result(400, "Missing mediaId")
    end
    local thumbMrl = vlc.ml.media_thumbnail(mediaId)
    return wrap_thumbnail(thumbMrl)
end

local function get_artist_thumbnail(get)
    local artistId = get['artistId']
    if not artistId then
        return wrap_result(400, "Missing artistId")
    end
    local thumbMrl = vlc.ml.artist_thumbnail(artistId)
    return wrap_thumbnail(thumbMrl)
end

local function get_album_thumbnail(get)
    local albumId = get['albumId']
    if not albumId then
        return wrap_result(400, "Missing albumId")
    end
    local thumbMrl = vlc.ml.album_thumbnail(albumId)
    return wrap_thumbnail(thumbMrl)
end

initialReload = false

local function wrap_handler(func)
    return function(data, url, request)
        if not initialReload then
            vlc.ml.reload()
            initialReload = true
        end
        local get = parse_url_request(request)
        return func(get)
    end
end

function medialib.setup_handlers(h, password)
    handlers = {
        h:handler("/medialib/video", nil, password, wrap_handler(list_video), nil),
        h:handler("/medialib/audio", nil, password, wrap_handler(list_audio), nil),
        h:handler("/medialib/tvshows", nil, password, wrap_handler(list_show_episode), nil),
        h:handler("/medialib/thumbnail", nil, password, wrap_handler(get_thumbnail), nil),
        h:handler("/medialib/albums", nil, password, wrap_handler(list_albums), nil),
        h:handler("/medialib/album", nil, password, wrap_handler(get_album), nil),
        h:handler("/medialib/album/thumbnail", nil, password, wrap_handler(get_album_thumbnail), nil),
        h:handler("/medialib/album/tracks", nil, password, wrap_handler(get_album_tracks), nil),
        h:handler("/medialib/artists", nil, password, wrap_handler(list_artists), nil),
        h:handler("/medialib/artist", nil, password, wrap_handler(get_artist), nil),
        h:handler("/medialib/artist/albums", nil, password, wrap_handler(get_artist_albums), nil),
        h:handler("/medialib/artist/thumbnail", nil, password, wrap_handler(get_artist_thumbnail), nil),
        h:handler("/medialib/genres", nil, password, wrap_handler(list_genres), nil),
        h:handler("/medialib/genre", nil, password, wrap_handler(get_genre), nil),
        h:handler("/medialib/genre/albums", nil, password, wrap_handler(get_genre_albums), nil),
        h:handler("/medialib/genre/artists", nil, password, wrap_handler(get_genre_artists), nil),
    }
    -- Return the handlers so they don't immediatly get garbage collected
    return handlers
end

return medialib
