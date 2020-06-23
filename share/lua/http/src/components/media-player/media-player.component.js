const formatTime = second => new Date(second * 1000).toISOString().substr(11, 8);
Vue.component('media-player', {
    template: '#media-player-template',
    props: ['src', 'autoPlay', 'ended', 'canPlay'],
    computed: {
        ...Vuex.mapState({
            activeItem: state => state.playlist.activeItem,
            status: state => state.status.data
        }),
    },
    data() {
        return {
            firstPlay: true,
            isMuted: false,
            loaded: false,
            playing: false,
            paused: false,
            percentage: 0,
            currentTime: '00:00:00',
            audio: undefined,
            totalDuration: 0,
            duration: 0,
            volumeIconState: 'up',
            formattedDuration: formatTime(0),
            formattedCurrentTime: formatTime(0),
            imageSrc
        }
    },
    methods: {
        init: function() {
            this.player.addEventListener('loadeddata', this._handleLoaded);
            this.player.addEventListener('pause', this._handlePlayPause);
            this.player.addEventListener('play', this._handlePlayPause);
            this.player.addEventListener('seeked', this._handleSeeked);
            this.player.addEventListener('progress', this._handleProgress);
            this.player.addEventListener('timeupdate', this._handleTimeUpdate);
            this.player.addEventListener('ended', this._handleEnded);
            this.player.volume = 0.5;

            this.scrubber.addEventListener('input', this._handleScrubberChange);
            this.volume.addEventListener('input', this._handleVolumeChange);
            this._handleVolumeChange();
        },
        fetchStatus() {
            this.$store.dispatch('status/fetchStatus');
        },
        refreshStatus() {
            if (this.interval) {
                clearTimeout(this.interval);
                this.interval = null;
            }
            this.fetchStatus();
            this.interval = setInterval(() => {
                this.fetchStatus();
            }, 1000);
        },
        play(dispatch = true) {
            this.previousState = null;
            if (this.playing) {
                return;
            }
            this.paused = false;
            if (this.src) {
                this.player.play().then(() => {
                    this.playing = true;
                });
            } else {
                this.playing = true;
            }
            if (dispatch) {
                this.$store.dispatch('status/play', this.status.currentplid);
            }
        },
        pause(dispatch = true) {
            this.previousState = null;
            this.paused = !this.paused;
            if (this.src) {
                this.paused ? this.player.pause() : this.player.play();
            }
            if (dispatch) {
                this.$store.dispatch('status/pause', this.status.currentplid);
            }
        },
        stop(dispatch = true) {
            this.previousState = null;
            this.paused = this.playing = false;
            if (this.src) {
                this.player.pause();
            }
            this.player.currentTime = 0;
            if (dispatch) {
                this.$store.dispatch('status/stop', this.status.currentplid);
            }
        },
        setPosition() {
            this.player.currentTime = parseInt(this.player.duration / 100 * this.percentage);
        },
        next() {
            this.$store.dispatch('status/next');
        },
        previous() {
            this.$store.dispatch('status/previous');
        },
        toggleRandom() {
            this.$store.dispatch('status/toggleRandom');
        },
        toggleRepeat() {
            this.$store.dispatch('status/toggleRepeat');
        },
        toggleFullscreen() {
            this.$store.dispatch('status/toggleFullscreen');
        },
        toggleMute() {
            if (parseInt(this.volume.value)) {
                this.previousVolume = this.volume.value;
                this.volume.value = 0;
            } else {
                this.volume.value = this.previousVolume;
            }
            this._handleVolumeChange();
        },
        handleState(state) {
            if (this.previousState && this.previousState == state) {
                return;
            }
            this.previousState = state;
            switch (state) {
                case 'playing':
                    this.play(false);
                    break;
                case 'paused':
                    this.pause(false);
                    break;
                case 'stopped':
                    this.stop(false);
                    break;
            }
        },
        handleVolume(volume) {
            this.volume.value = volume / 255 * 100;
            this._handleVolumeChange(false);
        },
        handleCurrentTime(currentTime) {
            this.player.currentTime = currentTime;
            this._handleTimeUpdate();
        },
        _handleSeeked() {
        },
        _handleProgress: function() {
            let duration = this.player.duration;
            if (duration > 0) {
                for (let i = 0; i < this.player.buffered.length; i++) {
                    if (this.player.buffered.start(this.player.buffered - 1 - i) < this.player.currentTime) {
                        let scaleX = this.player.buffered.end(this.player.buffered.length - 1 - i) / duration;
                        this.bufferBar.style.transform = `scaleX(${scaleX})`;
                        break;
                    }
                }
            }
        },
        _handleTimeUpdate: function() {
            this.duration = this.status.length * 1000;
            const currentTime = this.status.time * 1000;
            this.percentage = currentTime / this.duration * 100
            this.formattedCurrentTime = formatTime(currentTime / 1000);
            this.formattedDuration = formatTime(this.duration / 1000);
            if (this.duration > 0) {
                let scaleX = currentTime / this.duration;
                this.progressBar.style.transform = `scaleX(${scaleX})`;
                // Update scrubber
                this.scrubber.value = scaleX * 1000;
            }
        },
        _handleEnded() {
            this.paused = this.playing = false;
        },
        _handlePlayPause: function(e) {
            if (e.type === 'play' && this.firstPlay) {
                this.player.currentTime = 0;
                if (this.firstPlay) {
                    this.firstPlay = false;
                }
            }
            if (e.type === 'pause' && this.paused === false && this.playing === false) {
                this.formattedCurrentTime = formatTime(0);
            }
        },
        _handleLoaded: function() {
            if (this.player.readyState >= 2) {
                if (this.player.duration === Infinity) {
                    this.player.currentTime = 1e101;
                    this.player.ontimeupdate = () => {
                        this.player.ontimeupdate = () => {}
                        this.player.currentTime = 0
                        this.totalDuration = parseInt(this.player.duration)
                        this.loaded = true;
                    }
                } else {
                    this.totalDuration = parseInt(this.player.duration);
                    this.loaded = true;
                }
                // this.duration = formatTime(this.totalDuration);
                if (this.autoPlay) {
                    this.player.play();
                }
            } else {
                throw new Error('Failed to load sound file')
            }
        },
        _handleScrubberChange: function(e) {
            this.progressBar.style.transition = 'none;';
            this.progressBar.style.transform = `scaleX(${this.scrubber.value / 1000})`;
            this.progressBar.style.transition = 'transform .1s;';
            const cTime = this.scrubber.value / 1000 * (this.duration);
            this.player.currentTime = cTime;
            this.$store.dispatch('status/seek', (this.player.currentTime / 1000).toFixed());
        },
        _handleVolumeChange: function(dispatch = true) {
            this.volumeBar.style.width = `${this.volume.value}%`;
            this.player.volume = this.volume.value / 100;
            if (dispatch) {
                this.$store.dispatch('status/updateVolume', this.player.volume * 255);
            }
            if (this.volume.value >= 50) {
                this.volumeIconState = 'up';
            } else if (this.volume.value < 50 && this.volume.value > 0) {
                this.volumeIconState = 'down';
            } else {
                this.volumeIconState = 'mute';
            }
        }
    },
    mounted() {
        this.player = this.$refs.player;
        this.scrubber = this.$refs.scrubber;
        this.volume = this.$refs.volume;
        this.progressBar = this.$refs.progressBar;
        this.volumeBar = this.$refs.volumeBar;
        this.bufferBar = this.$refs.bufferBar;
        this.init();
        this.$store.subscribe((mutation, payload) => {
            switch (mutation.type) {
                case 'status/setStatus':
                    this.handleCurrentTime(payload.status.data.time);
                    this.handleState(payload.status.data.state);
                    this.handleVolume(payload.status.data.volume);
                    break;
                // case 'playlist/activeItem':
            }
        });
    },
    created() {
        this.refreshStatus();
    },
    beforeDestroy() {
        this.player.removeEventListener('loadeddata', this._handleLoaded);
        this.player.removeEventListener('pause', this._handlePlayPause);
        this.player.removeEventListener('play', this._handlePlayPause);
        this.player.removeEventListener('seeked', this._handleSeeked);
        this.player.removeEventListener('progress', this._handleProgress);
        this.player.removeEventListener('timeupdate', this._handleTimeUpdate);
        this.player.removeEventListener('ended', this._handleEnded);

        this.scrubber.removeEventListener('input', this._handleScrubberChange);
        this.volume.removeEventListener('input', this._handleVolumeChange);
        clearTimeout(this.interval);
    }
});
