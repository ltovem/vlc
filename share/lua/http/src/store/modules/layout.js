// initial state
const state = {
    audioPlayerOpened: false,
    itemLayout: 'grid'
};

const getters = { };

const actions = {
    toggleItemLayout({ commit }) {
        let displayValue = 'grid';
        if (state.itemLayout === 'grid') {
            displayValue = 'list'
        }
        commit('setItemLayout', displayValue);
    },
    openAudioPlayer({ commit }) {
        commit('setAudioPlayerOpened', true);
    },
    closeAudioPlayer({ commit }) {
        commit('setAudioPlayerOpened', false);
    }
};

const mutations = {
    setAudioPlayerOpened(state, value) {
        state.audioPlayerOpened = value;
    },
    setItemLayout(state, value) {
        state.itemLayout = value;
    }
};

export default {
    namespaced: true,
    state,
    getters,
    actions,
    mutations
};
