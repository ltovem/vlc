import {
    guessTypeFromUri
} from '../utils/media/types.js';

export default {
    sendPlaylist(params) {
        return $.ajax({
            url: 'requests/playlist.json',
            data: params
        })
        .then((data) => {
            let jsonData = JSON.parse(data);
            jsonData = jsonData.map((data) => {
                data.duration = data.duration ? data.duration * 1000 : 0;
                // @TODO: remove when type fixed on backend side
                data.type = guessTypeFromUri(data.uri);
                return data;
            });
            return jsonData;
        });
    },
    sendPlaylistStatus(params) {
        return $.ajax({
            url: 'requests/status.json',
            data: params
        })
        .then((data) => {
            const jsonData = JSON.parse(data);
            return jsonData;
        });
    },
    fetchPlaylist() {
        return this.sendPlaylist();
    },
    addItem(src) {
        return this.sendPlaylistStatus(`command=in_enqueue&input=${src}`);
    },
    removeItem(id) {
        if (id) {
            return this.sendPlaylistStatus(`command=pl_delete&id=${id}`);
        }
        return this.sendPlaylistStatus(`command=pl_empty`);
    }
};
