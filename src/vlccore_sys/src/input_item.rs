use crate::vlc_core::{VLC_ENOMEM, *};
use crate::vlc_core_sys::*;
use std::ffi::{CStr, CString};
use std::ptr::null_mut;

pub type CInputItem = input_item_t;
pub type CInputItemNode = input_item_node_t;

impl CInputItemNode {
    /// Add a new child node to this parrent node that will point to this subitem
    ///
    /// ```rust
    /// let mut input_item_node: CInputItemNode = existing_input_item_node;
    /// let input_item: Box<CInputItem> = existing_input_item;
    /// input_item_node.append_item(input_item);
    /// ```
    pub fn append_item(&mut self, mut item: Box<CInputItem>) {
        unsafe {
            input_item_node_AppendItem(self, item.as_mut());
        }
        item.release();
    }
}

impl CInputItem {
    /// Create a new CInputItem wrapper with the provided information.
    ///
    /// ```rust
    /// let uri = "file:///home/enulp/Downloads/file.flac";
    /// let name = "audio_name";
    /// let i_duration = existing_i_duration;
    /// let i_type = existing_i_type;
    /// let i_net = existing_i_net;
    /// let new_CInputItem: ManuallyDrop<CInputItem> = CInputItem::new(uri, name, i_type, i_net);
    /// ```
    pub fn new(
        uri: &str,
        name: &str,
        i_duration: vlc_tick_t,
        i_type: input_item_type_e,
        i_net: input_item_net_type,
    ) -> Box<Self> {
        let c_uri = CString::new(uri).expect("CString::new failed");
        let c_name = CString::new(name).expect("CString::new failed");
        unsafe {
            Box::from_raw(input_item_NewExt(
                c_uri.as_ptr(),
                c_name.as_ptr(),
                i_duration,
                i_type,
                i_net,
            ))
        }
    }

    /// This function allows to add option to CInputItem.
    ///
    /// ```rust
    /// let item: CInputItem = existing_item;
    /// item.add_option(VLC_INPUT_OPTION_TRUSTED as i32, ":start-time=0");
    /// ```
    pub fn add_option(&mut self, option_type: &str, flags: u32) {
        let add_option_return;
        let c_option_type = CString::new(option_type).expect("CString::new failed");
        unsafe {
            let c_option_type_p = c_option_type.as_ptr();
            add_option_return = input_item_AddOption(self, c_option_type_p, flags);
        }
        match add_option_return as i32 {
            VLC_ENOMEM => {
                panic!("input_item_AddOption: VLC_ENOMEM");
            }
            _ => {}
        }
    }

    /// This function allows to add options to CInputItem.
    ///
    /// ```rust
    /// let mut item: CInputItem = existing_item;
    /// let mut options = Vec::new();
    /// options.push(format!(":start-time={}", 0));
    /// options.push(format!(":stop-time={}", 1));
    /// item.add_options(options, VLC_INPUT_OPTION_TRUSTED as i32);
    /// ```
    pub fn add_options(&mut self, options_type: Vec<String>, i_flags: u32) {
        for i in options_type {
            self.add_option(&i, i_flags);
        }
    }

    /// This function allows to set meta to the CInputItem
    ///
    /// ```rust
    /// let mut item: CInputItem = existing_item;
    /// item.set_meta(Album, "meta")
    /// ```
    pub fn set_meta(&mut self, meta_type: vlc_meta_type_t, val: &str) {
        let c_val = CString::new(val).expect("CString: new failed");
        unsafe {
            let pointer = c_val.as_ptr();
            input_item_SetMeta(self, meta_type, pointer);
        }
    }

    /// This function allows to set meta to the CInputItem
    ///
    /// ```rust
    /// let mut item: CInputItem = existing_item;
    /// let album_meta = item.get_meta(Album);
    /// ```

    pub fn get_meta(&mut self, meta_type: vlc_meta_type_t) -> String {
        let c_buf = unsafe { input_item_GetMeta(self, meta_type) };
        if c_buf as *mut libc::c_void == null_mut() {
            return String::new();
        }
        let c_str: &CStr = unsafe { CStr::from_ptr(c_buf) };
        let str_slice: &str = c_str.to_str().unwrap();
        str_slice.to_owned()
    }

    pub fn release(self: Box<Self>) {
        let item = Box::leak(self);
        unsafe {
            input_item_Release(item);
        }
    }

    /// # Warning: not tested TODO
    /// This function allows adding a slave to an existing input item
    pub fn add_slave(&mut self, slave: Box<input_item_slave>) -> i32 {
        unsafe { input_item_AddSlave(self, Box::into_raw(slave)) }
    }

