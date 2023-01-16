//! Everything related to demuxing

use std::ffi::CStr;
use std::marker::PhantomData;
use std::mem::ManuallyDrop;
use std::ptr::{self, NonNull};

use vlcrs_core_sys::{demux_t, input_item_node_t, vlc_logger};
use vlcrs_core_sys::{es_out_t, vlc_object_t, vlc_tick_t};

use crate::error::{CoreError, Errno, Result};
use crate::es_out::EsOut;
use crate::input_item::InputItemNode;
use crate::messages::Logger;
use crate::object::VlcObjectRef;
use crate::stream::{Demux, DemuxControl, ReadDirDemux, Stream};
use crate::tick::Tick;

use super::args::ModuleArgs;

/// A representation of a stream module
pub enum DemuxModule<'a> {
    Demux(Box<dyn Demux + 'a>),
    ReadDir(Box<dyn ReadDirDemux + 'a>),
}

/// A opaque representation of the current demux module
#[repr(transparent)]
pub struct ThisDemux<'a>(*mut demux_t, PhantomData<&'a mut vlc_object_t>);

/// Stream filter module
pub trait Module {
    /// Open function for a stream filter module
    fn open<'a>(
        _this_demux: ThisDemux<'a>,
        source: &'a mut Stream,
        es_out: &'a mut EsOut,
        logger: &'a mut Logger,
        _args: &mut ModuleArgs,
    ) -> Result<DemuxModule<'a>>;
}

impl ThisDemux<'_> {
    /// Get the `demux_t.name`
    pub fn name(&mut self) -> Result<&str> {
        // SAFETY: `demux_t` is valid and well-formed
        unsafe { CStr::from_ptr((*self.0).psz_name) }
            .to_str()
            .map_err(CoreError::Utf8Error)
    }

    /// Get the `demux_t.url`
    pub fn url(&mut self) -> Option<Result<&str>> {
        // SAFETY: `demux_t` is valid and well-formed
        let ptr_url = unsafe { (*self.0).psz_url };

        if ptr_url.is_null() {
            return None;
        }

        // SAFETY: `ptr_url` is non-null and points to a valid c-string
        Some(
            unsafe { CStr::from_ptr(ptr_url) }
                .to_str()
                .map_err(CoreError::Utf8Error),
        )
    }

    /// Get the `demux_t.location`
    pub fn location(&mut self) -> Option<Result<&str>> {
        // SAFETY: `demux_t` is valid and well-formed
        let ptr_location = unsafe { (*self.0).psz_location };

        if ptr_location.is_null() {
            return None;
        }

        // SAFETY: `ptr_location` is non-null and points to a valid c-string
        Some(
            unsafe { CStr::from_ptr(ptr_location) }
                .to_str()
                .map_err(CoreError::Utf8Error),
        )
    }

    /// Get the `demux_t.filepath`
    pub fn filepath(&mut self) -> Option<Result<&str>> {
        // SAFETY: `demux_t` is valid and well-formed
        let ptr_filepath = unsafe { (*self.0).psz_filepath };

        if ptr_filepath.is_null() {
            return None;
        }

        // SAFETY: `ptr_filepath` is non-null and points to a valid c-string
        Some(
            unsafe { CStr::from_ptr(ptr_filepath) }
                .to_str()
                .map_err(CoreError::Utf8Error),
        )
    }

    /// Get an ref-`vlc_object_t`
    pub fn object(&mut self) -> VlcObjectRef<'_> {
        VlcObjectRef::from_raw(self.0 as *mut _)
    }
}

/// Generic module open callback for demux_t like steam
///
/// # Safety
///
/// The `object` parameter must point to a valid `demux_t` that has been initiliazed with
/// all the requirement so a any module can hock up to it.
pub unsafe extern "C" fn module_open<T: Module>(object: *mut vlc_object_t) -> i32 {
    let ptr_demux = object as *mut demux_t;

    let this_demux = ThisDemux(ptr_demux, PhantomData);

    // SAFETY: TODO
    let demux_t_ptr_ptr: *mut *mut demux_t = unsafe { &mut (*ptr_demux).s };
    let mut_stream: &'static mut Stream = unsafe {
        demux_t_ptr_ptr
            .cast::<Stream>()
            .as_mut::<'static>()
            .unwrap()
    };

    // SAFETY: TODO
    let es_out_t_ptr_ptr: *mut *mut es_out_t = unsafe { &mut (*ptr_demux).out };
    let mut_es_out: &'static mut EsOut = unsafe {
        es_out_t_ptr_ptr
            .cast::<EsOut>()
            .as_mut::<'static>()
            .unwrap()
    };

    // SAFETY: TODO
    let logger_ptr_ptr: *mut *mut vlc_logger = unsafe { &mut (*object).logger };
    let mut_logger: &'static mut Logger =
        unsafe { logger_ptr_ptr.cast::<Logger>().as_mut::<'static>().unwrap() };

    let mut module_args = ModuleArgs(NonNull::new(object).unwrap());

    match T::open(
        this_demux,
        mut_stream,
        mut_es_out,
        mut_logger,
        &mut module_args,
    ) {
        Ok(demux_module) => unsafe { register(ptr_demux, demux_module) },
        Err(err) => err.to_vlc_errno(),
    }
}

