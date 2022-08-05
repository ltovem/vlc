import videoService from '../../services/video.service.js';

// initial state
const state = {
    videos: [],
    movies: [],
    tvShows: [],
};

const getters = {

};

const actions = {
    fetchVideos({ commit }) {
        videoService.fetchVideos()
            .then((tracks) => {
                commit('updateVideos', tracks);
            });
    },
    fetchMovies({ commit }) {
        videoService.fetchMovies()
            .then((tracks) => {
                commit('updateMovies', tracks);
            });
    },
    fetchTvShows({ commit }) {
        videoService.fetchTvShows()
            .then((tracks) => {
                commit('updateTvShows', tracks);
            });
    }
};

const mutations = {
    updateVideos(state, data) {
        state.videos = data;
    },
    updateMovies(state, data) {
        state.movies = data;
    },
    updateTvShows(state, data) {
        state.tvShows = data;
    }
};

export default {
    namespaced: true,
    state,
    getters,
    actions,
    mutations
};
