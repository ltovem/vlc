export default {
    template: '#video-item-list-template',
    props: ['item'],
    methods: {
        toggleLiked() {
            this.liked = !this.liked;
        }
    }
};
