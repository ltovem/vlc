Vue.component('play-button-secondary', {
    template: '#play-button-secondary-template',
    props: ['item'],
    computed: {
        ...Vuex.mapState({ playlist: state => state.playlist.items }),
    },
    data() {
        return {
            isAlreadyAdded: true,
            isPlaying: false,
        }
    },
    watch: {
        playlist(playlist = []) {
            if (this.item) {
                const currPlaylistItem = this.item && playlist.find((item = {}) => decodeURI(item.uri) === decodeURI(this.item.mrl || this.item.uri))

                this.isAlreadyAdded = !!currPlaylistItem

                if (this.mustPlay) {
                    this.$store.dispatch('status/play', currPlaylistItem.id);
                    this.mustPlay = false;
                }
            }
        }
    },
    methods: {
        playItem() {
            if (this.item) {
                this.mustPlay = true;

                if (this.item.mrl && !this.isAlreadyAdded) {
                    this.$store.dispatch('playlist/addItem', this.item.mrl);
                }
            }
        }
    }
});