/// Generic module close callback for demux_t
pub unsafe extern "C" fn module_close(_object: *mut vlc_object_t) -> i32 {
    // nothing to do, watch `ops->close` for the actual closing of the demux
    0
}

/// Register the Rust module to it's stream_t
unsafe fn register(this: *mut demux_t, demux_module: DemuxModule<'_>) -> i32 {
    let ptr_demux_t = this;

    match demux_module {
        DemuxModule::Demux(demux) => {
            let sys = Box::into_raw(Box::new(demux));

            // SAFETY: The demux_t has just been created so it is our own.
            unsafe { (*ptr_demux_t).p_sys = sys.cast() };

            static DEMUX_OPS: vlcrs_core_sys::vlc_stream_operations =
                vlcrs_core_sys::vlc_stream_operations {
                    can_seek: Some(pf_control_can_seek::<dyn Demux>),
                    can_control_pace: Some(pf_control_can_control_pace::<dyn Demux>),
                    can_pause: Some(pf_control_can_pause::<dyn Demux>),
                    get_pts_delay: Some(pf_control_get_pts_delay::<dyn Demux>),
                    get_signal: None,
                    get_meta: None,
                    get_type: None,
                    set_pause_state: Some(pf_control_set_pause_state::<dyn Demux>),
                    set_seek_point: Some(pf_control_set_seek_point::<dyn Demux>),
                    set_title: None,
                    close: Some(pf_close::<dyn Demux>),
                    __bindgen_anon_1: vlcrs_core_sys::vlc_stream_operations__bindgen_ty_1 {
                        demux: vlcrs_core_sys::vlc_stream_operations__bindgen_ty_1__bindgen_ty_2 {
                            demux: Some(pf_demux::<dyn Demux>),
                            readdir: None,
                            can_record: None,
                            can_control_rate: None,
                            has_unsupported_meta: None,
                            get_seekpoint: None,
                            get_position: None,
                            get_title: None,
                            get_length: Some(pf_control_get_length::<dyn Demux>),
                            get_time: Some(pf_control_get_time::<dyn Demux>),
                            get_normal_time: None,
                            get_title_info: None,
                            get_fps: None,
                            get_attachments: None,
                            set_position: None,
                            set_time: None,
                            set_next_demux_time: None,
                            set_record_state: None,
                            set_rate: None,
                            set_group_default: None,
                            set_group_all: None,
                            set_group_list: None,
                            set_es: None,
                            set_es_list: None,
                            nav_activate: None,
                            nav_up: None,
                            nav_down: None,
                            nav_left: None,
                            nav_right: None,
                            nav_popup: None,
                            nav_menu: None,
                            filter_enable: None,
                            filter_disable: None,
                            test_and_clear_flags: None,
                        },
                    },
                };

            // SAFETY: The demux_t has just been created so it is our own.
            unsafe { (*ptr_demux_t).ops = &DEMUX_OPS };
        }
        DemuxModule::ReadDir(read_dir) => {
            let sys = Box::into_raw(Box::new(read_dir));

            // SAFETY: The demux_t has just been created so it is our own.
            unsafe { (*ptr_demux_t).p_sys = sys.cast() };

            static DIR_OPS: vlcrs_core_sys::vlc_stream_operations =
                vlcrs_core_sys::vlc_stream_operations {
                    can_seek: None,
                    can_control_pace: None,
                    can_pause: None,
                    get_pts_delay: None,
                    get_signal: None,
                    get_meta: None,
                    get_type: None,
                    set_pause_state: None,
                    set_seek_point: None,
                    set_title: None,
                    close: Some(pf_close::<dyn ReadDirDemux>),
                    __bindgen_anon_1: vlcrs_core_sys::vlc_stream_operations__bindgen_ty_1 {
                        demux: vlcrs_core_sys::vlc_stream_operations__bindgen_ty_1__bindgen_ty_2 {
                            demux: None,
                            readdir: Some(pf_readdir::<dyn ReadDirDemux>),
                            can_record: None,
                            can_control_rate: None,
                            has_unsupported_meta: None,
                            get_seekpoint: None,
                            get_position: None,
                            get_title: None,
                            get_length: None,
                            get_time: None,
                            get_normal_time: None,
                            get_title_info: None,
                            get_fps: None,
                            get_attachments: None,
                            set_position: None,
                            set_time: None,
                            set_next_demux_time: None,
                            set_record_state: None,
                            set_rate: None,
                            set_group_default: None,
                            set_group_all: None,
                            set_group_list: None,
                            set_es: None,
                            set_es_list: None,
                            nav_activate: None,
                            nav_up: None,
                            nav_down: None,
                            nav_left: None,
                            nav_right: None,
                            nav_popup: None,
                            nav_menu: None,
                            filter_enable: None,
                            filter_disable: None,
                            test_and_clear_flags: None,
                        },
                    },
                };

            // SAFETY: The demux_t has just been created so it is our own.
            unsafe { (*ptr_demux_t).ops = &DIR_OPS };
        }
    }

    Errno::SUCCESS.to_vlc_errno()
}

