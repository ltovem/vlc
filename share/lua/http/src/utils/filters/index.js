import timeUtils from '../time/index.js';

export default {
    formatDuration(value) {
        if (!value) {
            return '';
        }

        return timeUtils.formatTime(value / 1000);
    }
}
