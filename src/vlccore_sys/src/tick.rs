use crate::vlc_core::vlc_tick_t;
use crate::vlc_core_sys::*;

pub type CTick = vlc_tick_t;

pub fn tick_from_samples(samples: u64, samp_rate: u64) -> CTick {
    //TODO replace 1000 when bindgen properly import
    (1000000 * samples / samp_rate) as CTick
}

pub fn tick_now() -> CTick {
    unsafe { vlc_tick_now() }
}
