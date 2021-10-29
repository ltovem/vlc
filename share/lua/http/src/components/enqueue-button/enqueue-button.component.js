Vue.component('enqueue-button', {
    template: '#enqueue-button-template',
    props: ['item'],
    computed: {
        ...Vuex.mapState({ playlist: state => state.playlist.items }),
    },
    data() {
        return {
            playlistItem: null,
        }
    },
    watch: {
        playlist(playlist = []) {
            this.playlistItem = playlist.find((item = {}) => decodeURI(item.uri) === decodeURI(this.item.mrl || this.item.uri))
        }
    },
    methods: {
        enqueueItem() {
            if (this.item) {
                if (this.item.mrl) {
                    this.$store.dispatch('playlist/addItem', this.item.mrl);
                }
            }
        },
        removeFromQueue() {
            if (this.item) {
                if (this.item.mrl) {
                    this.$store.dispatch('playlist/removeItem', this.playlistItem.id);
                }
            }
        }
    }
});
