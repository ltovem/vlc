import capitalizeFirstLetter from '../../utils/strings/index.js';

Vue.component('videos', {
    template: '#videos-template',
    props: ['type'],
    data: function() {
        return {
            displayMode: 'grid'
        }
    },
    computed: {
        ...Vuex.mapState({
            videos(state) {
                return state.video[this.type];
            },
            layout: state => state.layout
        }),
    },
    methods: {
        fetchVideos() {
            this.$store.dispatch(`video/fetch${capitalizeFirstLetter(this.type)}`);
        }
    },
    created() {
        this.fetchVideos();
    }
});
