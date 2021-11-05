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
            isPlaying: false,
            classname,
        }
    },
    methods: {
        async play() {
            if (this.item) {
                let currItem = this.item;

                if (!currItem.mediaID) {
                    currItem = this.playlist.find((item = {}) => Number(item.mediaID) === Number(this.item.id));
                }

                if (!currItem) {
                    currItem = await this.$store.dispatch('playlist/addAndGetItem', { src: this.item.mrl, mediaID: this.item.id })
                }

                const path = '/watch';

                if (currItem.type === 'audio') {
                    if (this.$route.path === path) {
                        this.$router.push('/');
                    }

                    this.$store.dispatch('layout/openAudioPlayer');
                }

                if (currItem.type === 'leaf' || currItem.type === 'video') {
                    if (this.$route.path !== path) {
                        this.$router.push(path);
                    }

                    this.$store.dispatch('layout/closeAudioPlayer');
                }

                this.$store.dispatch('status/play', currItem.id)
                this.$store.dispatch('playlist/setActiveItem', currItem);
            }
        }
    }
});
