Vue.component('tracks', {
    template: '#tracks-template',
    data: function() {
        return {
            displayMode: 'table'
        }
    },
    computed: {
        ...Vuex.mapState({
            music: state => state.music,
        }),
    },
    methods: {
        fetchTracks() {
            this.$store.dispatch('music/fetchTracks');
        }
    },
    created() {
        this.fetchTracks();
    }
});
