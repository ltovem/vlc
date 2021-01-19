use crate::player::CPlayer;
use crate::vlc_core::*;
use crate::vlc_core_sys::*;
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
}
