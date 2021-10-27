Vue.component('track-item-table', {
    template: '#track-item-table-template',
    props: ['item', 'index'],
    data: function() {
        return {
            liked: this.item.liked,
            showMore: false,
            showMoreStyle: {
                position: "absolute",
                top: '125px',
                left: 0
            }
        }
    },
    computed: { },
    methods: {
        toggleShowMore() {
            this.showMore = !this.showMore
        },
        toggleLiked() {
            this.liked = !this.liked;
        }
    },
    created() { }
});
