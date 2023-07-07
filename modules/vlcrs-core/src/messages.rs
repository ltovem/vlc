//! Messages facilities.

use std::{ffi::CStr, ptr::NonNull};

use vlcrs_core_sys::{vlc_Log, vlc_logger};

#[doc(hidden)]
pub use vlcrs_core_sys::vlc_log_type as LogType;

/// Logger object to be used with the warn!, error!, log! macros
#[repr(transparent)]
pub struct Logger(pub(crate) NonNull<vlc_logger>);

impl Logger {
    /// Log message to the logger
    pub fn log(&mut self, priority: LogType, file: &CStr, func: &CStr, line: u32, msg: &str) {
        extern "C" {
            pub static vlc_module_name: *const libc::c_char;
        }

        const GENERIC: &CStr = unsafe { CStr::from_bytes_with_nul_unchecked(b"generic\0") };
        const PRINTF_S: &CStr = unsafe { CStr::from_bytes_with_nul_unchecked(b"%.*s\0") };

        // SAFETY: All the pointers are non-null and valid and everything is fine
        unsafe {
            vlc_Log(
                &self.0.as_ptr(),
                priority as i32,
                GENERIC.as_ptr(),
                vlc_module_name,
                file.as_ptr(),
                line,
                func.as_ptr(),
                // We do this to avoid manipulating the original formated string to be printf
                // escaped. Using %s is just way simpler.
                PRINTF_S.as_ptr(),
                msg.len(),
                msg.as_ptr(),
            )
        }
    }
}

/// Log for a VLC Object
#[macro_export]
macro_rules! log {
    ($logger:expr, $level:expr, $format:expr, $($args:expr),*) => {{
        // SAFETY: The file name cannot be nul and doens't contains nul byte.
        // With the concat of the nul byte the slice is now a C-style array.
        let file = unsafe { ::std::ffi::CStr::from_bytes_with_nul_unchecked(concat!(file!(), "\0").as_bytes()) };
        let func = ::std::ffi::CString::new(::vlcrs_core::func!())
            .expect("should always to be valid utf-8");
        let func = func.as_c_str();
        let formatted = ::std::fmt::format(format_args!("{}\0", format_args!($format, $($args),*)));
        ::vlcrs_core::messages::Logger::log(
            $logger,
            $level,
            file,
            func,
            line!(),
            &formatted
        )
    }};
}

/// Debug-level log for a VLC Object
///
/// ```ignore
/// // let module = ...;
/// vlcrs_core::debug!(module, "test");
/// ```
#[macro_export]
#[doc(alias = "msg_Dbg")]
macro_rules! debug {
    ($logger:expr, $format:expr $(,)?) => {{
        $crate::log!($logger, $crate::messages::LogType::VLC_MSG_DBG, $format,)
    }};
    ($logger:expr, $format:expr, $($args:tt)*) => {{
        $crate::log!($logger, $crate::messages::LogType::VLC_MSG_DBG, $format, $($args)*)
    }};
}

/// Info-level log for a VLC Object
///
/// ```ignore
/// // let module = ...;
/// vlcrs_core::info!(module, "test");
/// ```
#[macro_export]
#[doc(alias = "msg_Err")]
macro_rules! info {
    ($logger:expr, $format:expr $(,)?) => {{
        $crate::log!($logger, $crate::messages::LogType::VLC_MSG_INFO, $format,)
    }};
    ($logger:expr, $format:expr, $($args:tt)*) => {{
        $crate::log!($logger, $crate::messages::LogType::VLC_MSG_INFO, $format, $($args)*)
    }};
}

/// Warning-level log for a VLC Object
///
/// ```ignore
/// // let module = ...;
/// vlcrs_core::warn!(module, "test");
/// ```
#[macro_export]
#[doc(alias = "msg_Warn")]
macro_rules! warn {
    ($logger:expr, $format:expr $(,)?) => {{
        $crate::log!($logger, $crate::messages::LogType::VLC_MSG_WARN, $format,)
    }};
    ($logger:expr, $format:expr, $($args:tt)*) => {{
        $crate::log!($logger, $crate::messages::LogType::VLC_MSG_WARN, $format, $($args)*)
    }};
}

/// Error-level log for a VLC Object
///
/// ```ignore
/// // let module = ...;
/// vlcrs_core::error!(module, "test");
/// ```
#[macro_export]
#[doc(alias = "msg_Err")]
macro_rules! error {
    ($logger:expr, $format:expr $(,)?) => {{
        $crate::log!($logger, $crate::messages::LogType::VLC_MSG_ERR, $format,)
    }};
    ($logger:expr, $format:expr, $($args:tt)*) => {{
        $crate::log!($logger, $crate::messages::LogType::VLC_MSG_ERR, $format, $($args)*)
    }};
}

/// Function name getter.
///
/// This macro returns the name of the enclosing function.
/// As the internal implementation is based on the [`std::any::type_name`], this macro derives
/// all the limitations of this function.
//
// Originate from stdext-rs (MIT-License: "Copyright (c) 2020 Igor Aleksanov"):
//  - https://github.com/popzxc/stdext-rs/blob/a79153387aa3f08d08dcaff08e214a17851d61c4/src/macros.rs#L63-L74
#[macro_export]
macro_rules! func {
    () => {{
        // Okay, this is ugly, I get it. However, this is the best we can get on a stable rust.
        fn f() {}
        fn type_name_of<T>(_: T) -> &'static str {
            std::any::type_name::<T>()
        }
        let name = type_name_of(f);
        // `3` is the length of the `::f`.
        &name[..name.len() - 3]
    }};
}
