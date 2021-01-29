use crate::input_item::CInputItem;
use crate::vlc_core::*;
use crate::vlc_core_sys::*;
use std::ptr::null_mut;

pub type CPlayer = vlc_player_t;

impl CPlayer {
    /// Get the number of tracks for an ES category
    ///
    /// @warning The returned size becomes invalid when the player is unlocked.
    ///
    /// @param player locked player instance
    /// @param cat VIDEO_ES, AUDIO_ES or SPU_ES
    /// @return number of tracks, or 0 (in case of error, or if the media is not
    /// started)

    pub fn get_track_count(&mut self, cat: es_format_category_e) -> u64 {
        unsafe { vlc_player_GetTrackCount(self, cat) }
    }
    /// Get the current played media.
    ///
    /// @see vlc_player_cbs.on_current_media_changed
    ///
    /// @param player locked player instance
    /// @return a valid media or NULL (if no media is set)
    /// # Warning: not tested TODO
    pub fn get_current_media(&mut self) -> Option<&'static mut CInputItem> {
        let ptr = unsafe { vlc_player_GetCurrentMedia(self) };
        if ptr == null_mut() {
            None
        } else {
            Some(unsafe { &mut *ptr })
        }
    }
    // Other methods
    // Get the current played media
    /*
    pub fn get_current_media(&mut self) -> Option<CInputItem> {
        let ret = unsafe { vlc_player_GetCurrentMedia(self) };
        if ret == null_mut() as *mut CInputItem { None }
        else { unsafe { *ret.clone() } }
    }
    */

    /// Remove a listener callback
    ///
    /// @param player locked player instance
    /// @param listener_id listener id returned by vlc_player_AddListener()

    pub fn remove_listener(&mut self, listener: &mut vlc_player_listener_id) {
        unsafe { vlc_player_RemoveListener(self, listener) }
    }
    /// Add a listener callback
    ///
    /// @note Every registered callbacks need to be removed by the caller with
    /// vlc_player_RemoveListener().
    ///
    /// @param player locked player instance
    /// @param cbs pointer to a vlc_player_cbs structure, the structure must be
    /// valid during the lifetime of the player
    /// @param cbs_data opaque pointer used by the callbacks
    /// @return a valid listener id, or NULL in case of allocation error

    pub fn add_listener(
        &mut self,
        player_cbs: &mut vlc_player_cbs,
        cbs_data: *mut libc::c_void,
    ) -> Option<Box<vlc_player_listener_id>> {
        let ptr = unsafe { vlc_player_AddListener(self, player_cbs, cbs_data) };
        if ptr == null_mut() {
            None
        } else {
            unsafe { Some(Box::from_raw(ptr)) }
        }
    }

    /// Lock the player
    ///
    /// All player functions (except vlc_player_Delete()) need to be called
    /// while the player lock is held
    /// # Warning: not tested TODO
    pub fn lock(&mut self) {
        unsafe { vlc_player_Lock(self) }
    }

    /// Unlock the player
    /// # Warning: not tested TODO
    pub fn unlock(&mut self) {
        unsafe { vlc_player_Unlock(self) }
    }

    /// Wait on a conditional variable
    ///
    /// This call allow users to use their own condition with the player mutex.
    /// # Warning: not tested TODO
    pub fn cond_wait(&mut self, cond: &mut vlc_cond_t) {
        unsafe { vlc_player_CondWait(self, cond) }
    }

    /// Set the current media
    ///
    /// This function replaces the current and next medias
    ///
    /// # Note
    /// A successfull call will always result of
    /// vlc_player_cbs.on_current_media_changed being called. This function is
    /// not block. If a media is currently being played, this media will be
    /// stopped and the requested will be set after.
    ///
    /// # Warning
    /// This function is either synchronous (if the player state is STOPPED) or
    /// asynchronous. In the later case, get_current_media() will return the old
    /// media, even after this call, and until the
    /// vlc_player_cbs.on_current_media changed is called
    /// # Warning: not tested TODO
    pub fn set_current_media(&mut self, media: &mut input_item_t) -> i32 {
        unsafe { vlc_player_SetCurrentMedia(self, media) }
    }
}
