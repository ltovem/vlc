Vue.component('play-button-secondary', {
    template: '#play-button-secondary-template',
    props: ['item'],
    methods: {
        playItem() {
            if (this.item) {
                // Check if in playlist else add it
                if (this.item.mrl) {
                    this.$store.dispatch('playlist/addItem', this.item.mrl);
                }
                this.$store.dispatch('status/play', this.item.id);
            }
        }
    }
});
