use crate::inflate::Inflate;
use std::os::raw::c_int;
use vlccore_sys::modules::stream_filter::stream_filter_module_to_raw;
use vlccore_sys::vlc_core::stream_t;

#[no_mangle]
pub unsafe extern "C" fn inflate_new(stream: *mut stream_t, bits: c_int) -> *mut libc::c_void {
    let inflate = Inflate::new(&mut (*stream), bits as u8);
    stream_filter_module_to_raw(inflate)
}
