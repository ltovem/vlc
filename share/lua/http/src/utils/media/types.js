export const VIDEO_TYPES = [
    'asf', 'avi', 'bik', 'bin', 'divx', 'drc', 'dv', 'f4v', 'flv', 'gxf', 'iso',
    'm1v', 'm2v', 'm2t', 'm2ts', 'm4v', 'mkv', 'mov',
    'mp2', 'mp4', 'mpeg', 'mpeg1',
    'mpeg2', 'mpeg4', 'mpg', 'mts', 'mtv', 'mxf', 'mxg', 'nuv',
    'ogg', 'ogm', 'ogv', 'ogx', 'ps',
    'rec', 'rm', 'rmvb', 'rpl', 'thp', 'ts', 'txd', 'vob', 'wmv', 'xesc'
];

export const AUDIO_TYPES = [
    '3ga', 'a52', 'aac', 'ac3', 'ape', 'awb', 'dts', 'flac', 'it',
    'm4a', 'm4p', 'mka', 'mlp', 'mod', 'mp1', 'mp2', 'mp3',
    'oga', 'ogg', 'oma', 's3m', 'spx', 'thd', 'tta',
    'wav', 'wma', 'wv', 'xm'
];

export const PLAYLIST_TYPES = [
    'asx', 'b4s', 'cue', 'ifo', 'm3u', 'm3u8', 'pls', 'ram', 'rar',
    'sdp', 'vlc', 'xspf', 'zip', 'conf'
];

export const guessTypeFromUri = (uri) => {
    if (!uri) {
        return;
    }
    const ext = uri.split('.').pop();
    if (!ext) {
        return;
    }
    if (VIDEO_TYPES.includes(ext)) {
        return 'video';
    } else if (AUDIO_TYPES.includes(ext)) {
        return 'audio';
    } else if (PLAYLIST_TYPES.includes(ext)) {
        return 'playlist';
    }
}
