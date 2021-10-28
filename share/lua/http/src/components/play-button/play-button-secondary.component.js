Vue.component('play-button-secondary', {
    template: '#play-button-secondary-template',
    props: ['item'],
    computed: {
        ...Vuex.mapState({ playlist: state => state.playlist.items }),
    },
    data() {
        return {
            isAlreadyAdded: true,
        }
    },
    watch: {
        playlist(playlist = []) {
            if (this.item) {
                this.isAlreadyAdded = !!playlist.find((item = {}) => decodeURI(item.uri) === decodeURI(this.item.mrl))
            }
        }
    },
    methods: {
        playItem() {
            if (this.item) {
                if (this.item.mrl && !this.isAlreadyAdded) {
                    this.$store.dispatch('playlist/addItem', this.item.mrl);
                }
                this.$store.dispatch('status/play', this.item.id);
            }
        }
    }
});
