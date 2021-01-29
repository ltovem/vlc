use crate::cuesheet::Cuesheet;
use std::convert::TryFrom;
use std::ffi::{CStr, CString};
use std::io::{BufRead, BufReader};
use std::os::raw::{c_char, c_int};
use std::ptr::null_mut;
use vlccore_sys::stream::CStream;
use vlccore_sys::vlc_core::*;

pub struct CCuesheet {
    cuesheet: Cuesheet,
}

fn rust_cuesheet_from_demux(stream: &mut CStream) -> Option<Cuesheet> {
    let mut cuesheet = Cuesheet::default();
    let reader = BufReader::new(stream);
    for line in reader.lines() {
        match line {
            Ok(l) => cuesheet.process_line(l.as_str())?,
            Err(_) => return None,
        }
    }
    Some(cuesheet)
}

#[no_mangle]
pub unsafe extern "C" fn cuesheet_get_track_property(
    c_cuesheet: *mut CCuesheet,
    index: c_int,
    line: *const c_char,
) -> *mut c_char {
    let cuesheet = &(*c_cuesheet).cuesheet;

    usize::try_from(index)
        .ok()
        .and_then(|index| cuesheet.get_track_at_index(index))
        .zip(CStr::from_ptr(line).to_str().ok())
        .and_then(|(track, property)| track.get_property(property))
        .map(|value| convert_string_to_ptr(value.to_string()))
        .unwrap_or(null_mut())
}

unsafe fn convert_string_to_ptr(string: String) -> *mut c_char {
    if string.is_empty() {
        return std::ptr::null_mut();
    }
    CString::new(string)
        .map(|ptr| ptr.into_raw())
        .unwrap_or(null_mut())
}

#[no_mangle]
pub unsafe extern "C" fn unref_cuesheet_string(text: *mut c_char) {
    let _ = CString::from_raw(text);
}

#[no_mangle]
pub unsafe extern "C" fn unref_cuesheet(cuesheet: *mut CCuesheet) {
    let _ = Box::from_raw(cuesheet);
}

#[no_mangle]
pub unsafe extern "C" fn read_dir(
    demux_s: *mut stream_t,
    subitems: *mut input_item_node_t,
    url: *mut c_char,
) -> c_int {
    let rust_url = CStr::from_ptr(url).to_string_lossy().to_owned();
    let rust_cuesheet = rust_cuesheet_from_demux(&mut (*demux_s));

    if rust_cuesheet.is_none() {
        return VLC_EGENERIC as c_int;
    }
    rust_cuesheet.unwrap().read_dir(&mut *subitems, &rust_url) as c_int
}
