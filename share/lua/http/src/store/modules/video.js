import videoService from '../../services/video.service.js';

// initial state
const state = {
    videos: [],
};

const getters = {

};

const actions = {
    fetchVideos({ commit }) {
        videoService.fetchVideos()
            .then((tracks) => {
                commit('updateVideos', tracks);
            });
    }
};

const mutations = {
    updateVideos(state, data) {
        state.videos = data;
    }
};

export default {
    namespaced: true,
    state,
    getters,
    actions,
    mutations
};
