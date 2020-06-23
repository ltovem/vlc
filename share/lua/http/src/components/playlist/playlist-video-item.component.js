Vue.component('playlist-video-item', {
    template: '#playlist-video-item-template',
    props: ['item', 'index'],
    computed: {
        ...Vuex.mapState({
            playlist: state => state.playlist
        }),
    },
    methods: {
        addItem(mode, id, title, src) {
            this.$store.dispatch('playlist/addItem', src);
        },
        play(src, id) {
            this.$store.dispatch('status/play', id);
        },
        onImgError(item) {
            item.src = '';
        },
        setActiveItem(item) {
            const path = '/watch'
            if (this.$route.path !== path) {
                this.$router.push(path);
            }
            this.$store.dispatch('layout/closeAudioPlayer');
            this.$store.dispatch('playlist/setActiveItem', item);
        }
    },
    created() {

    }
});
