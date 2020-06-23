Vue.component('item-layout-button', {
    template: '#item-layout-button-template',
    computed: {
        ...Vuex.mapState({
            layout: state => state.layout
        }),
    },
    created() {
        document.body.style['overflow-y'] = 'auto';
    },
    methods: {
        toggleItemLayout() {
            this.$store.dispatch('layout/toggleItemLayout');
        }
    }
});
