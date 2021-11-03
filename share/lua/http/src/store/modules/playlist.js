import playlistService from '../../services/playlist.service.js';

// initial state
const state = {
    activeItem: {},
    items: [],
};

const getters = {
    getCurrentItem: (state, getters, rootState) => {
        const currentItem = state.items.filter((state) => {
            return state.current === 'current';
        });
        return currentItem.length ? currentItem[0] : null;
    }
};

const actions = {
    setActiveItem({ commit }, item) {
        // Some set actuve request
        commit('setActiveItem', item);
    },
    fetchPlaylist({ commit }) {
        playlistService.fetchPlaylist()
            .then((playlist) => {
                commit('setPlaylist', playlist);
            }).catch(e => reject(e));
    },
    addItem({ commit, dispatch }, src) {
        playlistService.addItem(src)
            .then(() => {
                // Refresh playlist
                dispatch('fetchPlaylist');
            }).catch(e => reject(e));
    },
    removeItem({ commit }, id) {
        playlistService.removeItem(id)
            .then(() => {
                commit('removeItem', id);
            });
    },
    async addAndGetItem({ commit, dispatch }, src) {
        try {
            await playlistService.addItem(src)

            const fetchedPlaylist = await playlistService.fetchPlaylist()
            await commit('setPlaylist', fetchedPlaylist);

            const currItem = state.items.find((item = {}) => decodeURI(item.uri || item.mrl) === decodeURI(src))

            return currItem;
        } catch (e) {
            return e;
        }
    },
};

const mutations = {
    setActiveItem(state, item) {
        state.activeItem = item;
        state.items = state.items.map((s) => {
            s.active = item && item.id === s.id;
            return s;
        });
    },
    setPlaylist(state, playlist) {
        return new Promise((resolve, reject) => {
            const origin = location.origin;
            playlist = playlist.map((pl, index) => {
                pl.nb = index;
                // @TODO: uncomment when working in the backend side
                pl.src = '' // origin + '?item=' + pl.id;
                pl.active = state.activeItem && state.activeItem.id === pl.id;
                return pl;
            });
            state.items = playlist;
            resolve(playlist);
        });
    },
    removeItem(state, id) {
        return state.items = state.items.filter((item) => {
            return item.id !== id;
        });
    }
};

export default {
    namespaced: true,
    state,
    getters,
    actions,
    mutations
};
