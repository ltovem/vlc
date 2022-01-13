import videoService from '../../services/video.service.js';

// initial state
const state = {
    videos: [],
    movies: [],
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
    }
};

const mutations = {
    updateVideos(state, data) {
        state.videos = data;
    },
    updateMovies(state, data) {
        state.movies = data;
    }
};

export default {
    namespaced: true,
    state,
    getters,
    actions,
    mutations
};
