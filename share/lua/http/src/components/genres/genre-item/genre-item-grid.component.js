export default {
    template: '#genre-item-grid-template',
    props: ['item'],
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
};
