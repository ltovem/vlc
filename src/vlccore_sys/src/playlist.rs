use crate::player::CPlayer;
use crate::vlc_core::*;
use crate::vlc_core_sys::*;
use std::ffi::CString;
use std::ptr::null_mut;

pub type CPlaylist = vlc_playlist_t;

impl CPlaylist {
    /// Return the player owned by the playlist.
    ///
    /// \param playlist the playlist (not necessarily locked)
    /// \return the player

    pub fn get_player(&mut self) -> Option<&'static mut CPlayer> {
        let ptr = unsafe { vlc_playlist_GetPlayer(self) };
        if ptr == null_mut() {
            None
        } else {
            Some(unsafe { &mut *ptr })
        }
    }
    /// Lock the playlist/player.
    ///
    /// The playlist and its player share the same lock, to avoid lock-order
    /// inversion issues.
    ///
    /// \warning Do not forget that the playlist and player lock are the same (or
    /// you could lock twice the same and deadlock).
    ///
    /// Almost all playlist functions must be called with lock held (check their
    /// description).
    ///
    /// The lock is not recursive.

    pub fn lock(&mut self) {
        unsafe { vlc_playlist_Lock(self) };
    }

    /// Add a playlist listener.
    ///
    /// Return an opaque listener identifier, to be passed to
    /// vlc_player_RemoveListener().
    ///
    /// If notify_current_state is true, the callbacks are called once with the
    /// current state of the playlist. This is useful because when a client
    /// registers to the playlist, it may already contain items. Calling callbacks
    /// is a convenient way to initialize the client automatically.
    ///
    /// \param playlist             the playlist, locked
    /// \param cbs                  the callbacks (must be valid until the listener
    ///                             is removed)
    /// \param userdata             userdata provided as a parameter in callbacks
    /// \param notify_current_state true to notify the current state immediately via
    ///                             callbacks
    /// \return a listener identifier, or NULL if an error occurred

    pub fn add_listener<T>(
        &mut self,
        playlist_cbs: &mut vlc_playlist_callbacks,
        cbs_data: &mut T,
        notify_current_state: bool,
    ) -> Option<Box<vlc_playlist_listener_id>> {
        let ptr = unsafe {
            vlc_playlist_AddListener(
                self,
                playlist_cbs,
                cbs_data as *mut T as *mut libc::c_void,
                notify_current_state,
            )
        };
        if ptr == null_mut() {
            None
        } else {
            Some(unsafe { Box::from_raw(ptr) })
        }
    }
    /// Remove a player listener.
    ///
    /// \param playlist the playlist, locked
    /// \param id       the listener identifier returned by
    ///                 vlc_playlist_AddListener()

    pub fn remove_listener(&mut self, id: &mut vlc_playlist_listener_id) {
        unsafe { vlc_playlist_RemoveListener(self, id) }
    }

    /// Unlock the playlist
    pub fn unlock(&mut self) {
        unsafe { vlc_playlist_Unlock(self) }
    }

    /// Return number of items
    /// # Warning: not tested TODO
    pub fn count(&mut self) -> u64 {
        unsafe { vlc_playlist_Count(self) }
    }

    /// Clear the playlist
    /// # Warning: not tested TODO
    pub fn clear(&mut self) {
        unsafe { vlc_playlist_Clear(self) }
    }

    /// Move a slice of items to a given target index
    ///
    /// The slice and the target must be in range (both index+count and
    /// target+count less than or equal to count()
    /// # Warning: not tested TODO
    pub fn r#move(&mut self, index: u64, count: u64, target: u64) {
        unsafe { vlc_playlist_Move(self, index, count, target) }
    }

    /// Remove a slice of items at a given index
    ///
    /// The slice must be in range (index+count less than or equal to count()
    /// # Warning: not tested TODO
    pub fn remove(&mut self, index: u64, count: u64) {
        unsafe { vlc_playlist_Remove(self, index, count) }
    }

    /// Shuffle the playlist
    /// # Warning: not tested TODO
    pub fn shuffle(&mut self) {
        unsafe { vlc_playlist_Shuffle(self) }
    }

    /// Return the index of a given item
    /// # Warning: not tested TODO
    pub fn index_of(&mut self, item: &vlc_playlist_item_t) -> i64 {
        unsafe { vlc_playlist_IndexOf(self, item) }
    }

    /// Return the index of a given media
    /// # Warning: not tested TODO
    pub fn index_of_media(&mut self, media: &input_item_t) -> i64 {
        unsafe { vlc_playlist_IndexOfMedia(self, media) }
    }

    /// Return the index of a given item id
    /// # Warning: not tested TODO
    pub fn index_of_id(&mut self, id: u64) -> i64 {
        unsafe { vlc_playlist_IndexOfId(self, id) }
    }

    /// Return the playback "repeat" mode
    /// # Warning: not tested TODO
    pub fn get_playback_repeat(&mut self) -> vlc_playlist_playback_repeat {
        unsafe { vlc_playlist_GetPlaybackRepeat(self) }
    }

    /// Return the playback order
    /// # Warning: not tested TODO
    pub fn get_playback_order(&mut self) -> vlc_playlist_playback_order {
        unsafe { vlc_playlist_GetPlaybackOrder(self) }
    }

    /// Change the playback to "repeat" mode
    /// # Warning: not tested TODO
    pub fn set_playback_repeat(&mut self, repeat: vlc_playlist_playback_repeat) {
        unsafe { vlc_playlist_SetPlaybackRepeat(self, repeat) }
    }

    /// Change the playback order
    /// # Warning: not tested TODO
    pub fn set_playback_order(&mut self, order: vlc_playlist_playback_order) {
        unsafe { vlc_playlist_SetPlaybackOrder(self, order) }
    }

    /// Return the index of the current item
    /// # Warning: not tested TODO
    pub fn get_current_index(&mut self) -> i64 {
        unsafe { vlc_playlist_GetCurrentIndex(self) }
    }

    /// Indicate wheter a previous item is available
    /// # Warning: not tested TODO
    pub fn had_prev(&mut self) -> bool {
        unsafe { vlc_playlist_HasPrev(self) }
    }

    /// Return the playback order
    /// # Warning: not tested TODO
    pub fn playback_order(&mut self) -> vlc_playlist_playback_order {
        unsafe { vlc_playlist_GetPlaybackOrder(self) }
    }

    /// Indicate wheter a previous item is available
    /// # Warning: not tested TODO
    pub fn has_prev(&mut self) -> bool {
        unsafe { vlc_playlist_HasPrev(self) }
    }

    /// Indicate wheter a next item is available
    /// # Warning: not tested TODO
    pub fn has_next(&mut self) -> bool {
        unsafe { vlc_playlist_HasNext(self) }
    }

    /// Go to a given index
    ///
    /// The index must be -1 or in range (less thant count())
    /// # Warning: not tested TODO
    pub fn go_to(&mut self, index: i64) -> i32 {
        unsafe { vlc_playlist_GoTo(self, index) }
    }

    /// Start player
    /// # Warning: not tested TODO
    pub fn start(&mut self) -> i32 {
        unsafe { vlc_playlist_Start(self) }
    }

    /// Stop the player
    /// # Warning: not tested TODO
    pub fn stop(&mut self) {
        unsafe { vlc_playlist_Stop(self) }
    }

    /// Pause the player
    /// # Warning: not tested TODO
    pub fn pause(&mut self) {
        unsafe { vlc_playlist_Pause(self) }
    }

    /// Resume the player
    /// # Warning: not tested TODO
    pub fn resume(&mut self) {
        unsafe { vlc_playlist_Resume(self) }
    }

    /// Preparse the media, and expand it in the playlist on subitems added
    /// # Warning: not tested TODO
    pub fn preparse(&mut self, media: &mut input_item_t) {
        unsafe { vlc_playlist_Preparse(self, media) }
    }

    /// Export the playlist to a file
    /// # Warning: not tested TODO
    pub fn export(&mut self, filename: String, file_type: String) -> i32 {
        let c_filename = CString::new(filename).unwrap().as_ptr();
        let c_type = CString::new(file_type).unwrap().as_ptr();
        unsafe { vlc_playlist_Export(self, c_filename, c_type) }
    }
}
