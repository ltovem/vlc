export default {
    template: '#album-item-grid-template',
    props: ['item'],
    data: function() {
        return {
            liked: this.item.liked
        }
    },
    computed: {
        ...Vuex.mapState({
            music: state => state.music
        }),
    },
    methods: {
        toggleAlbumTracks(id) {
            this.$store.dispatch('music/fetchAlbumTracks', id);
        }
    },
    created() { }
};
