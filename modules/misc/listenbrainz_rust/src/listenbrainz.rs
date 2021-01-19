use libc::c_void;
use listenbrainz::api::submit_listens;
use listenbrainz::api_struct::*;
use std::ptr::null_mut;
use std::thread;
use std::thread::JoinHandle;
use std::time::{SystemTime, UNIX_EPOCH};
use vlccore_sys::modules::interface::*;
use vlccore_sys::tick::tick_now;
use vlccore_sys::vlc_core::vlc_meta_type_t::*;
use vlccore_sys::vlc_core::*;
use std::sync::mpsc::*;
use vlccore_sys::callbacks::playlist::*;

pub struct IntfSys {
    pub queue: Sender<(String, UserToken, Listenbrainz)>,
    pub thread: JoinHandle<()>,
    pub playlist: Box<vlc_playlist_t>,
    pub playlist_listener: Box<vlc_playlist_listener_id>,
    #[doc = "< where to submit data"]
    pub now_url: String,
    #[doc = "< Authentication token"]
    pub auth_token: Option<::listenbrainz::api_struct::UserToken>,
    #[doc = "< song being played"]
    pub current_song: AudioscrobblerSong,
    #[doc = "< time when vlc paused"]
    pub time_pause: vlc_tick_t,
    #[doc = "< total time in pause"]
    pub time_total_pauses: vlc_tick_t,
}

#[derive(Clone)]
pub struct AudioscrobblerSong {
    #[doc = "< track artist"]
    pub psz_a: String,
    #[doc = "< track title"]
    pub psz_t: String,
    #[doc = "< track album"]
    pub psz_b: String,
    #[doc = "< track number"]
    pub psz_n: String,
    #[doc = "< track length"]
    pub l: ::std::os::raw::c_int,
    #[doc = "< musicbrainz id"]
    pub psz_m: String,
    #[doc = "< date since epoch"]
    pub date: time_t,
    #[doc = "< playing start"]
    pub start: vlc_tick_t,
}

impl Default for AudioscrobblerSong {
    fn default() -> Self {
        AudioscrobblerSong {
            psz_a: String::new(),
            psz_t: String::new(),
            psz_b: String::new(),
            psz_n: String::new(),
            l: 0,
            psz_m: String::new(),
            date: 0,
            start: 0,
        }
    }
}

impl IntfSys {
    pub fn new(
        playlist: Box<vlc_playlist_t>,
        playlist_listener: Box<vlc_playlist_listener_id>,
        queue: Sender<(String, UserToken, Listenbrainz)>,
        jn_handle: JoinHandle<()>,
    ) -> Self {
        IntfSys {
            queue: queue,
            thread: jn_handle,
            playlist: playlist,
            playlist_listener: playlist_listener,
            now_url: String::new(),
            auth_token: None,
            current_song: AudioscrobblerSong::default(),
            time_pause: 0,
            time_total_pauses: 0,
        }
    }
}

pub fn read_metadata(sys: &mut IntfSys) {
    let item = sys
        .playlist
        .get_player()
        .and_then(|player| player.get_current_media());
    if item.is_none() {
        return;
    }
    let item = item.unwrap();

    sys.current_song.psz_a = item.get_meta(vlc_meta_Artist);
    sys.current_song.psz_t = item.get_meta(vlc_meta_Title);
    sys.current_song.psz_b = item.get_meta(vlc_meta_Album);
    sys.current_song.psz_m = item.get_meta(vlc_meta_TrackID);
    sys.current_song.psz_n = item.get_meta(vlc_meta_TrackNumber);
    sys.current_song.l = item.get_duration() as i32;
}

fn thread_loop(queue: Receiver<(String, UserToken, Listenbrainz)>) {
    loop {
        queue.recv().map(|val| {
            let (url, auth_token, listenbrainz) = val;
            submit_listens(url.to_string(), &auth_token, &listenbrainz);
        });
    }
}
impl IntfSys {
    fn send_song(&mut self) {
        read_metadata(self);
        let track_metadata = TrackMetadata {
            artist_name: self.current_song.psz_a.clone(),
            track_name: self.current_song.psz_t.clone(),
        };
        let listen_data = ListenData {
            listened_at: SystemTime::now()
                .duration_since(UNIX_EPOCH)
                .unwrap()
                .as_secs(),
            track_metadata: track_metadata,
        };
        let listenbrainz = Listenbrainz {
            listen_type: "single".to_string(),
            payload: vec![listen_data],
        };
        if self.auth_token.is_some() {
            let url = self.now_url.clone();
            let auth_token = self.auth_token.clone().unwrap();
            self.queue
                .send((url, auth_token, listenbrainz));
        }
    }
}

impl PlaylistCallbacks for IntfSys {
    fn playlist_on_current_index_changed(&mut self, _playlist: &mut vlc_playlist_t, _index: i64) {
        if tick_now() - self.current_song.start > 30 * 1000000 {
            self.send_song();
        }
        self.current_song.start = tick_now();
    }
}

impl InterfaceModule for IntfSys {}

#[no_mangle]
pub unsafe extern "C" fn listenbrainz_Open(p_this: *mut intf_thread_t) -> *mut c_void {
    let token = (&mut *(p_this as *mut vlc_object_t))
        .inherit_string("listenbrainz-token".to_string())
        .unwrap_or("".to_string());
    let url = (&mut *(p_this as *mut vlc_object_t))
        .inherit_string("listenbrainz-url".to_string())
        .unwrap_or("".to_string());

    let p_this = &mut *p_this;
    static mut PLAYLIST_CBS: vlc_playlist_callbacks = vlc_playlist_callbacks {
        on_items_reset: None,
        on_items_added: None,
        on_items_moved: None,
        on_items_removed: None,
        on_items_updated: None,
        on_playback_repeat_changed: None,
        on_playback_order_changed: None,
        on_current_index_changed: Some(vlcrs_playlist_callbacks_on_current_index_changed),
        on_has_prev_changed: None,
        on_has_next_changed: None,
        };

    let mut playlist = p_this.get_main_playlist();

    playlist.lock();
    let playlist_listener = playlist.add_listener(&mut PLAYLIST_CBS, p_this, false);
    if playlist_listener.is_none() {
        playlist.remove_listener(&mut playlist_listener.unwrap());
        playlist.unlock();
        return null_mut();
    }

    playlist.unlock();
    let (send, recv) = channel();
    let jn_handle = thread::spawn(move || thread_loop(recv));
    let mut sys = IntfSys::new(playlist, playlist_listener.unwrap(), send, jn_handle);
    sys.auth_token = Some(UserToken { token });
    sys.now_url = url;
    playlist_callback_to_raw(Some(sys))
}
