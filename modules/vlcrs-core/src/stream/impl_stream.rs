use std::ffi::CStr;
use std::io::Read;
use std::ptr::{self, NonNull};

use crate::error::{cvp, cvr, cvt, Result};
use crate::stream::block::NativeBlock;
use crate::tick::Tick;
use vlcrs_core_sys::{
    stream_t, vlc_stream_Block, vlc_stream_CanFastSeek, vlc_stream_CanPace, vlc_stream_CanPause,
    vlc_stream_CanSeek, vlc_stream_Eof, vlc_stream_GetContentType, vlc_stream_GetPtsDelay,
    vlc_stream_GetSize, vlc_stream_Peek, vlc_stream_Read, vlc_stream_ReadBlock,
    vlc_stream_ReadPartial, vlc_stream_Seek, vlc_stream_SetPauseState, vlc_stream_Tell,
};

/// A stream
///
/// Nearly an owned one but not quite.
#[repr(transparent)]
#[doc(alias = "stream_t")]
#[derive(Debug)]
pub struct Stream(pub(crate) NonNull<stream_t>);

impl Stream {
    /// Create a stream fom a pointer
    ///
    /// # Safety
    ///  - The pointer must point to a valid and correctly initialized `stream_t`
    pub unsafe fn from_ptr(ptr: NonNull<stream_t>) -> Stream {
        Stream(ptr)
    }

    /// Get the input if there is one
    pub fn input_stream(&mut self) -> Option<&mut Stream> {
        // SAFETY: The self-pointer is non-null and valid to deference
        unsafe { (*self.0.as_ptr()).s.cast::<Stream>().as_mut() }
    }

    /// Reads data from a byte stream.
    ///
    /// This function always waits for the requested number of bytes, unless a fatal
    /// error is encountered or the end-of-stream is reached first.
    #[doc(alias = "vlc_stream_Read")]
    pub fn read(&mut self, buf: &mut [u8]) -> Result<usize> {
        // SAFETY: The self-pointer is non-null, the buf-pointer is non-null and te buf is
        // large enough
        cvr(unsafe { vlc_stream_Read(self.0.as_ptr(), buf.as_mut_ptr().cast(), buf.len() as _) })
    }

    /// Reads partial data from a byte stream.
    ///
    /// This function waits until some data is available for reading from the
    /// stream, a fatal error is encountered or the end-of-stream is reached.
    ///
    /// Unlike vlc_stream_Read(), this function does not wait for the full requested
    /// bytes count. It can return a short count even before the end of the stream
    /// and in the absence of any error.
    #[doc(alias = "vlc_stream_ReadPartial")]
    pub fn read_partial(&mut self, buf: &mut [u8]) -> Result<usize> {
        // SAFETY: The self-pointer is non-null, the buf-pointer is non-null and te buf is
        // large enough
        cvr(unsafe {
            vlc_stream_ReadPartial(self.0.as_ptr(), buf.as_mut_ptr().cast(), buf.len() as _)
        })
    }

    /// Reads a data block from a byte stream.
    ///
    /// This function dequeues the next block of data from the byte stream. The
    /// byte stream back-end decides on the size of the block; the caller cannot
    /// make any assumption about it.
    #[doc(alias = "vlc_stream_ReadBlock")]
    pub fn read_block(&mut self) -> Result<NativeBlock> {
        // SAFETY: The self-pointer is non-null, the buf-pointer is non-null and te buf is
        // large enough
        cvp(unsafe { vlc_stream_ReadBlock(self.0.as_ptr()) })
            .map(|ptr| unsafe { NativeBlock::from_ptr(ptr) })
    }

    /// Reads a data block from a byte stream with a specific size.
    #[doc(alias = "vlc_stream_Block")]
    pub fn block(&mut self, len: usize) -> Result<NativeBlock> {
        // SAFETY: The self-pointer is non-null, the buf-pointer is non-null and te buf is
        // large enough
        cvp(unsafe { vlc_stream_Block(self.0.as_ptr(), len as _) })
            .map(|ptr| unsafe { NativeBlock::from_ptr(ptr) })
    }

    /// Peeks at data from a byte stream.
    ///
    /// This function buffers for the requested number of bytes, waiting if
    /// necessary. Then it stores a pointer to the buffer. Unlike vlc_stream_Read()
    /// or vlc_stream_Block(), this function does not modify the stream read offset.
    #[doc(alias = "vlc_stream_Peek")]
    pub fn peek(&mut self, len: usize) -> Result<BufPeek<'_>> {
        let mut buf: *const u8 = ptr::null_mut();

