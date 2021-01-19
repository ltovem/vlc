#![allow(non_camel_case_types, dead_code, non_snake_case)]

extern crate core;
extern crate flavors;
extern crate libc;
extern crate nom;

use crate::flv::*;
use vlccore_sys::modules::demux::*;
use vlccore_sys::vlc_core::*;

#[no_mangle]
pub unsafe extern "C" fn flv_open(stream: *mut stream_t) -> *mut libc::c_void {
    let stream = &mut *stream;
    let ret = flv_open_flv(stream);
    demux_module_to_raw(ret)
}
