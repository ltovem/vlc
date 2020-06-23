Vue.component('videos', {
    template: '#videos-template',
    data: function() {
        return {
            displayMode: 'grid'
        }
    },
    computed: {
        ...Vuex.mapState({
            videos: state => state.video.videos,
            layout: state => state.layout
        }),
    },
    methods: {
        fetchVideos() {
            this.$store.dispatch('video/fetchVideos');
        }
    },
    created() {
        this.fetchVideos();
    }
});
