Vue.component('album-item-list', {
    template: '#album-item-list-template',
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
});
