import sdService from '../../services/sd.service.js';

// initial state
const state = {
    activeItem: {},
    items: [],
};

const actions = {
    fetchSd({ commit }) {
        sdService.getServiceDiscovery()
            .then((services) => {
                commit('setServices', services);
            }).catch(e => reject(e));
    },
};

const mutations = {
    setServices(state, services) {
        return new Promise((resolve, reject) => {
            newServices = services.map((service, index) => ({
                ...service,
                nb: index,
                active: state.activeItem && state.activeItem.id === service.id
            }));

            state.items = newServices;
            resolve(newServices);
        });
    },
};

export default {
    namespaced: true,
    state,
    actions,
    mutations
};
