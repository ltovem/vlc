use crate::block::*;
use crate::vlc_core::*;
use crate::vlc_core_sys::*;
use libc::c_void;
use std::ffi::CStr;
use std::io::{Error, ErrorKind, Read, Result};
use std::ptr::null_mut;
use std::slice::from_raw_parts;

pub type CStream = stream_t;

impl Read for CStream {
    fn read(&mut self, buf: &mut [u8]) -> Result<usize> {
        let c_buf = buf.as_mut_ptr() as *mut c_void;
        let bytes_read = unsafe { vlc_stream_Read(self, c_buf, buf.len() as u64) };
        if bytes_read < 0 {
            Err(Error::new(
                ErrorKind::Other,
                "Error while reading from stream",
            ))
        } else {
            Ok(bytes_read as usize)
        }
    }
}

impl<'a> CStream {
    pub fn block(&mut self, size: u64) -> Box<CBlock> {
        let block: *mut CBlock = unsafe { vlc_stream_Block(self, size) };
        if block == null_mut() {
            panic!("CStream: block: allocation failed");
        } else {
            unsafe { Box::from_raw(block) }
        }
    }
    pub fn peek(&mut self, size: u64) -> Result<&'a [u8]> {
        let mut buf = 0 as *const u8;
        let bytes_read = unsafe { vlc_stream_Peek(self, &mut buf, size) };
        if bytes_read < 0 {
            Err(Error::new(
                ErrorKind::Other,
                "Error while peeking from stream",
            ))
        } else {
            let ret = unsafe { from_raw_parts(buf, bytes_read as usize) };
            Ok(ret)
        }
    }

    /// Return stream size
    ///
    /// ```rust
    /// let stream: CStream = existing_stream;
    /// let stream_size = stream.len();
    /// ```
    pub fn len(&mut self) -> std::result::Result<u64, Error> {
        let size: *mut __va_list_tag = null_mut();
        const STREAM_GET_SIZE: i32 = 6;
        let result = unsafe { vlc_stream_vaControl(self, STREAM_GET_SIZE, size) };
        if result != 0 {
            return Err(Error::new(
                ErrorKind::Other,
                "Could not determine stream size",
            ));
        }
        Ok(size as u64)
    }

    /// Tells the current stream position.
    ///
    /// This function tells the current read offset (in bytes) from the start of
    /// the start of the stream.
    ///
    /// # Note
    ///
    /// The read offset may be larger than the stream size, either because of a
    /// seek past the end, or because the stream shrank asynchronously.
    ///
    /// ```rust
    /// let mut stream: CStream = existing_stream;
    /// let position = stream.tell();
    /// ```
    pub fn tell(&self) -> u64 {
        unsafe { vlc_stream_Tell(self) }
    }

    /// Sets the current stream position
    ///
    /// This function changes the read offset within a stream, if the change
    /// supports seeking. In case of error, the read offset is not change.
    ///
    /// # Note
    /// It is possible (but not useful) to seek past the end of a stream.
    /// ```rust
    /// let mut stream: CStream = existing_stream;
    /// let seek = stream.seek(69);
    /// ```
    pub fn seek(&mut self, offset: u64) -> i32 {
        unsafe { vlc_stream_Seek(self, offset) }
    }

    pub fn mime_type(&mut self) -> Option<String> {
        self.content_type().map(|content_type| {
            let index = content_type.chars().position(|c| c == ';').unwrap();
            let mime_type: String = content_type[..index].to_owned();
            mime_type
        })
    }

    pub fn content_type(&mut self) -> Option<String> {
        // FIXME: STREAM_GET_CONTENT_TYPE does not have a fixed value
        const STREAM_GET_CONTENT_TYPE: i32 = 0;
        let result: *mut __va_list_tag = null_mut();
        unsafe {
            if vlc_stream_vaControl(self, STREAM_GET_CONTENT_TYPE, result) != 0 {
                return None;
            } else if result.is_null() {
                return None;
            }
            // FIXME: Free the C pointer received but retain copy of data in Rust
            CStr::from_ptr(result as *const i8)
                .to_str()
                .map(|s| s.to_string())
                .ok()
        }
    }

    pub fn s(&self) -> &mut Self {
        unsafe { &mut *self.s }
    }

    pub fn es_out(&self) -> &mut es_out_t {
        unsafe { &mut *self.out }
    }
}
