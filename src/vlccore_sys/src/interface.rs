use crate::vlc_core::*;
use crate::vlc_core_sys::vlc_intf_GetMainPlaylist;

pub type IntThread = intf_thread_t;

impl IntThread {
    pub fn get_main_playlist(&mut self) -> Box<vlc_playlist> {
        unsafe { Box::from_raw(vlc_intf_GetMainPlaylist(self)) }
    }
}
