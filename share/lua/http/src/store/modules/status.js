import statusService from '../../services/status.service.js';

// initial state
const state = {
    data: {}
};

const getters = {};

const actions = {
    fetchStatus({ commit }) {
        statusService.fetchStatus()
            .then((status) => {
                commit('setStatus', status);
            });
    },
    toggleRandom() {
        state.data.random = !state.data.random;
        statusService.toggleRandom();
    },
    toggleRepeat() {
        state.data.repeat = !state.data.repeat;
        statusService.toggleRepeat();
    },
    toggleFullscreen() {
        state.data.fullscreen = !state.data.fullscreen;
        statusService.toggleFullscreen();
    },
    play({ dispatch }, id) {
        statusService.play(id)
            .then(() => {
                dispatch('playlist/fetchPlaylist', {}, {root:true})
            });
    },
    pause({ dispatch }, id) {
        statusService.pause(id);
    },
    stop() {
        statusService.stop();
    },
    updateVolume({}, volume) {
        statusService.updateVolume(volume);
    },
    previous({}) {
        statusService.previous();
    },
    next({}) {
        statusService.next();
    },
    seek({}, value) {
        statusService.seek(value);
    }
};

const mutations = {
    setStatus(state, status) {
        state.data = status;
    }
};

export default {
    namespaced: true,
    state,
    getters,
    actions,
    mutations
};
