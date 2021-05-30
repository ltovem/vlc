Vue.component('item-layout-button', {
    template: '#item-layout-button-template',
    props: ['disabled'],
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
            if (this.disabled) {
                return true;
            }
            this.$store.dispatch('layout/toggleItemLayout');
        }
    }
});
