use crate::vlc_core::*;
use crate::vlc_core_sys::var_Inherit;
use std::ffi::*;

pub type CObject = vlc_object_t;

impl CObject {
    pub fn inherit_string(&mut self, name: String) -> Option<String> {
        let mut val: vlc_value_t = unsafe { std::mem::zeroed() };
        if unsafe {
            var_Inherit(
                self,
                CString::new(name).expect("CString new failed").as_ptr(),
                VLC_VAR_STRING as i32,
                &mut val,
            )
        } != 0
        {
            return None;
        }
        Some(unsafe { CStr::from_ptr(val.psz_string).to_str().unwrap().to_owned() })
    }
}
