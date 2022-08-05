export default {
    template: '#artists-detail-template',
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
        fetchArtists() {
            this.$store.dispatch('music/fetchArtists');
        }
    },
    created() {
        this.fetchArtists();
    }
};
