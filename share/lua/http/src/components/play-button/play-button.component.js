Vue.component('play-button', {
    template: '#play-button-template',
    props: ['item'],
    methods: {
        playItem() {
            if (this.item) {
                // Check if in playlist else add it
                // Don't add if already added
                if (this.item.mrl) {
                    this.$store.dispatch('playlist/addItem', this.item.mrl);
                }
                this.$store.dispatch('status/play', this.item.id);
            }
        }
    }
});
