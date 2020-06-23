Vue.component('video-item-list', {
    template: '#video-item-list-template',
    props: ['item'],
    methods: {
        toggleLiked() {
            this.liked = !this.liked;
        }
    }
});
