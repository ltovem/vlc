import playlist from './modules/playlist.js';
import status from './modules/status.js';
import layout from './modules/layout.js';
import music from './modules/music.js';
import video from './modules/video.js';
import vlmCmd from './modules/vlm_cmd.js';

export default new Vuex.Store({
    modules: {
        playlist,
        status,
        layout,
        music,
        video,
        vlmCmd
    }
});
