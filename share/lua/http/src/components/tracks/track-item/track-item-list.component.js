Vue.component('track-item-list', {
    template: '#track-item-list-template',
    props: ['item', 'index'],
    methods: {
        toggleLiked() {
            this.liked = !this.liked;
        }
    }
});
