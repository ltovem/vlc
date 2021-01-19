use crate::vlc_core::*;
use crate::vlc_core_sys::*;
use std::ptr::null_mut;

pub trait VLCObject {}

#[warn(non_snake_case)]
pub fn log<T: VLCObject>(object: &mut T, priority: vlc_log_type, module: &[u8], format: &str) {
    unsafe {
        let ptr = object as *mut T;
        vlc_object_Log(
            ptr as *mut vlc_object_t,
            priority as i32,
            module.as_ptr() as *const i8,
            null_mut(),
            0,
            null_mut(),
            format.as_ptr() as *const i8,
        )
    }
}

#[macro_export]
macro_rules! LogInfo {
  ($demux:expr, $module:expr, $format:expr) => {{
    log($demux, vlc_log_type::VLC_MSG_INFO, $module, concat!($format, "\0"))
  }};
  ($demux:expr, $module:expr, $format:expr, $($args:expr),*) => {{
    let formatted = std::fmt::format(format_args!(concat!($format, "\0"),$($args),*));
    log($demux, vlc_log_type::VLC_MSG_INFO, $module, &formatted)
  }};
}

#[macro_export]
macro_rules! LogErr {
  ($demux:expr, $module:expr, $format:expr) => {{
    log($demux, vlc_log_type::VLC_MSG_ERR, $module, concat!($format, "\0"))
  }};
  ($demux:expr, $module:expr, $format:expr, $($args:expr),*) => {{
    let formatted = std::fmt::format(format_args!(concat!($format, "\0"),$($args),*));
    log($demux, vlc_log_type::VLC_MSG_ERR, $module, &formatted)
  }};
}

#[macro_export]
macro_rules! LogWarn {
  ($demux:expr, $module:expr, $format:expr) => {{
    log($demux, vlc_log_type::VLC_MSG_WARN, $module, concat!($format, "\0"))
  }};
  ($demux:expr, $module:expr, $format:expr, $($args:expr),*) => {{
    let formatted = std::fmt::format(format_args!(concat!($format, "\0"),$($args),*));
    log($demux, vlc_log_type::VLC_MSG_WARN, $module, &formatted)
  }};
}

#[macro_export]
macro_rules! LogDbg {
  ($demux:expr, $module:expr, $format:expr) => {{
    log($demux, vlc_log_type::VLC_MSG_DBG, $module, concat!($format, "\0"))
  }};
  ($demux:expr, $module:expr, $format:expr, $($args:expr),*) => {{
    let formatted = std::fmt::format(format_args!(concat!($format, "\0"),$($args),*));
    log($demux, vlc_log_type::VLC_MSG_DBG, $module, &formatted)
  }};
}

#[macro_export]
macro_rules! vlc_Log {
  ($demux:expr, $priority:expr, $module:expr, $format:expr) => {{
    log($demux, $priority, $module, concat!($format, "\0"))
  }};
  ($demux:expr, $priority:expr, $module:expr, $format:expr, $($args:expr),*) => {{
    let formatted = std::fmt::format(format_args!(concat!($format, "\0"),$($args),*));
    log($demux, $priority, $module, &formatted)
  }};
}
