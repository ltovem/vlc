Vue.component('player-button', {
    template: '#player-button-template',
    props: ['item', 'type', 'items'],
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
        enqueueItem(item) {
            if (item && item.mrl && !this.playlist.find(({ mediaID }) => Number(mediaID) === Number(item.id))) {
                this.$store.dispatch('playlist/addItem', { src: item.mrl, mediaID: item.id });
            }
        },

        playItem(item) {
            return new Promise(async resolve => {
                let currItem = item;

                if (!currItem.mediaID) {
                    currItem = this.playlist.find((pitem = {}) => Number(pitem.mediaID) === Number(item.id));
                }

                if (!currItem) {
                    currItem = await this.$store.dispatch('playlist/addAndGetItem', { src: item.mrl, mediaID: item.id })
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

                resolve();
            });
        },

        async play() {
            if (this.item) {
                this.playItem(this.item);
            }

            if (this.items && this.items.length) {
                await this.playItem(this.items[0]);

                this.items.forEach((item, index) => index && this.enqueueItem(item));
            }
        }
    }
});
