#![allow(non_snake_case)]

use crate::es_out::*;
use crate::message::VLCObject;
use crate::vlc_core::{ssize_t, VLC_DEMUXER_EGENERIC, VLC_DEMUXER_EOF, VLC_DEMUXER_SUCCESS, *};
use libc::*;
use std::mem::ManuallyDrop;
use std::ptr;

pub type CDemux = demux_t;
impl VLCObject for CDemux {}

#[no_mangle]
#[must_use]
pub extern "C" fn vlcrs_demux_get_start(sys: *mut c_void) -> c_long {
    let demux_module = unsafe { demux_module_from_raw(sys) };
    demux_module.start()
}

#[no_mangle]
#[must_use]
pub extern "C" fn vlcrs_demux_get_end(sys: *mut c_void) -> c_long {
    let demux_module = unsafe { demux_module_from_raw(sys) };
    demux_module.end()
}

#[no_mangle]
#[must_use]
pub extern "C" fn vlcrs_demux_get_bitrate(sys: *mut c_void) -> c_long {
    let demux_module = unsafe { demux_module_from_raw(sys) };
    demux_module.bitrate()
}

#[no_mangle]
#[must_use]
pub extern "C" fn vlcrs_demux_get_align(sys: *mut c_void) -> c_int {
    let demux_module = unsafe { demux_module_from_raw(sys) };
    demux_module.align()
}

#[no_mangle]
#[must_use]
pub extern "C" fn vlcrs_demux_demux(demux: *mut demux_t) -> c_int {
    let demux = unsafe { &mut *demux };
    let mut demux_module = unsafe { demux_module_from_raw(demux.p_sys) };
    let mut rs_demux = demux.s();
    let mut rs_es_out = demux.es_out();
    demux_module.demux(&mut rs_demux, &mut rs_es_out) as i32
}

#[no_mangle]
#[must_use]
pub extern "C" fn vlcrs_demux_close(demux: *mut demux_t) {
    let demux = unsafe { &mut *demux };
    let mut demux_module = unsafe { demux_module_from_raw(demux.p_sys) };
    let mut rs_demux = demux.s();
    demux_module.close(&mut rs_demux);
    unsafe {
        ManuallyDrop::drop(&mut demux_module);
    }
}

#[no_mangle]
#[must_use]
pub extern "C" fn vlcrs_demux_control_is_playlist(
    demux: *mut demux_t,
    sys: *mut c_void,
    b: *mut bool,
) -> c_int {
    let mut demux_module = unsafe { demux_module_from_raw(sys) };
    let mut rs_demux = unsafe { &mut *demux };
    let ret = demux_module.control_IS_PLAYLIST(&mut rs_demux);
    match ret {
        Some(value) => unsafe {
            *b = value;
            VLC_SUCCESS as i32
        },
        None => VLC_ENOOBJ as i32,
    }
}

#[no_mangle]
#[must_use]
pub extern "C" fn vlcrs_demux_control_can_seek(
    demux: *mut demux_t,
    sys: *mut c_void,
    b: *mut bool,
) -> c_int {
    let mut demux_module = unsafe { demux_module_from_raw(sys) };
    let mut rs_demux = unsafe { &mut *demux };
    let b = unsafe { &mut *b };
    let ret = demux_module.control_CAN_SEEK(&mut rs_demux);
    match ret {
        Some(val) => {
            *b = val;
            VLC_SUCCESS as i32
        }
        None => VLC_ENOOBJ as i32,
    }
}

#[no_mangle]
#[must_use]
pub extern "C" fn vlcrs_demux_control_get_length(
    demux: *mut demux_t,
    sys: *mut c_void,
    tick: *mut vlc_tick_t,
) -> c_int {
    let mut demux_module = unsafe { demux_module_from_raw(sys) };
    let mut rs_demux = unsafe { &mut *demux };
    let tick = unsafe { &mut *tick };
    let ret = demux_module.control_GET_LENGTH(&mut rs_demux);
    match ret {
        Some(value) => {
            *tick = value;
            VLC_SUCCESS as i32
        }
        None => VLC_ENOOBJ as i32,
    }
}

#[no_mangle]
#[must_use]
pub extern "C" fn vlcrs_demux_control_get_position(
    demux: *mut demux_t,
    sys: *mut c_void,
    double: *mut c_double,
) -> c_int {
    let mut demux_module = unsafe { demux_module_from_raw(sys) };
    let mut rs_demux = unsafe { &mut *demux };
    let double = unsafe { &mut *double };
    let ret = demux_module.control_GET_POSITION(&mut rs_demux);
    match ret {
        Some(value) => {
            *double = value;
            VLC_SUCCESS as i32
        }
        None => VLC_ENOOBJ as i32,
    }
}

