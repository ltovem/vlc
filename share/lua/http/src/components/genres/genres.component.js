Vue.component('genres', {
    template: '#genres-template',
    data: function() {
        return {
            displayMode: 'grid',
            items: [

            ]
        }
    },
    computed: {
        ...Vuex.mapState({
            music: state => state.music,
            layout: state => state.layout
        }),
    },
    methods: {
        fetchGenres() {
            this.$store.dispatch('music/fetchGenres');
        }
    },
    created() {
        this.fetchGenres();
    }
});