    /// # Warning: not tested TODO
    pub fn has_error_when_reading(&mut self) -> bool {
        unsafe { input_item_HasErrorWhenReading(self) }
    }

    /// # Warning: not tested TODO
    pub fn meta_match(&mut self, meta_type: vlc_meta_type_t, val: &str) -> bool {
        let c_val = CString::new(val).expect("CString: new failed");
        unsafe {
            let pointer = c_val.as_ptr();
            input_item_MetaMatch(self, meta_type, pointer)
        }
    }

    /// # Warning: not tested TODO
    pub fn get_meta_locked(&mut self, meta_type: vlc_meta_type_t) -> String {
        let c_buf = unsafe { input_item_GetMetaLocked(self, meta_type) };
        if c_buf as *mut libc::c_void == null_mut() {
            return String::new();
        }
        let c_str: &CStr = unsafe { CStr::from_ptr(c_buf) };
        let str_slice: &str = c_str.to_str().unwrap();
        str_slice.to_owned()
    }

    /// # Warning: not tested TODO
    pub fn get_name(&mut self) -> String {
        let c_buf = unsafe { input_item_GetName(self) };
        if c_buf as *mut libc::c_void == null_mut() {
            return String::new();
        }
        let c_str: &CStr = unsafe { CStr::from_ptr(c_buf) };
        let str_slice: &str = c_str.to_str().unwrap();
        str_slice.to_owned()
    }

    /// # Warning: not tested TODO
    pub fn get_title_fb_name(&mut self) -> String {
        let c_buf = unsafe { input_item_GetTitleFbName(self) };
        if c_buf as *mut libc::c_void == null_mut() {
            return String::new();
        }
        let c_str: &CStr = unsafe { CStr::from_ptr(c_buf) };
        let str_slice: &str = c_str.to_str().unwrap();
        str_slice.to_owned()
    }

    /// # Warning: not tested TODO
    pub fn get_uri(&mut self) -> String {
        let c_buf = unsafe { input_item_GetURI(self) };
        if c_buf as *mut libc::c_void == null_mut() {
            return String::new();
        }
        let c_str: &CStr = unsafe { CStr::from_ptr(c_buf) };
        let str_slice: &str = c_str.to_str().unwrap();
        str_slice.to_owned()
    }

    /// # Warning: not tested TODO
    pub fn get_now_playing_fb(&mut self) -> String {
        let c_buf = unsafe { input_item_GetNowPlayingFb(self) };
        if c_buf as *mut libc::c_void == null_mut() {
            return String::new();
        }
        let c_str: &CStr = unsafe { CStr::from_ptr(c_buf) };
        let str_slice: &str = c_str.to_str().unwrap();
        str_slice.to_owned()
    }

    pub fn get_duration(&mut self) -> vlc_tick_t {
        unsafe { input_item_GetDuration(self) }
    }

    /// # Warning: not tested TODO
    pub fn set_duration(&mut self, duration: vlc_tick_t) {
        unsafe { input_item_SetDuration(self, duration) }
    }

    pub fn is_preparsed(&mut self) -> bool {
        unsafe { input_item_IsPreparsed(self) }
    }

    /// # Warning: not tested TODO
    pub fn is_art_fetched(&mut self) -> bool {
        unsafe { input_item_IsArtFetched(self) }
    }

    /// # Warning: not tested TODO
    pub fn get_info(&mut self, cat: &str, name: &str) -> String {
        let c_cat = CString::new(cat).expect("CString: new failed").as_ptr();
        let c_name = CString::new(name).expect("CString: new failed").as_ptr();
        let c_buf = unsafe { input_item_GetInfo(self, c_cat, c_name) };
        if c_buf as *mut libc::c_void == null_mut() {
            return String::new();
        }
        let c_str: &CStr = unsafe { CStr::from_ptr(c_buf) };
        let str_slice: &str = c_str.to_str().unwrap();
        str_slice.to_owned()
    }

    /// # Warning: not tested TODO
    pub fn del_info(&mut self, cat: &str, name: &str) -> i32 {
        let c_cat = CString::new(cat).expect("CString: new failed").as_ptr();
        let c_name = CString::new(name).expect("CString: new failed").as_ptr();
        unsafe { input_item_DelInfo(self, c_cat, c_name) }
    }

    /// # Warning: not tested TODO
    pub fn replace_infos(&mut self, info: &mut info_category_t) {
        unsafe { input_item_ReplaceInfos(self, info) }
    }

    /// # Warning: not tested TODO
    pub fn merge_infos(&mut self, info: &mut info_category_t) {
        unsafe { input_item_MergeInfos(self, info) }
    }
}
