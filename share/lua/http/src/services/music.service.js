export default {
    fetchTracks(params = {}) {
        return $.ajax({
            url: 'medialib/audio',
            data: params
        })
        .then((data) => {
            let jsonData = JSON.parse(data);
            const origin = location.origin;
            jsonData = jsonData.map((d) => {
                d.src = d.hasThumbnail ? origin + '/medialib/thumbnail?mediaId=' + d.id : '';
                return d;
            });
            return jsonData;
        });
    },
    fetchAlbums(params = {}) {
        return $.ajax({
            url: 'medialib/albums',
            data: params
        })
        .then((data) => {
            let jsonData = JSON.parse(data);
            const origin = location.origin;
            jsonData = jsonData.map((d) => {
                d.src = d.hasThumbnail ? origin + '/medialib/album/thumbnail?albumId=' + d.id : '';
                return d;
            });
            return jsonData;
        });
    },
    fetchAlbumTracks(params = {}) {
        return $.ajax({
            url: 'medialib/album/tracks',
            data: params
        })
        .then((data) => {
            let jsonData = JSON.parse(data);
            return jsonData;
        });
    },
    fetchArtists(params = {}) {
        return $.ajax({
            url: 'medialib/artists',
            data: params
        })
        .then((data) => {
            let jsonData = JSON.parse(data);
            const origin = location.origin;
            jsonData = jsonData.map((d) => {
                d.src = d.hasThumbnail ? origin + '/medialib/artist/thumbnail?artistId=' + d.id : '';
                return d;
            });
            return jsonData;
        });
    },
    fetchArtistAlbums(params = {}) {
        return $.ajax({
            url: 'medialib/artist/albums',
            data: params
        })
        .then((data) => {
            let jsonData = JSON.parse(data);
            return jsonData;
        });
    },
    fetchGenres(params = {}) {
        return $.ajax({
            url: 'medialib/genres',
            data: params
        })
        .then((data) => {
            let jsonData = JSON.parse(data);
            return jsonData;
        });
    }
};
