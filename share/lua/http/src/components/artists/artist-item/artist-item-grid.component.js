export default {
    template: '#artist-item-grid-template',
    props: ['item'],
    data: function() {
        return {
            liked: this.item.liked
        }
    },
    computed: { },
    methods: { },
    created() { }
};
