use crate::vlc_core_sys::*;
use std::ffi::{CStr, CString};

pub struct CUrl {}

impl CUrl {
    /// Encodes a URI component.
    ///
    /// Subtitutes URI-unsafe, URI delimiters and non-ASCII characters into theirs
    /// URI-encoded URI-safe representation. Sea also IETF RFC3938 §2.
    ///
    /// ```rust
    /// let audio_file = "ANZX-13044.flac"
    /// let audio_file_uri = Url::encode(audio_file);
    /// assert_eq!(audio_file_uri, "ANZX-13044.flac");
    /// ```
    pub fn encode(audio_file: &str) -> String {
        let c_audio_file = CString::new(audio_file).expect("CString:: new failed");
        unsafe {
            let c_audio_file_p = c_audio_file.as_ptr();
            let uri_encode = vlc_uri_encode(c_audio_file_p);
            if uri_encode.is_null() {
                return "".to_string();
            }
            let rust_uri_encode = CStr::from_ptr(uri_encode).to_string_lossy().into_owned();
            libc::free(uri_encode as *mut libc::c_void);
            rust_uri_encode
        }
    }

    /// Resolves an URI reference.
    ///
    /// Resolves an URI reference relatative to a base URI.
    /// If the reference is an absolute URI, then this function simply returns a
    /// copy of the URI reference.
    ///
    /// ```rust
    /// let demux_url = "file:///home/enulp/Downloads/ANZX-13044.cue";
    /// let url = Url::resolve(demux_url);
    /// assert_eq!(url, "file:///home/enulp/Downloads/ANZX-13044.flac");
    /// ```
    pub fn resolve(base: &str, reference: &str) -> String {
        let c_base = CString::new(base).expect("CString::new failed");
        let c_reference = CString::new(reference).expect("CString::new failed");
        unsafe {
            let c_base_p = c_base.as_ptr();
            let c_refence_p = c_reference.as_ptr();
            let uri_resolve = vlc_uri_resolve(c_base_p, c_refence_p);
            if uri_resolve.is_null() {
                return "".to_string();
            }
            let rust_uri_resolve = CStr::from_ptr(uri_resolve).to_string_lossy().into_owned();
            libc::free(uri_resolve as *mut libc::c_void);
            rust_uri_resolve
        }
    }
}