#[no_mangle]
#[must_use]
pub extern "C" fn vlcrs_demux_control_set_position(
    demux: *mut demux_t,
    sys: *mut c_void,
    double: c_double,
    boolean: bool,
) -> c_int {
    let mut demux_module = unsafe { demux_module_from_raw(sys) };
    let mut rs_demux = unsafe { &mut *demux };
    let ret = demux_module.control_SET_POSITION(&mut rs_demux, double, boolean);
    match ret {
        Some(()) => VLC_SUCCESS as i32,
        None => VLC_ENOOBJ as i32,
    }
}

pub enum RetDemux {
    ERR = VLC_DEMUXER_EGENERIC as isize,
    EOF = VLC_DEMUXER_EOF as isize,
    OK = VLC_DEMUXER_SUCCESS as isize,
}

pub trait DemuxModule {
    fn demux(&mut self, _demux: &mut CDemux, _es_out: &mut CEsOut) -> RetDemux;
    fn close(&mut self, _demux: &mut CDemux) {}

    /// Checks whether the stream supports seeking.
    /// Can fail if seeking is not supported (same as returning false).
    /// \bug Failing should not be allowed.
    ///
    /// arg1 = boolg
    fn control_CAN_SEEK(&mut self, _s: &mut CDemux) -> Option<bool> {
        None
    }

    /// Checks whether (long) pause then stream resumption is supported.
    ///Can fail only if synchronous and <b>not</b> an access-demuxer. The
    ///underlying input stream then determines if pause is supported.
    ///\bug Failing should not be allowed.
    ///arg1= boolg
    fn control_CAN_PAUSE(&mut self) -> Option<bool> {
        None
    }

    /// Whether the stream can be read at an arbitrary pace.
    ///Cannot fail.
    ///arg1= boolg
    fn control_CAN_CONTROL_PACE(&mut self) -> Option<bool> {
        None
    }

    /// Retrieves the PTS delay (roughly the default buffer duration).
    ///   Can fail only if synchronous and <b>not</b> an access-demuxer. The
    ///   underlying input stream then determines the PTS delay.
    ///   arg1= vlc_tick_tg
    fn control_GET_PTS_DELAY(&mut self) -> Option<vlc_tick_t> {
        None
    }

    /// Retrieves stream meta-data.
    ///   Should fail if no meta-data were retrieved.
    ///   arg1= vlc_meta_tg
    fn control_GET_META(&mut self) -> Option<vlc_tick_t> {
        None
    }

    /// Retrieves an estimate of signal quality and strength.
    ///   Can fail.
    ///   arg1=double *quality, arg2=double *strength
    fn control_GET_SIGNAL(&mut self) -> Option<(f64, f64)> {
        None
    }

    /// Sets the paused or playing/resumed state.
    ///   Streams are initially in playing state. The control always specifies a
    ///   change from paused to playing (false) or from playing to paused (true)
    ///   and streams are initially playing; a no-op cannot be requested.
    ///   The control is never used if fn control_CAN_PAUSE fails.
    ///   Can fail.
    ///   arg1= bool
    fn control_SET_PAUSE_STATE(&mut self, _s: &mut CDemux, _arg1: bool) -> Option<()> {
        None
    }
    /// Seeks to the beginning of a title.
    ///   The control is never used if DEMUX_GET_TITLE_INFO fails.
    ///   Can fail.
    ///   arg1= int
    fn control_SET_TITLE(&mut self, _arg1: i32) -> Option<()> {
        None
    }
    /// Seeks to the beginning of a chapter of the current title.
    ///   The control is never used if DEMUX_GET_TITLE_INFO fails.
    ///   Can fail.
    ///   arg1= int
    fn control_SET_SEEKPOINT(&mut self, _arg1: i32) -> Option<()> {
        None
    }

    /// Read the title number currently playing
    ///   Can fail.
    ///   arg1= intg
    fn control_GET_TITLE(&mut self) -> Option<i32> {
        None
    } /* arg1= int*           can fail */

    /// Read the seekpoint/chapter currently playing
    ///   Can fail.
    ///   arg1= intg
    fn control_GET_SEEKPOINT(&mut self) -> Option<i32> {
        None
    } /* arg1= int*           can fail */

