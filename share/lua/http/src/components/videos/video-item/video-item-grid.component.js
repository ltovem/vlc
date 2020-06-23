Vue.component('video-item-grid', {
    template: '#video-item-grid-template',
    props: ['item'],
    methods: {
        toggleLiked() {
            this.liked = !this.liked;
        }
    }
});
