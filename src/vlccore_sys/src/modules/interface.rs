use crate::vlc_core::*;
use core::mem::ManuallyDrop;
use libc::*;
use std::ptr;

pub trait InterfaceModule {
    fn close(&mut self) {}
}

pub fn interface_module_to_raw(interface_module: Option<impl InterfaceModule>) -> *mut c_void {
    interface_module.map_or(ptr::null_mut(), |sys| {
        /* We need a trait object because when we receive a pointer from C, we
         * don't know the concrete Rust type, we just now that it implements
         * InterfaceModule */
        let trait_object: Box<dyn InterfaceModule> = Box::new(sys);
        /* A trait objet is a fat pointer, so box it one more time to get a thin
         * pointer, convertible to a raw pointer without loss of information */
        let thin_pointer = Box::new(trait_object);

        Box::into_raw(thin_pointer) as *mut c_void
    })
}

pub unsafe fn interface_module_from_raw(
    sys: *mut intf_sys_t,
) -> ManuallyDrop<Box<Box<dyn InterfaceModule>>> {
    assert!(!sys.is_null());
    ManuallyDrop::new(Box::from_raw(sys as *mut Box<dyn InterfaceModule>))
}
