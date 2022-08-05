export default {
    template: '#video-item-grid-template',
    props: ['item'],
    methods: {
        toggleLiked() {
            this.liked = !this.liked;
        }
    }
};