    /// I. Common queries to access_demux and demux
    /// POSITION double between 0.0 and 1.0
    /// arg1= double *
    fn control_GET_POSITION(&mut self, _s: &mut CDemux) -> Option<f64> {
        None
    }

    /// arg1= double arg2= bool b_precise    res=can fail
    fn control_SET_POSITION(&mut self, _s: &mut CDemux, _arg1: f64, _arg2: bool) -> Option<()> {
        None
    }

    /// LENGTH/TIME in microsecond, 0 if unknown
    /// arg1= vlc_tick_t *   res=
    fn control_GET_LENGTH(&mut self, _s: &mut CDemux) -> Option<vlc_tick_t> {
        None
    }

    /// arg1= vlc_tick_t *   res=
    fn control_GET_TIME(&mut self, _s: &mut CDemux) -> Option<vlc_tick_t> {
        None
    }

    /// arg1= vlc_tick_t arg2= bool b_precise   res=can fail
    fn control_SET_TIME(&mut self, _s: &mut CDemux, _arg1: vlc_tick_t, _arg2: bool) -> Option<()> {
        None
    }

    /// Normal or original time, used mainly by the ts module
    /// arg1= vlc_tick_t *   res= can fail, in that case VLC_TICK_0 will be used as NORMAL_TIME
    fn control_GET_NORMAL_TIME(&mut self, _s: &mut CDemux) -> Option<vlc_tick_t> {
        None
    }

    /// DEMUX_SET_GROUP_* / DEMUX_SET_ES is only a hint for demuxer (mainly DVB)
    ///   to avoid parsing everything (you should not use this to call
    ///   es_out_Control()).
    ///   If you don't know what to do with it, just IGNORE it: it is safe(r).
    fn control_SET_GROUP_DEFAULT(&mut self) -> Option<()> {
        None
    }

    fn control_SET_GROUP_ALL(&mut self) -> Option<()> {
        None
    }

    /// arg1= int, can fail
    fn control_SET_ES(&mut self, _arg1: i32) -> Option<()> {
        None
    }

    /// Ask the demux to demux until the given date at the next pf_demux call
    ///   but not more (and not less, at the precision available of course).
    ///   XXX: not mandatory (except for subtitle demux) but will help a lot
    ///   for multi-input
    /// arg1= vlc_tick_t     can fail
    fn control_SET_NEXT_DEMUX_TIME(&mut self) -> Option<vlc_tick_t> {
        None
    }

    /// FPS for correct subtitles handling
    /// arg1= double *       res=can fail
    fn control_GET_FPS(&mut self) -> Option<f64> {
        None
    }

    /// Meta data
    /// arg1= bool *   res can fail
    fn control_HAS_UNSUPPORTED_META(&mut self) -> Option<bool> {
        None
    }

    ///   \todo Document
    ///   \warning The prototype is different from STREAM_SET_RECORD_STATE
    ///   The control is never used if DEMUX_CAN_RECORD fails or returns false.
    ///   Can fail.
    ///   arg1= bool
    fn control_SET_RECORD_STATE(&mut self) -> Option<()> {
        None
    }

    /// Checks whether the stream is actually a playlist, rather than a real
    ///   stream.
    ///   Can fail if the stream is not a playlist (same as returning false).
    ///   arg1= boolg

    fn control_IS_PLAYLIST(&mut self, _s: &mut CDemux) -> Option<bool> {
        None
    }

    fn start(&self) -> i64 {
        -1
    }
    fn end(&self) -> i64 {
        -1
    }
    fn bitrate(&self) -> i64 {
        -1
    }
    fn align(&self) -> i32 {
        -1
    }
}

/* Could not implement From, because both Option and c_void are defined outside this crate */
pub fn demux_module_to_raw(demux_module: Option<impl DemuxModule>) -> *mut c_void {
    demux_module.map_or(ptr::null_mut(), |sys| {
        /* We need a trait object because when we receive a pointer from C, we
         * don't know the concrete Rust type, we just now that it implements
         * DemuxModule */
        let trait_object: Box<dyn DemuxModule> = Box::new(sys);
        /* A trait objet is a fat pointer, so box it one more time to get a thin
         * pointer, convertible to a raw pointer without loss of information */
        let thin_pointer = Box::new(trait_object);

        Box::into_raw(thin_pointer) as *mut c_void
    })
}

pub unsafe fn demux_module_from_raw(sys: *mut c_void) -> ManuallyDrop<Box<Box<dyn DemuxModule>>> {
    assert!(!sys.is_null());
    ManuallyDrop::new(Box::from_raw(sys as *mut Box<dyn DemuxModule>))
}
