use core::slice;
use std::mem::ManuallyDrop;
use std::ptr::NonNull;

use vlcrs_core_sys::{vlc_frame_Alloc, vlc_frame_Realloc, vlc_frame_Release, vlc_frame_t};

use crate::error::{cvp, Result};
use crate::tick::Tick;

use super::block::NativeBlock;

/// A frame
#[doc(alias = "vlc_frame_t")]
#[derive(Debug)]
pub struct Frame(pub(crate) NonNull<vlc_frame_t>);

#[derive(Copy, Clone, PartialEq, Eq)]
#[repr(u32)]
#[doc(alias = "VLC_FRAME")]
pub enum FrameFlag {
    Discontinuity = vlcrs_core_sys::VLC_FRAME_FLAG_DISCONTINUITY,
    TypeI = vlcrs_core_sys::VLC_FRAME_FLAG_TYPE_I,
    TypeP = vlcrs_core_sys::VLC_FRAME_FLAG_TYPE_P,
    TypeB = vlcrs_core_sys::VLC_FRAME_FLAG_TYPE_B,
    TypePB = vlcrs_core_sys::VLC_FRAME_FLAG_TYPE_PB,
    Header = vlcrs_core_sys::VLC_FRAME_FLAG_HEADER,
    EndOfSequence = vlcrs_core_sys::VLC_FRAME_FLAG_END_OF_SEQUENCE,
    Scrambled = vlcrs_core_sys::VLC_FRAME_FLAG_SCRAMBLED,
    Preroll = vlcrs_core_sys::VLC_FRAME_FLAG_PREROLL,
    Corrupted = vlcrs_core_sys::VLC_FRAME_FLAG_CORRUPTED,
    AuEnd = vlcrs_core_sys::VLC_FRAME_FLAG_AU_END,
    TopFieldFirst = vlcrs_core_sys::VLC_FRAME_FLAG_TOP_FIELD_FIRST,
    BottomFieldFirst = vlcrs_core_sys::VLC_FRAME_FLAG_BOTTOM_FIELD_FIRST,
    SingleField = vlcrs_core_sys::VLC_FRAME_FLAG_SINGLE_FIELD,
}

impl Drop for Frame {
    fn drop(&mut self) {
        // SAFETY: We owned the frame, the pointer is non-null and the pointer points to a valid
        // vlc_frame_t
        unsafe { vlc_frame_Release(self.0.as_ptr()) }
    }
}

impl From<NativeBlock> for Frame {
    fn from(value: NativeBlock) -> Self {
        let me = ManuallyDrop::new(value);
        Frame(me.0)
    }
}

impl Frame {
    /// Create a [Frame] from a pointer
    ///
    /// # Safety
    ///
    /// The pointer must be aligned, must be pointing to a valid `vlc_frame_t` and
    /// the pointee must be properly initiliazed.
    pub(crate) unsafe fn from_ptr(ptr: NonNull<vlc_frame_t>) -> Frame {
        Frame(ptr)
    }

    /// Transform the wrapper to it's "original" pointer
    #[allow(dead_code)]
    pub(crate) fn into_raw_ptr(self) -> *mut vlc_frame_t {
        let md = ManuallyDrop::new(self);
        md.0.as_ptr()
    }

    /// Create a new frame with a given size
    #[doc(alias = "vlc_frame_Alloc")]
    pub fn alloc(size: usize) -> Result<Frame> {
        // SAFETY: No precondition, safe to call
        let ptr = unsafe { vlc_frame_Alloc(size as _) };

        let ptr = cvp(ptr)?;

        // SAFETY: The pointer is non-null and points to vlc_frame_t
        Ok(unsafe { Frame::from_ptr(ptr) })
    }

    /// Reallocates a frame.
    ///
    /// This function expands, shrinks or moves a data frame.
    /// In many cases, this function can return without any memory allocation by
    /// reusing spare buffer space. Otherwise, a new frame is created and data is
    /// copied.
    #[doc(alias = "vlc_frame_Realloc")]
    pub fn realloc(self, pre: isize, size: usize) -> Result<Frame> {
        // SAFETY: No precondition, safe to call
        let ptr = unsafe { vlc_frame_Realloc(self.0.as_ptr(), pre as _, size as _) };

        let ptr = cvp(ptr)?;

        // SAFETY: The pointer is non-null and points to vlc_frame_t
        Ok(unsafe { Frame::from_ptr(ptr) })
    }

