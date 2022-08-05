export default {
    template: '#albums-template',
    props: ['artistId'],
    watch: {
        artistId: function(newVal, oldVal) {
            if (newVal !== oldVal) {
                this.fetchArtistAlbums(newVal);
            }
        }
    },
    data: function() {
        return {
            displayMode: 'grid'
        }
    },
    computed: {
        ...Vuex.mapState({
            music: state => state.music,
            layout: state => state.layout
        }),
    },
    methods: {
        fetchAlbums() {
            this.$store.dispatch('music/fetchAlbums');
        },
        fetchArtistAlbums(artistId) {
            this.$store.dispatch('music/fetchArtistAlbums', artistId);
        },
        getAlbums() {
            return !this.$route.params.id ? this.music.albums : this.music.artistAlbums[this.$route.params.id];
        }
    },
    created() {
        const artistId = this.$route.params.id;
        if (artistId) {
            this.fetchArtistAlbums(artistId);
        } else {
            this.fetchAlbums();
        }
    }
};
