export default DiscoverServicesViewComponent = {
    template: '#discover-services-view-template',
    methods: { },
    created() { },
    data: function() {
        return {
            addService: {
                icon: 'add',
                title: 'Add a service',
                additionalItemClass: 'create-item'
            },
            mainServices: [
                {
                    icon: 'display',
                    title: 'TV',
                    additionalItemClass: 'tv-service',
                    titleRoute: 'services/tv'
                },
                {
                    icon: 'feed',
                    title: 'Radio',
                    additionalItemClass: 'radio-service'
                },
                {
                    icon: 'mic',
                    title: 'Podcast',
                    additionalItemClass: 'podcast-service'
                },
                {
                    icon: 'headphones',
                    title: 'Audio Book',
                    additionalItemClass: 'audiobook-service'
                },
            ],
            secondaryServices: [
                {
                    icon: 'youtube',
                    title: 'Youtube',
                    additionalItemClass: 'youtube-service'
                },
                {
                    icon: 'twitch',
                    title: 'Twitch',
                    additionalItemClass: 'twitch-service'
                }
            ]
        }
    }
};
