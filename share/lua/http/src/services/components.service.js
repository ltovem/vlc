// Import views
import MainView from '../routes/main/main-view.component.js';

// Import components.
import Albums from '../components/albums/albums.component.js';
import AlbumItemGrid from '../components/albums/album-item/album-item-grid.component.js';
import AlbumItemList from '../components/albums/album-item/album-item-list.component.js';
import Artists from '../components/artists/artists.component.js';
import ArtistItemGrid from '../components/artists/artist-item/artist-item-grid.component.js';
import ArtistsDetail from '../components/artists/detail/artists-detail.component.js';
import CButton from '../components/button/c-button.component.js';
import ContinueWatching from '../components/continue-watching/continue-watching.component.js';
import EnqueueButton from '../components/enqueue-button/enqueue-button.component.js';
import EqualizerModal from '../components/equalizer/equalizer.component.js';
import GenreItemGrid from '../components/genres/genre-item/genre-item-grid.component.js';
import Genres from '../components/genres/genres.component.js';
import GridItem from '../components/grid-item/grid-item.component.js';
import ItemLayoutButton from '../components/item-layout-button/item-layout-button.component.js';
import Library from '../components/library/library.component.js';
import LibraryItem from '../components/library/library-item/library-item.component.js';
import MediaPlayer from '../components/media-player/media-player.component.js';
import SeekLoader from '../components/media-player/seek-loader/seek-loader.component.js';
import PlayerButton from '../components/player-button/player-button.component.js';
import PlaylistButtons from '../components/playlist/playlist-buttons.component.js';
import PlaylistAudioItem from '../components/playlist/playlist-audio-item.component.js';
import PlaylistVideoItem from '../components/playlist/playlist-video-item.component.js';
import Playlist from '../components/playlist/playlist.component.js';
import Playlists from '../components/playlists/playlists.component.js';
import PlaylistsItem from '../components/playlists/playlists-item/playlists-item.component.js';
import StreamManagerModal from '../components/sot/stream-manager.component.js';
import SvgIcon from '../components/svg-icon/svg-icon.component.js';
import TrackSyncModal from '../components/track-synchronisation/tracksync.component.js';
import Tracks from '../components/tracks/tracks.component.js';
import TrackItemList from '../components/tracks/track-item/track-item-list.component.js';
import Videos from '../components/videos/videos.component.js';
import VideoItemGrid from '../components/videos/video-item/video-item-grid.component.js';
import VideoItemList from '../components/videos/video-item/video-item-list.component.js';
import VideoItemPoster from '../components/videos/video-item/video-item-poster.component.js';
import VlmModal from '../components/vlm/vlm.component.js';


// Export all components.
export default [
    // Views
    {
        name: 'MainView',
        component: MainView,
    },

    // Components
    {
        name: 'albums',
        component: Albums,
    },
    {
        name: 'AlbumItemGrid',
        component: AlbumItemGrid,
    },
    {
        name: 'AlbumItemList',
        component: AlbumItemList,
    },
    {
        name: 'Artists',
        component: Artists,
    },
    {
        name: 'ArtistItemGrid',
        component: ArtistItemGrid,
    },
    {
        name: 'ArtistsDetail',
        component: ArtistsDetail,
    },
    {
        name: 'CButton',
        component: CButton,
    },
    {
        name: 'ContinueWatching',
        component: ContinueWatching,
    },
    {
        name: 'EnqueueButton',
        component: EnqueueButton,
    },
    {
        name: 'EqualizerModal',
        component: EqualizerModal,
    },
    {
        name: 'GenreItemGrid',
        component: GenreItemGrid,
    },
    {
        name: 'Genres',
        component: Genres,
    },
    {
        name: 'GridItem',
        component: GridItem,
    },
    {
        name: 'ItemLayoutButton',
        component: ItemLayoutButton,
    },
    {
        name: 'Library',
        component: Library,
    },
    {
        name: 'LibraryItem',
        component: LibraryItem,
    },
    {
        name: 'MediaPlayer',
        component: MediaPlayer,
    },
    {
        name: 'SeekLoader',
        component: SeekLoader,
    },
    {
        name: 'PlayerButton',
        component: PlayerButton,
    },
    {
        name: 'PlaylistButtons',
        component: PlaylistButtons,
    },
    {
        name: 'PlaylistAudioItem',
        component: PlaylistAudioItem,
    },
    {
        name: 'PlaylistVideoItem',
        component: PlaylistVideoItem,
    },
    {
        name: 'Playlist',
        component: Playlist,
    },
    {
        name: 'Playlists',
        component: Playlists,
    },
    {
        name: 'PlaylistsItem',
        component: PlaylistsItem,
    },
    {
        name: 'StreamManagerModal',
        component: StreamManagerModal,
    },
    {
        name: 'SvgIcon',
        component: SvgIcon,
    },
    {
        name: 'TrackSyncModal',
        component: TrackSyncModal,
    },
    {
        name: 'Tracks',
        component: Tracks,
    },
    {
        name: 'TrackItemList',
        component: TrackItemList,
    },
    {
        name: 'Videos',
        component: Videos,
    },
    {
        name: 'VideoItemGrid',
        component: VideoItemGrid,
    },
    {
        name: 'VideoItemList',
        component: VideoItemList,
    },
    {
        name: 'VideoItemPoster',
        component: VideoItemPoster,
    },
    {
        name: 'VlmModal',
        component: VlmModal,
    },
];
