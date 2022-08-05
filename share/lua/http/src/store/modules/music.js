import musicService from '../../services/music.service.js';

// initial state
const state = {
    tracks: [],
    albums: [],
    artists: [],
    genres: [],
    albumTracks: {},
    artistAlbums: {}
};

const getters = {

};

const actions = {
    fetchTracks({ commit }) {
        musicService.fetchTracks()
            .then((tracks) => {
                commit('updateTracks', tracks);
            });
    },
    fetchAlbums({ commit }) {
        musicService.fetchAlbums()
            .then((albums) => {
                commit('updateAlbums', albums);
            });
    },
    fetchAlbumTracks({ commit }, id) {
        musicService.fetchAlbumTracks({ albumId: id })
            .then((tracks) => {
                commit('updateAlbumTracks', {
                    id,
                    tracks
                });
            });
    },
    fetchArtists({ commit }) {
        musicService.fetchArtists()
            .then((artists) => {
                commit('updateArtists', artists);
            });
    },
    fetchArtistAlbums({ commit }, id) {
        musicService.fetchArtistAlbums({
                artistId: id
            })
            .then((albums) => {
                commit('updateArtistAlbums', {
                    albums,
                    id
                });
            });
    },
    fetchGenres({ commit }) {
        musicService.fetchGenres()
            .then((genres) => {
                commit('updateGenres', genres);
            });
    }
};

const mutations = {
    updateTracks(state, data) {
        state.tracks = data;
    },
    updateAlbums(state, data) {
        state.albums = data;
    },
    updateAlbumTracks(state, data) {
        const newTracks = {
            [data.id]: data.tracks
        }
        state.albumTracks = Object.assign({}, state.albumTracks, newTracks);
    },
    updateArtists(state, data) {
        state.artists = data;
    },
    updateArtistAlbums(state, data) {
        const newAlbums = {
            [data.id]: data.albums
        };
        state.artistAlbums = Object.assign({}, state.artistAlbums, newAlbums);
    },
    updateGenres(state, data) {
        state.genres = data;
    }
};

export default {
    namespaced: true,
    state,
    getters,
    actions,
    mutations
};
