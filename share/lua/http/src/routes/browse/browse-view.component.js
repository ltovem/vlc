export default BrowseViewComponent = {
    template: '#browse-view-template',
    methods: {
        canPlay: function() {
            console.log('canPlay');
        },
        ended: function() {
            console.log('ended');
        }
    },
    computed: {
        ...Vuex.mapState({
            audioPlayerOpened: state => state.layout.audioPlayerOpened
        }),
    },
    created() {
        document.body.style['overflow-y'] = 'auto';
    }
};
