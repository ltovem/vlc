Vue.component('playlist-audio-item', {
    template: '#playlist-audio-item-template',
    props: ['item', 'index'],
    methods: {
        addItem(mode, id, title, src) {
            this.$store.dispatch('playlist/addItem', { mediaID: id, src });
        },
        play(src, id) {
            this.$store.dispatch('layout/openAudioPlayer');
            this.$store.dispatch('status/play', id);
        },
        onImgError(item) {
            item.src = '';
        },
        setActiveItem(item) {
            this.$store.dispatch('layout/openAudioPlayer');
            this.$store.dispatch('playlist/setActiveItem', item);
        }
    },
    created() {

    }
});
