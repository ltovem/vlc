import BrowseViewComponent from '../routes/browse/browse-view.component.js';
import VideoViewComponent from '../routes/video/video-view.component.js';
import WatchViewComponent from '../routes/watch/watch-view.component.js';

import MusicViewComponent from '../routes/music/music-view.component.js';
import DiscoverViewComponent from '../routes/discover/discover-view.component.js';
import DiscoverHomeViewComponent from '../routes/discover/home/discover-home-view.component.js';
import DiscoverServicesViewComponent from '../routes/discover/services/discover-services-view.component.js';
import DiscoverServicesTvViewComponent from '../routes/discover/services/tv/discover-services-tv-view.component.js';
import DiscoverUrlViewComponent from '../routes/discover/url/discover-url-view.component.js';

import MusicAlbumsViewComponent from '../routes/music/albums/music-albums-view.component.js';
import MusicArtistsViewComponent from '../routes/music/artists/music-artists-view.component.js';
import MusicArtistsDetailViewComponent from '../routes/music/artists/detail/music-artists-detail-view.component.js';
import MusicGenresViewComponent from '../routes/music/genres/music-genres-view.component.js';
import MusicTracksViewComponent from '../routes/music/tracks/music-tracks-view.component.js';

import NetworkViewComponent from '../routes/network/network-view.component.js';
import VideoMoviesComponent from '../routes/video/movies/video-movies-view.component.js';
import VideoTvshowsComponent from '../routes/video/tvshows/video-tvshows-view.component.js';
import VideoAllComponent from '../routes/video/all/video-all-view.component.js';
import MusicPlaylistsViewComponent from '../routes/music/playlists/music-playlists-view.component.js';
import VideoPlaylistsComponent from '../routes/video/playlists/video-playlists-view.component.js';

const routes = [
    {
        path: '/',
        component: BrowseViewComponent,
        redirect: '/video/all',
        children: [
            {
                path: 'music',
                component: MusicViewComponent,
                redirect: '/music/albums',
                children: [
                    {
                        path: 'albums',
                        component: MusicAlbumsViewComponent
                    },
                    {
                        path: 'artists',
                        component: MusicArtistsViewComponent,
                    },
                    {
                        path: 'artists/:id',
                        component: MusicArtistsDetailViewComponent
                    },
                    {
                        path: 'genres',
                        component: MusicGenresViewComponent
                    },
                    {
                        path: 'tracks',
                        component: MusicTracksViewComponent
                    },
                    {
                        path: 'playlists',
                        component: MusicPlaylistsViewComponent
                    }
                ]
            },
            {
                path: 'video',
                component: VideoViewComponent,
                redirect: '/video/all',
                children: [
                    {
                        path: 'all',
                        component: VideoAllComponent
                    },
                    {
                        path: 'movies',
                        component: VideoMoviesComponent
                    },
                    {
                        path: 'tvshows',
                        component: VideoTvshowsComponent
                    },
                    {
                        path: 'playlists',
                        component: VideoPlaylistsComponent
                    }
                ]
            },
            {
                path: 'network',
                component: NetworkViewComponent
            },
            {
                path: 'discover',
                component: DiscoverViewComponent,
                redirect: '/discover/home',
                children: [
                    {
                        path: 'home',
                        component: DiscoverHomeViewComponent
                    },
                    {
                        path: 'services',
                        component: DiscoverServicesViewComponent
                    },
                    {
                        path: 'services/tv',
                        component: DiscoverServicesTvViewComponent
                    },
                    {
                        path: 'url',
                        component: DiscoverUrlViewComponent
                    }
                ]
            }
        ]
    },
    {
        path: '/watch',
        component: WatchViewComponent
    }
];

export default VueRouter.createRouter({
    history: VueRouter.createWebHashHistory(),
    routes,
});
