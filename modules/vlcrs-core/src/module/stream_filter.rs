//! stream_filter

use std::{marker::PhantomData, ptr::NonNull};

use vlcrs_core_sys::{stream_t, vlc_logger, vlc_object_t};

use crate::error::Result;
use crate::{messages::Logger, stream::Stream};

use super::ModuleArgs;

pub use super::stream_t_like::StreamModule;
pub use super::stream_t_like::ThisStream;

/// Stream filter module
pub trait Module {
    /// Open function for a stream filter module
    fn open<'a>(
        _this_stream: ThisStream<'a>,
        source: &'a mut Stream,
        logger: &'a mut Logger,
        args: &mut ModuleArgs,
    ) -> Result<StreamModule<'a>>;
}

/// Generic module open callback for stream_t like steam
///
/// # Safety
///
/// The `object` parameter must point to a valid `stream_t` that has been initiliazed with
/// all the requirement so a any module can hock up to it.
pub unsafe extern "C" fn module_open<T: Module>(object: *mut vlc_object_t) -> i32 {
    let ptr_stream = object as *mut stream_t;

    let this_stream = ThisStream(ptr_stream, PhantomData);

    // SAFETY: TODO
    let stream_t_ptr_ptr: *mut *mut stream_t = unsafe { &mut (*ptr_stream).s };
    let mut_stream: &'static mut Stream = unsafe {
        stream_t_ptr_ptr
            .cast::<Stream>()
            .as_mut::<'static>()
            .unwrap()
    };

    // SAFETY: TODO
    let logger_ptr_ptr: *mut *mut vlc_logger = unsafe { &mut (*object).logger };
    let mut_logger: &'static mut Logger =
        unsafe { logger_ptr_ptr.cast::<Logger>().as_mut::<'static>().unwrap() };

    let mut module_args = ModuleArgs(NonNull::new(object).unwrap());

    match T::open(this_stream, mut_stream, mut_logger, &mut module_args) {
        Ok(stream_module) => unsafe { super::stream_t_like::register(ptr_stream, stream_module) },
        Err(err) => err.to_vlc_errno(),
    }
}

/// Generic module close callback for stream_t like steam
///
/// # Safety
///
/// The `object` parameter must point to a valid `stream_t` that has been initiliazed by
/// `module_close`.
pub unsafe extern "C" fn module_close(_object: *mut vlc_object_t) -> i32 {
    // nothing to do, watch `pf_close` for the actual closing of the stream
    0
}