    /// Get the pts
    pub fn pts(&self) -> Tick {
        // SAFETY: The pointer is non-null and the data is hopefully fully initialized
        Tick(unsafe { (*self.0.as_ptr()).i_pts })
    }

    /// Get the dst
    pub fn dts(&self) -> Tick {
        // SAFETY: The pointer is non-null and the data is hopefully fully initialized
        Tick(unsafe { (*self.0.as_ptr()).i_dts })
    }

    /// Get the lenght
    pub fn length(&self) -> Tick {
        // SAFETY: The pointer is non-null and the data is hopefully fully initialized
        Tick(unsafe { (*self.0.as_ptr()).i_length })
    }

    /// Get the number of samples
    pub fn samples(&self) -> u32 {
        // SAFETY: The pointer is non-null and the data is hopefully fully initialized
        (unsafe { (*self.0.as_ptr()).i_nb_samples }) as _
    }

    /// Set the pts
    pub fn set_pts(&mut self, pts: Tick) {
        // SAFETY: The pointer is non-null and points to a vlc_frame_t
        unsafe { (*self.0.as_ptr()).i_pts = pts.0 }
    }

    /// Set the pts
    pub fn set_dts(&mut self, dts: Tick) {
        // SAFETY: The pointer is non-null and points to a vlc_frame_t
        unsafe { (*self.0.as_ptr()).i_dts = dts.0 }
    }

    /// Set the pts
    pub fn set_length(&mut self, length: Tick) {
        // SAFETY: The pointer is non-null and points to a vlc_frame_t
        unsafe { (*self.0.as_ptr()).i_length = length.0 }
    }

    /// Set the pts
    pub fn set_samples(&mut self, samples: u32) {
        // SAFETY: The pointer is non-null and points to a vlc_frame_t
        unsafe { (*self.0.as_ptr()).i_nb_samples = samples as _ }
    }

    /// Get the payload
    // FIXME: Is it safe to expose potentially undef memory as a slice of u8 and not
    // as a slice of MaybeUninit<u8> ?
    pub fn payload(&self) -> &[u8] {
        // SAFETY: The pointer is non-null and points to a block_t
        unsafe {
            slice::from_raw_parts(
                (*self.0.as_ptr()).p_buffer.cast::<u8>(),
                // SAFETY: Having a buffer larger than a pointer is unlikely
                (*self.0.as_ptr()).i_buffer as _,
            )
        }
    }

    /// Get a mutable variation of the payload
    // FIXME: Is it safe to expose potentially undef memory as a slice of u8 and not
    // as a slice of MaybeUninit<u8> ?
    pub fn payload_mut(&mut self) -> &mut [u8] {
        // SAFETY: The pointer is non-null and points to a block_t
        unsafe {
            slice::from_raw_parts_mut(
                (*self.0.as_ptr()).p_buffer.cast::<u8>(),
                // SAFETY: Having a buffer larger than a pointer is unlikely
                (*self.0.as_ptr()).i_buffer as _,
            )
        }
    }

    /// Tell if a flag has been set
    pub fn has_flag(&self, flag: FrameFlag) -> bool {
        // SAFETY: The pointer is non-null and points to a vlc_frame_t
        (unsafe { (*self.0.as_ptr()).i_flags }) & (flag as u32) == (flag as u32)
    }

    /// Set a flag
    pub fn set_flag(&mut self, flag: FrameFlag) {
        // SAFETY: The pointer is non-null and points to a vlc_frame_t
        unsafe { (*self.0.as_ptr()).i_flags = (*self.0.as_ptr()).i_flags | (flag as u32) }
    }

    /// Unset a flag
    pub fn unset_flag(&mut self, flag: FrameFlag) {
        // SAFETY: The pointer is non-null and points to a vlc_frame_t
        unsafe { (*self.0.as_ptr()).i_flags = (*self.0.as_ptr()).i_flags & !(flag as u32) }
    }
}
