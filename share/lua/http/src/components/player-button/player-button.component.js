Vue.component('player-button', {
    template: '#player-button-template',
    props: ['item', 'type'],
    computed: {
        ...Vuex.mapState({ playlist: state => state.playlist.items }),
    },
    data() {
        const classname = {
            'btn-link': this.type === 'primary',
            'play-button':  this.type === 'primary',
            'btn-sm': this.type === 'secondary',
            'play-button-secondary': this.type === 'secondary',
        };

        return {
            isAlreadyAdded: true,
            isPlaying: false,
            classname,
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
        play() {
            if (this.item) {
                this.mustPlay = true;

                if (this.item.mrl && !this.isAlreadyAdded) {
                    this.$store.dispatch('playlist/addItem', this.item.mrl);
                }
            }
        }
    }
});
