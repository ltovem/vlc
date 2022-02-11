export default {
    template: '#enqueue-button-template',
    props: ['item', 'items'],
    computed: {
        ...Vuex.mapState({ playlist: state => state.playlist.items }),
    },
    data() {
        return {
            isEnqueued: false,
        }
    },
    watch: {
        playlist(playlist = []) {
            this.checkQueue(playlist);
        }
    },
    methods: {
        checkQueue(playlist) {
            if (this.item) {
                this.isEnqueued = !!playlist.find((item = {}) => Number(item.mediaID) === Number(this.item.id));
            }

            if (this.items && this.items.length) {
                const itemsCount = this.items.reduce((acc, curr) => {
                    const playlistItem = playlist.find((pitem = {}) => Number(pitem.mediaID) === Number(curr.id));

                    if (playlistItem) {
                        return acc + 1;
                    }

                    return acc;
                }, 0);

                this.isEnqueued = itemsCount === this.items.length;
            }
        },

        enqueueItem() {
            let items = this.items;

            if (this.item) {
                items = [this.item];
            }

            if (items.length) {
                items.forEach(item => this.enqueue(item));
            }
        },

        enqueue(item) {
            if (item && item.mrl && !this.playlist.find((pitem = {}) => Number(pitem.mediaID) === Number(item.id))) {
                this.$store.dispatch('playlist/addItem', { src: item.mrl, mediaID: item.id });
            }
        },

        remove(item) {
            if (item && item.mrl) {
                const playlistItem = this.playlist.find((pitem = {}) => Number(pitem.mediaID) === Number(item.id));
                this.$store.dispatch('playlist/removeItem', playlistItem.id);
            }
        },

        removeFromQueue() {
            let items = this.items;

            if (this.item) {
                items = [this.item];
            }

            items.forEach(item => this.remove(item));
        }
    },

    mounted() {
        this.checkQueue(this.playlist || []);
    }
};
