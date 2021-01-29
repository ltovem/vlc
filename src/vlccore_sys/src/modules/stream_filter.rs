use crate::es_out::CEsOut;
use crate::modules::demux::CDemux;
use crate::vlc_core::{ssize_t, stream_t, VLC_EGENERIC, *};
use core::mem::ManuallyDrop;
use core::ptr::null_mut;
use libc::*;
use std::ptr;

use std::ptr::slice_from_raw_parts_mut;
#[no_mangle]
#[must_use]
pub extern "C" fn vlcrs_stream_filter_read(
    stream: *mut stream_t,
    buf_out: *mut u8,
    buflen: usize,
) -> ssize_t {
    let stream = unsafe { &mut *stream };
    let mut stream_filter_module = unsafe { stream_filter_module_from_raw(stream.p_sys) };
    let slice_buf_out: *mut [u8] = slice_from_raw_parts_mut(buf_out, buflen);
    let rust_buf_out: &mut [u8] = unsafe { &mut *slice_buf_out };
    match stream_filter_module.read(rust_buf_out) {
        Some(value) => value,
        None => VLC_EGENERIC as ssize_t,
    }
}

/// # Warning: not tested TODO
#[no_mangle]
#[must_use]
pub extern "C" fn vlcrs_stream_filter_block(stream: *mut stream_t, eof: *mut bool) -> *mut block_t {
    let stream = unsafe { &mut *stream };
    let mut stream_filter_module = unsafe { stream_filter_module_from_raw(stream.p_sys) };
    match stream_filter_module.block() {
        Some((block, eof_ret)) => {
            unsafe { *eof = eof_ret };
            Box::into_raw(block)
        }
        None => null_mut() as *mut block_t,
    }
}

/// # Warning: not tested TODO
#[no_mangle]
#[must_use]
pub extern "C" fn vlcrs_stream_filter_readdir(
    stream: *mut stream_t,
    input_item_node: *mut input_item_node_t,
) -> c_int {
    let stream = unsafe { &mut *stream };
    let mut stream_filter_module = unsafe { stream_filter_module_from_raw(stream.p_sys) };
    let input_item_node = unsafe { &mut *input_item_node };
    match stream_filter_module.readdir(input_item_node) {
        Some(value) => value,
        None => VLC_EGENERIC as c_int,
    }
}

/// # Warning: not tested TODO
#[no_mangle]
#[must_use]
pub extern "C" fn vlcrs_stream_filter_demux(stream: *mut stream_t) -> c_int {
    let stream = unsafe { &mut *stream };
    let mut stream_filter_module = unsafe { stream_filter_module_from_raw(stream.p_sys) };
    let rs_demux = stream.s();
    let rs_es_out = stream.es_out();
    match stream_filter_module.demux(rs_demux, rs_es_out) {
        Some(value) => value,
        None => VLC_EGENERIC as i32,
    }
}

#[no_mangle]
#[must_use]
pub extern "C" fn vlcrs_stream_filter_close(stream: *mut stream_t) {
    let stream = unsafe { &mut *stream };
    let mut stream_filter_module = unsafe { stream_filter_module_from_raw(stream.p_sys) };
    stream_filter_module.close();
}

pub trait StreamFilterModule {
    /// Read data
    ///
    /// Read data from the stream into a caller-supplied buffer.
    ///
    /// This may be NULL if the stream is actually a directory rather than a
    /// byte stream, or if block is not set.
    ///
    /// # Return value of isize
    /// -1: no data available yet
    /// 0: end of stream (incl. fatal error)
    /// isize > 0: number of bytes read (no more than len)
    fn read(&mut self, _buf_out: &mut [u8]) -> Option<i64> {
        None
    }

    /// Read data block
    ///
    /// Read a block of data. The data is read into a block of memory allocated
    /// by the stream. For some streams, data can be read more efficiently in
    /// block of certain size, and/or using a custom allocator for buffers.
    /// In such case, this callback should be provided instead of read;
    /// Otherwise, this should be NULL
    /// # Warning: not tested TODO
    fn block(&mut self) -> Option<(Box<block_t>, bool)> {
        None
    }
    /// Read directory
    ///
    /// Fill the item node from a directory
    /// (see doc/browsing.txt) for details
    /// # Warning: not tested TODO
    fn readdir(&mut self, _input_item_node: &mut input_item_node_t) -> Option<i32> {
        None
    }
    /// # Warning: not tested TODO
    fn demux(&mut self, _demux: &mut CDemux, _es_out: &mut CEsOut) -> Option<i32> {
        None
    }
    fn close(&mut self) {}
}

pub fn stream_filter_module_to_raw(
    stream_filter_module: Option<impl StreamFilterModule>,
) -> *mut c_void {
    stream_filter_module.map_or(ptr::null_mut(), |sys| {
        /* We need a trait object because when we receive a pointer from C, we
         * don't know the concrete Rust type, we just now that it implements
         * StreamFilterModule */
        let trait_object: Box<dyn StreamFilterModule> = Box::new(sys);
        /* A trait objet is a fat pointer, so box it one more time to get a thin
         * pointer, convertible to a raw pointer without loss of information */
        let thin_pointer = Box::new(trait_object);

        Box::into_raw(thin_pointer) as *mut c_void
    })
}

pub unsafe fn stream_filter_module_from_raw(
    sys: *mut c_void,
) -> ManuallyDrop<Box<Box<dyn StreamFilterModule>>> {
    assert!(!sys.is_null());
    ManuallyDrop::new(Box::from_raw(sys as *mut Box<dyn StreamFilterModule>))
}
