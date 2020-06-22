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

module("medialib",package.seeall)
local dkjson = require ("dkjson")

function wrapResult(code, content, mime)
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

listVideo = function(get)
    local v = vlc.ml.video(get['limit'], get['offset'], get['desc'])
    return wrapResult(200, v)
end

listAudio = function(get)
    local a = vlc.ml.audio(get['limit'], get['offset'], get['desc'])
    return wrapResult(200, a)
end

listAlbums = function(get)
    local a = vlc.ml.albums(get['limit'], get['offset'], get['desc'])
    return wrapResult(200, a)
end

getAlbum = function(get)
    local albumId = get['albumId']
    if not albumId then
        return wrapResult(400, "Missing albumId" )
    end
    local a = vlc.ml.album(albumId)
    return wrapResult(200, a)
end

getAlbumTracks = function(get)
    local albumId = get['albumId']
    if not albumId then
        return wrapResult(400, "Missing albumId" )
    end
    local tracks = vlc.ml.album_tracks(get['limit'], get['offset'], get['desc'], albumId)
    return wrapResult(200, tracks)
end

listArtists = function(get)
    local a = vlc.ml.artists(get['limit'], get['offset'], get['desc'])
    return wrapResult(200, a)
end

getArtist = function(get)
    local artistId = get['artistId']
    if not artistId then
        return wrapResult(400, "Missing artistId" )
    end
    local a = vlc.ml.artist(artistId)
    return wrapResult(200, a)
end

getArtistAlbums = function(get)
    local artistId = get['artistId']
    if not artistId then
        return wrapResult(400, "Missing artistId" )
    end
    local albums = vlc.ml.artist_albums(get['limit'], get['offset'], get['desc'], artistId)
    return wrapResult(200, albums)
end

listGenres = function(get)
    local a = vlc.ml.genres(get['limit'], get['offset'], get['desc'])
    return wrapResult(200, a)
end

getGenre = function(get)
    local genreId = get['genreId']
    if not genreId then
        return wrapResult(400, "Missing genreId" )
    end
    local a = vlc.ml.genre(genreId)
    return wrapResult(200, a)
end

getGenreAlbums = function(get)
    local genreId = get['genreId']
    if not genreId then
        return wrapResult(400, "Missing genreId" )
    end
    local albums = vlc.ml.genre_albums(get['limit'], get['offset'], get['desc'], genreId)
    return wrapResult(200, albums)
end

getGenreArtists = function(get)
    local genreId = get['genreId']
    if not genreId then
        return wrapResult(400, "Missing genreId" )
    end
    local artists = vlc.ml.genre_artists(get['limit'], get['offset'], get['desc'], genreId)
    return wrapResult(200, artists)
end

wrapThumbnail = function(thumbMrl)
    if not thumbMrl then
        return wrapResult(404, "Thumbnail not found")
    end
    filename = vlc.strings.make_path(thumbMrl)
    local windowsdrive = string.match(filename, "^/%a:/.+$")  --match windows drive letter
    if windowsdrive then
        filename = string.sub(filename, 2)  --remove starting forward slash before the drive letter
    end
    local size = vlc.net.stat(filename).size
    local ext = string.match(filename,"%.([^%.]-)$")
    local raw = io.open(filename, 'rb'):read("*a")
    return wrapResult(200, raw, "image/jpeg")
end

getThumbnail = function(get)
    local mediaId = get['mediaId']
    if not mediaId then
        return wrapResult(400, "Missing mediaId")
    end
    local thumbMrl = vlc.ml.media_thumbnail(mediaId)
    return wrapThumbnail(thumbMrl)
end

getArtistThumbnail = function(get)
    local artistId = get['artistId']
    if not artistId then
        return wrapResult(400, "Missing artistId")
    end
    local thumbMrl = vlc.ml.artist_thumbnail(artistId)
    return wrapThumbnail(thumbMrl)
end

getAlbumThumbnail = function(get)
    local albumId = get['albumId']
    if not albumId then
        return wrapResult(400, "Missing albumId")
    end
    local thumbMrl = vlc.ml.album_thumbnail(albumId)
    return wrapThumbnail(thumbMrl)
end

initialReload = false

function wrapHandler(func)
    return function(data, url, request)
        if not initialReload then
            vlc.ml.reload()
            initialReload = true
        end
        local get = parse_url_request(request)
        return func(get)
    end
end

setupHandlers = function(h, password)
    handlers = {
        h:handler("/medialib/video", nil, password, wrapHandler(listVideo), nil),
        h:handler("/medialib/audio", nil, password, wrapHandler(listAudio), nil),
        h:handler("/medialib/thumbnail", nil, password, wrapHandler(getThumbnail), nil),
        h:handler("/medialib/albums", nil, password, wrapHandler(listAlbums), nil),
        h:handler("/medialib/album", nil, password, wrapHandler(getAlbum), nil),
        h:handler("/medialib/album/thumbnail", nil, password, wrapHandler(getAlbumThumbnail), nil),
        h:handler("/medialib/album/tracks", nil, password, wrapHandler(getAlbumTracks), nil),
        h:handler("/medialib/artists", nil, password, wrapHandler(listArtists), nil),
        h:handler("/medialib/artist", nil, password, wrapHandler(getArtist), nil),
        h:handler("/medialib/artist/albums", nil, password, wrapHandler(getArtistAlbums), nil),
        h:handler("/medialib/artist/thumbnail", nil, password, wrapHandler(getArtistThumbnail), nil),
        h:handler("/medialib/genres", nil, password, wrapHandler(listGenres), nil),
        h:handler("/medialib/genre", nil, password, wrapHandler(getGenre), nil),
        h:handler("/medialib/genre/albums", nil, password, wrapHandler(getGenreAlbums), nil),
        h:handler("/medialib/genre/artists", nil, password, wrapHandler(getGenreArtists), nil),
    }
    -- Return the handlers so they don't immediatly get garbage collected
    return handlers
end