unsafe extern "C" fn pf_close<D: ?Sized>(stream: *mut demux_t) {
    let sys = unsafe { (*stream).p_sys } as *mut Box<D>;
    let super_sys = unsafe { Box::from_raw(sys) };
    drop(super_sys);
    unsafe { (*stream).p_sys = ptr::null_mut() };
}

unsafe extern "C" fn pf_demux<D: Demux + ?Sized>(stream: *mut demux_t) -> libc::c_int {
    let sys = unsafe { (*stream).p_sys } as *mut Box<D>;
    let res = D::demux(unsafe { &mut *sys });
    match res {
        Ok(_) => 1, // VLC_DEMUXER_SUCCESS
        Err(err) => err.to_vlc_errno().into(),
    }
}

unsafe extern "C" fn pf_readdir<D: ReadDirDemux + ?Sized>(
    demux: *mut demux_t,
    input_item_node: *mut input_item_node_t,
) -> i32 {
    let sys = unsafe { (*demux).p_sys } as *mut Box<D>;
    let mut iin = unsafe {
        ManuallyDrop::new(InputItemNode::from_ptr(NonNull::new_unchecked(
            input_item_node,
        )))
    };
    let res = D::read_dir(unsafe { &mut *sys }, &mut iin);
    match res {
        Ok(_) => 0,
        Err(err) => err.to_vlc_errno(),
    }
}

unsafe extern "C" fn pf_control_get_time<C: DemuxControl + ?Sized>(
    stream: *mut demux_t,
) -> vlc_tick_t {
    let sys = unsafe { (*stream).p_sys } as *mut Box<C>;
    C::time(unsafe { &mut *sys }).0
}

unsafe extern "C" fn pf_control_get_length<C: DemuxControl + ?Sized>(
    stream: *mut demux_t,
) -> vlc_tick_t {
    let sys = unsafe { (*stream).p_sys } as *mut Box<C>;
    let length = C::length(unsafe { &mut *sys });
    length.unwrap_or(Tick::ZERO).0
}

unsafe extern "C" fn pf_control_get_pts_delay<C: DemuxControl + ?Sized>(
    stream: *mut demux_t,
    ptr_pts_delay: *mut vlc_tick_t,
) -> libc::c_int {
    let sys = unsafe { (*stream).p_sys } as *mut Box<C>;
    let pts_delay = C::pts_delay(unsafe { &mut *sys });
    unsafe { *ptr_pts_delay = pts_delay.0 }
    0
}

unsafe extern "C" fn pf_control_can_seek<C: DemuxControl + ?Sized>(stream: *mut demux_t) -> bool {
    let sys = unsafe { (*stream).p_sys } as *mut Box<C>;
    C::can_seek(unsafe { &mut *sys })
}

unsafe extern "C" fn pf_control_can_pause<C: DemuxControl + ?Sized>(stream: *mut demux_t) -> bool {
    let sys = unsafe { (*stream).p_sys } as *mut Box<C>;
    C::can_pause(unsafe { &mut *sys })
}

unsafe extern "C" fn pf_control_can_control_pace<C: DemuxControl + ?Sized>(
    stream: *mut demux_t,
) -> bool {
    let sys = unsafe { (*stream).p_sys } as *mut Box<C>;
    C::can_control_pace(unsafe { &mut *sys })
}

unsafe extern "C" fn pf_control_set_pause_state<C: DemuxControl + ?Sized>(
    stream: *mut demux_t,
    pause_state: bool,
) -> libc::c_int {
    let sys = unsafe { (*stream).p_sys } as *mut Box<C>;
    match C::set_pause_state(unsafe { &mut *sys }, pause_state) {
        Ok(()) => 0,
        Err(err) => err.to_vlc_errno(),
    }
}

unsafe extern "C" fn pf_control_set_seek_point<C: DemuxControl + ?Sized>(
    stream: *mut demux_t,
    seek_point: i32,
) -> libc::c_int {
    let sys = unsafe { (*stream).p_sys } as *mut Box<C>;
    match C::set_seek_point(unsafe { &mut *sys }, seek_point) {
        Ok(()) => 0,
        Err(err) => err.to_vlc_errno(),
    }
}
