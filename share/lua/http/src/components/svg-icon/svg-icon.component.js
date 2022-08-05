const svgIconCache = new Map();
export default {
    template: '#svg-icon-template',
    props: ['name'],
    watch: {
        name: function(oldVal, newVal) {
            if (oldVal !== newVal) {
                this.setIcon();
            }
        }
    },
    methods: {
        async setIcon() {
            const url = location.origin + '/dist/assets/svg-icons/' + this.name + '.svg';
            if (!svgIconCache.has(url)) {
                try {
                    svgIconCache.set(url, $.ajax({url}));
                } catch (e) {
                    svgIconCache.delete(this.src);
                }
            }
            if (svgIconCache.has(url)) {
                this.$el.innerHTML = await svgIconCache.get(url);
            }
        }
    },
    mounted() {
        this.setIcon();
    }
};