        // SAFETY: The self-pointer is non-null, the buf-pointer is non-null and te buf is
        // large enough
        cvr(unsafe { vlc_stream_Peek(self.0.as_ptr(), &mut buf, len as _) }).map(|len| {
            debug_assert!(!buf.is_null());
            // SAFETY: len > 0 and no-errors
            BufPeek(unsafe { std::slice::from_raw_parts(buf, len) }, self)
        })
    }

    /// Sets the current stream position.
    ///
    /// This function changes the read offset within a stream, if the stream
    /// supports seeking. In case of error, the read offset is not changed.
    ///
    /// @note It is possible (but not useful) to seek past the end of a stream.
    #[doc(alias = "vlc_stream_Seek")]
    pub fn seek(&mut self, offset: u64) -> Result<()> {
        // SAFETY: The self-pointer is non-null
        cvt(unsafe { vlc_stream_Seek(self.0.as_ptr(), offset) })
    }

    /// Tells the current stream position.
    ///
    /// This function tells the current read offset (in bytes) from the start of
    /// the start of the stream.
    /// @note The read offset may be larger than the stream size, either because of
    /// a seek past the end, or because the stream shrank asynchronously.
    #[doc(alias = "vlc_stream_Tell")]
    pub fn tell(&self) -> u64 {
        // SAFETY: The self-pointer is non-null and valid.
        // The function also cannot fail.
        unsafe { vlc_stream_Tell(self.0.as_ptr()) }
    }

    /// Checks for end of stream.
    ///
    /// Checks if the last attempt to reads data from the stream encountered the
    /// end of stream before the attempt could be fully satisfied.
    /// The value is initially false, and is reset to false by vlc_stream_Seek().
    #[doc(alias = "vlc_stream_Eof")]
    pub fn eof(&self) -> bool {
        // SAFETY: The self-pointer is non-null and valid.
        unsafe { vlc_stream_Eof(self.0.as_ptr()) }
    }

    /// Get the size of a stream.
    #[doc(alias = "vlc_stream_GetSize")]
    pub fn size(&mut self) -> Result<u64> {
        let mut size = 0u64;

        // SAFETY: The stream pointer is valid and points to a valid stream
        cvt(unsafe { vlc_stream_GetSize(self.0.as_ptr(), &mut size) })?;

        Ok(size)
    }

    /// Get the pts-delay
    #[doc(alias = "STREAM_GET_PTS_DELAY")]
    pub fn pts_delay(&mut self) -> Tick {
        let mut tick: vlcrs_core_sys::vlc_tick_t = 0;

        // SAFETY: The stream pointer is valid and points to a valid stream
        unsafe { vlc_stream_GetPtsDelay(self.0.as_ptr(), &mut tick) };

        Tick(tick)
    }

    /// Get the `Content-Type` of a stream.
    #[doc(alias = "vlc_stream_GetContentType")]
    pub fn content_type(&mut self) -> Result<String> {
        let mut ptr: *mut libc::c_char = ptr::null_mut();

        // SAFETY: The stream pointer is valid and points to a valid stream
        cvt(unsafe { vlc_stream_GetContentType(self.0.as_ptr(), &mut ptr) })?;

        // SAFETY: We checked above the result code, so ptr should be non-null
        let c_string = unsafe { CStr::from_ptr(ptr) }.to_owned();

        // SAFETY: We just copied the data above so no need to keep it anymore
        unsafe { libc::free(ptr.cast()) };

        Ok(c_string.into_string().map_err(|e| e.utf8_error())?)
    }

    /// Get the `Mime-Type` of a stream.
    #[doc(alias = "vlc_stream_GetMimeType")]
    pub fn mime_type(&mut self) -> Result<Option<String>> {
        self.content_type()
            .map(|ct| ct.split_once(';').map(|mt| mt.0.to_owned()))
    }

    /// Can the stream seek
    #[doc(alias = "STREAM_CAN_SEEK")]
    pub fn can_seek(&mut self) -> bool {
        // SAFETY: The stream pointer is valid and points to a valid stream
        unsafe { vlc_stream_CanSeek(self.0.as_ptr()) }
    }

    /// Can the stream fast-seek
    #[doc(alias = "STREAM_CAN_FASTSEEK")]
    pub fn can_fast_seek(&mut self) -> bool {
        // SAFETY: The stream pointer is valid and points to a valid stream
        unsafe { vlc_stream_CanFastSeek(self.0.as_ptr()) }
    }

    /// Can the stream pause
    #[doc(alias = "STREAM_CAN_PAUSE")]
    pub fn can_pause(&mut self) -> bool {
        // SAFETY: The stream pointer is valid and points to a valid stream
        unsafe { vlc_stream_CanPause(self.0.as_ptr()) }
    }

    /// Can the stream control pace
    #[doc(alias = "STREAM_CAN_CONTROL_PACE")]
    pub fn can_control_pace(&mut self) -> bool {
        // SAFETY: The stream pointer is valid and points to a valid stream
        unsafe { vlc_stream_CanPace(self.0.as_ptr()) }
    }

    /// Set the pause state
    #[doc(alias = "STREAM_SET_PAUSE_STATE")]
    pub fn set_pause_state(&mut self, state: bool) -> Result<()> {
        // SAFETY: The stream pointer is valid and points to a valid stream
        cvt(unsafe { vlc_stream_SetPauseState(self.0.as_ptr(), state) })
    }
}

impl Read for Stream {
    fn read(&mut self, buf: &mut [u8]) -> std::io::Result<usize> {
        Stream::read_partial(self, buf)
            .map_err(|_| std::io::Error::new(std::io::ErrorKind::Other, "unknown for now"))
    }
}

/// A borrowed buffer from a Stream
#[derive(Debug)]
pub struct BufPeek<'a>(&'a [u8], &'a Stream);

impl BufPeek<'_> {
    /// Get the u8 buffer
    pub fn buf(&self) -> &[u8] {
        self.0
    }

    /// Get a immutable [Stream] from where the buffer comes from
    // It is important to not return here a &mut since calling other mutable
    // functions could invalidate the current buffer.
    pub fn stream(&self) -> &Stream {
        self.1
    }
}
