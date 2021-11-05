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
            this.playlistItem = playlist.find((item = {}) => Number(item.mediaID) === Number(this.item.id))
        }
    },
    methods: {
        enqueueItem() {
            if (this.item && this.item.mrl) {
                this.$store.dispatch('playlist/addItem', { src: this.item.mrl, mediaID: this.item.id });
            }
        },
        removeFromQueue() {
            if (this.item && this.item.mrl) {
                this.$store.dispatch('playlist/removeItem', this.playlistItem.id);
            }
        }
    }
});
