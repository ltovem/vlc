use crate::vlc_core::vlc_tick_t;
use crate::vlc_core_sys::*;

pub type CTick = vlc_tick_t;

pub fn tick_from_samples(samples: u64, samp_rate: u64) -> CTick {
    //TODO replace 1000 when bindgen properly import
    (1000000 * samples / samp_rate) as CTick
}

/// Precision monotonic clock
///
/// In principles, the clock has a precision of 1MHz. But the actual resolution
/// may be much lower, especially when it comes to sleeping with wait() or
/// sleep(). Most general purpose operating systems provide a resolution of only
/// 100 to 1000Hz.
///
/// # Warning
/// The origin date (time value "zero"). is not specified. It is typically the
/// time the kernel started, but this is platform-dependent.
pub fn tick_now() -> CTick {
    unsafe { vlc_tick_now() }
}

/// wait until a deadline
/// # Warning: not tested TODO
pub fn wait(deadline: CTick) {
    unsafe { vlc_tick_wait(deadline) }
}

/// wait for an interval of time.
/// # Notes
/// The delay may be exceeded due to OS scheduling.
/// This function is a cancellation point.
/// # Warning: not tested TODO
pub fn sleep(interval: CTick) {
    unsafe { vlc_tick_sleep(interval) }
}
