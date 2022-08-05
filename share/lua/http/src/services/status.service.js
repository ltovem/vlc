import langUtils from '../utils/lang/index.js';

export default {
    sendGetStatus(params) {
        return $.ajax({
            url: 'requests/status.json',
            data: params
        })
        .then((data) => {
            return JSON.parse(data);
        });
    },
    fetchStatus() {
        return this.sendGetStatus();
    },
    toggleRandom() {
        return this.sendGetStatus('command=pl_random');
    },
    toggleRepeat() {
        return this.sendGetStatus('command=pl_repeat');
    },
    toggleFullscreen() {
        return this.sendGetStatus('command=fullscreen');
    },
    play(id) {
        const idParam = `&id=${id}`;
        return this.sendGetStatus(`command=pl_play${langUtils.isPresent(id) && id !== -1 ? idParam : ''}`);
    },
    pause(id) {
        const idParam = `&id=${id}`;
        return this.sendGetStatus(`command=pl_pause${langUtils.isPresent(id) && id !== -1 ? idParam : ''}`);
    },
    updateVolume(volume) {
        const cmd = `command=volume&val=${volume}`;
        return this.sendGetStatus(cmd);
    },
    previous() {
        const cmd = `command=pl_previous`;
        return this.sendGetStatus(cmd);
    },
    next() {
        const cmd = `command=pl_next`;
        return this.sendGetStatus(cmd);
    },
    stop() {
        const cmd = `command=pl_stop`;
        return this.sendGetStatus(cmd);
    },
    seek(value) {
        const cmd = `command=seek&val=${value}`;
        return this.sendGetStatus(cmd);
    }
};
