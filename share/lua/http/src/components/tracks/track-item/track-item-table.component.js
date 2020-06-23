Vue.component('track-item-table', {
    template: '#track-item-table-template',
    props: ['item', 'index'],
    data: function() {
        return {
            liked: this.item.liked
        }
    },
    computed: { },
    methods: {
        toggleLiked() {
            this.liked = !this.liked;
        }
    },
    created() { }
});
