use crate::vlc_core::*;
use core::mem::ManuallyDrop;
use core::ptr::null_mut;
use libc::c_void;

#[no_mangle]
#[must_use]
pub extern "C" fn vlcrs_playlist_callbacks_on_current_index_changed(
    playlist: *mut vlc_playlist_t,
    index: i64,
    data: *mut libc::c_void,
) {
    let data = unsafe { &mut *(data as *mut intf_thread_t) };
    let playlist = unsafe { &mut *playlist };
    let mut playlist_callback =
        unsafe { playlist_callback_from_raw(data.p_sys as *mut libc::c_void) };
    playlist_callback.playlist_on_current_index_changed(playlist, index)
}

pub trait PlaylistCallbacks {
    fn playlist_on_current_index_changed(&mut self, _playlist: &mut vlc_playlist_t, _index: i64) {}
}

pub fn playlist_callback_to_raw(
    stream_filter_module: Option<impl PlaylistCallbacks>,
) -> *mut c_void {
    stream_filter_module.map_or(null_mut(), |sys| {
        /* We need a trait object because when we receive a pointer from C, we
         * don't know the concrete Rust type, we just now that it implements
         * StreamFilterModule */
        let trait_object: Box<dyn PlaylistCallbacks> = Box::new(sys);
        /* A trait objet is a fat pointer, so box it one more time to get a thin
         * pointer, convertible to a raw pointer without loss of information */
        let thin_pointer = Box::new(trait_object);

        Box::into_raw(thin_pointer) as *mut c_void
    })
}

pub unsafe fn playlist_callback_from_raw(
    sys: *mut c_void,
) -> ManuallyDrop<Box<Box<dyn PlaylistCallbacks>>> {
    assert!(!sys.is_null());
    ManuallyDrop::new(Box::from_raw(sys as *mut Box<dyn PlaylistCallbacks>))
}
