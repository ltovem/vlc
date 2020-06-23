Vue.component('enqueue-button', {
    template: '#enqueue-button-template',
    props: ['item'],
    methods: {
        enqueueItem() {
            if (this.item) {
                // Check if in playlist else add it
                if (this.item.mrl) {
                    this.$store.dispatch('playlist/addItem', this.item.mrl);
                }
            }
        }
    }
});
