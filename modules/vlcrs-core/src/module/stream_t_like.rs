//! Everything related to

use std::ffi::{CStr, CString};
use std::marker::PhantomData;
use std::mem::ManuallyDrop;
use std::ptr::{self, NonNull};
use std::slice;

use vlcrs_core_sys::{input_item_node_t, stream_t, vlc_frame_t};
use vlcrs_core_sys::{vlc_object_t, vlc_tick_t};

use crate::error::{CoreError, Errno, Result};
use crate::input_item::InputItemNode;
use crate::object::VlcObjectRef;
use crate::stream::{ReadBlockStream, ReadDirStream, ReadStream, StreamControl};

/// A representation of a stream module
pub enum StreamModule<'a> {
    Read(Box<dyn ReadStream + 'a>),
    ReadBlock(Box<dyn ReadBlockStream + 'a>),
    ReadDir(Box<dyn ReadDirStream + 'a>),
}

/// A opaque representation of the current stream module
#[repr(transparent)]
pub struct ThisStream<'a>(
    pub(super) *mut stream_t,
    pub(super) PhantomData<&'a mut vlc_object_t>,
);

impl ThisStream<'_> {
    /// Get the `stream_t.name`
    pub fn name(&mut self) -> Result<&str> {
        // SAFETY: `stream_t` is valid and well-formed
        unsafe { CStr::from_ptr((*self.0).psz_name) }
            .to_str()
            .map_err(CoreError::Utf8Error)
    }

    /// Get the `stream_t.url`
    pub fn url(&mut self) -> Option<Result<&str>> {
        // SAFETY: `stream_t` is valid and well-formed
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

    /// Get the `stream_t.location`
    pub fn location(&mut self) -> Option<Result<&str>> {
        // SAFETY: `stream_t` is valid and well-formed
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

    /// Get the `stream_t.filepath`
    pub fn filepath(&mut self) -> Option<Result<&str>> {
        // SAFETY: `stream_t` is valid and well-formed
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

/// Register the Rust module to it's stream_t
pub(super) unsafe fn register(this: *mut stream_t, stream_module: StreamModule<'_>) -> i32 {
    let ptr_stream_t = this;

    match stream_module {
        StreamModule::Read(read_stream) => {
            let sys = Box::into_raw(Box::new(read_stream));

            // SAFETY: `ptr_stream_t` is non-null
            unsafe { (*ptr_stream_t).p_sys = sys.cast() };

            static READ_OPS: vlcrs_core_sys::vlc_stream_operations =
                vlcrs_core_sys::vlc_stream_operations {
                    can_seek: Some(pf_control_can_seek::<dyn ReadStream>),
                    can_control_pace: Some(pf_control_can_control_pace::<dyn ReadStream>),
                    can_pause: Some(pf_control_can_pause::<dyn ReadStream>),
                    get_pts_delay: Some(pf_control_get_pts_delay::<dyn ReadStream>),
                    get_signal: None,
                    get_meta: None,
                    get_type: None,
                    set_pause_state: Some(pf_control_set_pause_state::<dyn ReadStream>),
                    set_seek_point: Some(pf_control_set_seek_point::<dyn ReadStream>),
                    set_title: None,
                    close: Some(pf_close::<dyn ReadStream>),
                    __bindgen_anon_1: vlcrs_core_sys::vlc_stream_operations__bindgen_ty_1 {
                        stream: vlcrs_core_sys::vlc_stream_operations__bindgen_ty_1__bindgen_ty_1 {
                            read: Some(pf_read::<dyn ReadStream>),
                            seek: Some(pf_seek::<dyn ReadStream>),
                            block: None,
                            readdir: None,
                            can_fastseek: Some(pf_control_can_fastseek::<dyn ReadStream>),
                            get_seekpoint: None,
                            get_title: None,
                            get_size: Some(pf_control_get_size::<dyn ReadStream>),
                            get_title_info: None,
                            get_content_type: Some(pf_control_get_content_type::<dyn ReadStream>),
                            get_tags: None,
                            get_private_id_state: None,
                            set_record_state: None,
                            set_private_id_state: None,
                            set_private_id_ca: None,
                        },
                    },
                };

            unsafe { (*ptr_stream_t).ops = &READ_OPS };
        }
        StreamModule::ReadBlock(read_block_stream) => {
            let sys = Box::into_raw(Box::new(read_block_stream));

            // SAFETY: `ptr_stream_t` is non-null
            unsafe { (*ptr_stream_t).p_sys = sys.cast() };

            static BLOCK_OPS: vlcrs_core_sys::vlc_stream_operations =
                vlcrs_core_sys::vlc_stream_operations {
                    can_seek: Some(pf_control_can_seek::<dyn ReadBlockStream>),
                    can_control_pace: Some(pf_control_can_control_pace::<dyn ReadBlockStream>),
                    can_pause: Some(pf_control_can_pause::<dyn ReadBlockStream>),
                    get_pts_delay: Some(pf_control_get_pts_delay::<dyn ReadBlockStream>),
                    get_signal: None,
                    get_meta: None,
                    get_type: None,
                    set_pause_state: Some(pf_control_set_pause_state::<dyn ReadBlockStream>),
                    set_seek_point: Some(pf_control_set_seek_point::<dyn ReadBlockStream>),
                    set_title: None,
                    close: Some(pf_close::<dyn ReadBlockStream>),
                    __bindgen_anon_1: vlcrs_core_sys::vlc_stream_operations__bindgen_ty_1 {
                        stream: vlcrs_core_sys::vlc_stream_operations__bindgen_ty_1__bindgen_ty_1 {
                            read: None,
                            seek: Some(pf_seek_block::<dyn ReadBlockStream>),
                            block: Some(pf_block::<dyn ReadBlockStream>),
                            readdir: None,
                            can_fastseek: Some(pf_control_can_fastseek::<dyn ReadBlockStream>),
                            get_seekpoint: None,
                            get_title: None,
                            get_size: Some(pf_control_get_size::<dyn ReadBlockStream>),
                            get_title_info: None,
                            get_content_type: Some(
                                pf_control_get_content_type::<dyn ReadBlockStream>,
                            ),
                            get_tags: None,
                            get_private_id_state: None,
                            set_record_state: None,
                            set_private_id_state: None,
                            set_private_id_ca: None,
                        },
                    },
                };

            unsafe { (*ptr_stream_t).ops = &BLOCK_OPS };
        }
        StreamModule::ReadDir(read_dir_stream) => {
            let sys = Box::into_raw(Box::new(read_dir_stream));

            // SAFETY: `ptr_stream_t` is non-null
            unsafe { (*ptr_stream_t).p_sys = sys.cast() };

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
                    close: Some(pf_close::<dyn ReadDirStream>),
                    __bindgen_anon_1: vlcrs_core_sys::vlc_stream_operations__bindgen_ty_1 {
                        stream: vlcrs_core_sys::vlc_stream_operations__bindgen_ty_1__bindgen_ty_1 {
                            read: None,
                            block: None,
                            seek: None,
                            readdir: Some(pf_readdir::<dyn ReadDirStream>),
                            can_fastseek: None,
                            get_seekpoint: None,
                            get_title: None,
                            get_size: None,
                            get_title_info: None,
                            get_content_type: None,
                            get_tags: None,
                            get_private_id_state: None,
                            set_record_state: None,
                            set_private_id_state: None,
                            set_private_id_ca: None,
                        },
                    },
                };

            unsafe { (*ptr_stream_t).ops = &DIR_OPS };
        }
    }

    Errno::SUCCESS.to_vlc_errno()
}

unsafe extern "C" fn pf_close<T: ?Sized>(stream: *mut stream_t) {
    let sys = unsafe { (*stream).p_sys } as *mut Box<T>;
    let super_sys = unsafe { Box::from_raw(sys) };
    drop(super_sys);
    unsafe { (*stream).p_sys = ptr::null_mut() };
}

unsafe extern "C" fn pf_read<R: ReadStream + ?Sized>(
    stream: *mut stream_t,
    buf: *mut libc::c_void,
    len: usize,
) -> isize {
    let sys = unsafe { (*stream).p_sys } as *mut Box<R>;
    let buffer = unsafe { slice::from_raw_parts_mut(buf as *mut _, len) };
    let res = R::read(unsafe { &mut *sys }, buffer);
    match res {
        Ok(bytes) => bytes as _,
        Err(err) => err.to_vlc_errno() as _,
    }
}

unsafe extern "C" fn pf_seek<R: ReadStream + ?Sized>(
    stream: *mut stream_t,
    pos: u64,
) -> libc::c_int {
    let sys = unsafe { (*stream).p_sys } as *mut Box<R>;
    let res = R::seek(unsafe { &mut *sys }, pos);
    match res {
        Ok(_) => 0,
        Err(err) => err.to_vlc_errno(),
    }
}

unsafe extern "C" fn pf_block<B: ReadBlockStream + ?Sized>(
    stream: *mut stream_t,
    eof: *mut bool,
) -> *mut vlc_frame_t {
    let sys = unsafe { (*stream).p_sys } as *mut Box<B>;
    let res = B::block(unsafe { &mut *sys });
    match res {
        Ok(owned_block) => owned_block.into_raw_ptr(),
        Err(err) => {
            if matches!(err, crate::error::CoreError::Eof) {
                unsafe { *eof = true };
            }
            ptr::null_mut()
        }
    }
}

unsafe extern "C" fn pf_seek_block<B: ReadBlockStream + ?Sized>(
    stream: *mut stream_t,
    pos: u64,
) -> libc::c_int {
    let sys = unsafe { (*stream).p_sys } as *mut Box<B>;
    let res = B::seek(unsafe { &mut *sys }, pos);
    match res {
        Ok(_) => 0,
        Err(err) => err.to_vlc_errno(),
    }
}

unsafe extern "C" fn pf_readdir<D: ReadDirStream + ?Sized>(
    stream: *mut stream_t,
    input_item_node: *mut input_item_node_t,
) -> i32 {
    let sys = unsafe { (*stream).p_sys } as *mut Box<D>;
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

unsafe extern "C" fn pf_control_can_seek<C: StreamControl + ?Sized>(stream: *mut stream_t) -> bool {
    let sys = unsafe { (*stream).p_sys } as *mut Box<C>;
    C::can_seek(unsafe { &mut *sys })
}

unsafe extern "C" fn pf_control_can_control_pace<C: StreamControl + ?Sized>(
    stream: *mut stream_t,
) -> bool {
    let sys = unsafe { (*stream).p_sys } as *mut Box<C>;
    C::can_control_pace(unsafe { &mut *sys })
}

unsafe extern "C" fn pf_control_can_pause<C: StreamControl + ?Sized>(
    stream: *mut stream_t,
) -> bool {
    let sys = unsafe { (*stream).p_sys } as *mut Box<C>;
    C::can_pause(unsafe { &mut *sys })
}

unsafe extern "C" fn pf_control_get_pts_delay<C: StreamControl + ?Sized>(
    stream: *mut stream_t,
    ptr_pts_delay: *mut vlc_tick_t,
) -> libc::c_int {
    let sys = unsafe { (*stream).p_sys } as *mut Box<C>;
    let pts_delay = C::pts_delay(unsafe { &mut *sys });
    unsafe { *ptr_pts_delay = pts_delay.0 }
    0
}

unsafe extern "C" fn pf_control_set_pause_state<C: StreamControl + ?Sized>(
    stream: *mut stream_t,
    pause_state: bool,
) -> libc::c_int {
    let sys = unsafe { (*stream).p_sys } as *mut Box<C>;
    match C::set_pause_state(unsafe { &mut *sys }, pause_state) {
        Ok(()) => 0,
        Err(err) => err.to_vlc_errno(),
    }
}

unsafe extern "C" fn pf_control_set_seek_point<C: StreamControl + ?Sized>(
    stream: *mut stream_t,
    seek_point: i32,
) -> libc::c_int {
    let sys = unsafe { (*stream).p_sys } as *mut Box<C>;
    match C::set_seek_point(unsafe { &mut *sys }, seek_point) {
        Ok(()) => 0,
        Err(err) => err.to_vlc_errno(),
    }
}

unsafe extern "C" fn pf_control_can_fastseek<C: StreamControl + ?Sized>(
    stream: *mut stream_t,
) -> bool {
    let sys = unsafe { (*stream).p_sys } as *mut Box<C>;
    C::can_fast_seek(unsafe { &mut *sys })
}

unsafe extern "C" fn pf_control_get_size<C: StreamControl + ?Sized>(
    stream: *mut stream_t,
    ptr_size: *mut u64,
) -> libc::c_int {
    let sys = unsafe { (*stream).p_sys } as *mut Box<C>;
    match C::size(unsafe { &mut *sys }) {
        Ok(size) => {
            unsafe { *ptr_size = size }
            0
        }
        Err(err) => err.to_vlc_errno(),
    }
}

unsafe extern "C" fn pf_control_get_content_type<C: StreamControl + ?Sized>(
    stream: *mut stream_t,
    ct: *mut *mut libc::c_char,
) -> libc::c_int {
    let sys = unsafe { (*stream).p_sys } as *mut Box<C>;
    unsafe { *ct = ptr::null_mut() };
    match C::content_type(unsafe { &mut *sys }) {
        Ok(content_type) => match CString::new(content_type) {
            Ok(content_type) => {
                unsafe { *ct = content_type.into_raw() }
                0
            }
            Err(_) => Errno::NO_MEM.to_vlc_errno(),
        },
        Err(err) => err.to_vlc_errno(),
    }
}
