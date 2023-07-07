use core::slice;
use std::mem::ManuallyDrop;
use std::ops::{Deref, DerefMut};
use std::ptr::NonNull;

use vlcrs_core_sys::{block_t, vlc_frame_New, vlc_frame_Release, vlc_frame_callbacks};

// TODO: for Block:
//  - handle next
//
// TODO: for Frame:
//  - add Frame and NativeFrame
//  - impl From Frame <-> NativeFrame
//  - add From Block and From NativeBlock

/// A pure Rust abstraction for a `block_t`
#[doc(alias = "block_t")]
#[derive(Debug, Clone)]
pub struct Block {
    pub data: Vec<u8>,
    pub next: Option<Box<Block>>,
}

/// An opaque `block_t` C type
#[derive(Debug)]
pub struct NativeBlock(pub(crate) NonNull<block_t>);

impl From<Block> for NativeBlock {
    fn from(block: Block) -> Self {
        const CBS: vlc_frame_callbacks = vlc_frame_callbacks {
            free: Some(rust_block_free),
        };

        unsafe extern "C" fn rust_block_free(ptr: *mut block_t) {
            debug_assert!(!ptr.is_null());

            // SAFETY: The pointer is non-null and should point to a Rust `Vec`
            let cap_len = unsafe { (*ptr).i_buffer };

            // SAFETY: The pointer is non-null and should point to a Rust `Vec`
            unsafe { Vec::from_raw_parts((*ptr).p_buffer.cast::<u8>(), cap_len, cap_len) };
        }

        let mut block = ManuallyDrop::new(block);
        block.data.shrink_to_fit();
        let (ptr, len) = (block.data.as_mut_ptr(), block.data.len());

        // SAFETY: The callbacks correctly deal with the fact it is owned Rust Vec
        // and the len and capacity are now the same (since the block doesn't hold the capacity)
        let new_block = unsafe { vlc_frame_New(&CBS, ptr.cast::<libc::c_void>(), len) };

        NativeBlock(NonNull::new(new_block).unwrap())
    }
}

impl From<NativeBlock> for Block {
    fn from(value: NativeBlock) -> Self {
        Block {
            data: {
                let mut v = Vec::with_capacity(value.len());
                v.extend_from_slice(&value);
                v
            },
            next: None,
        }
    }
}

impl Deref for Block {
    type Target = Vec<u8>;

    fn deref(&self) -> &Self::Target {
        &self.data
    }
}

impl DerefMut for Block {
    fn deref_mut(&mut self) -> &mut Self::Target {
        &mut self.data
    }
}

impl Deref for NativeBlock {
    type Target = [u8];

    fn deref(&self) -> &Self::Target {
        // SAFETY: We owned the frame, the pointer is non-null and the pointer points to a valid
        // vlc_frame_t
        unsafe {
            slice::from_raw_parts(
                (*self.0.as_ptr()).p_buffer.cast::<u8>(),
                // SAFETY: Having a buffer larger than a pointer is unlikely
                (*self.0.as_ptr()).i_buffer as _,
            )
        }
    }
}

impl Drop for NativeBlock {
    fn drop(&mut self) {
        // SAFETY: We owned the frame, the pointer is non-null and the pointer points to a valid
        // vlc_frame_t
        unsafe { vlc_frame_Release(self.0.as_ptr()) }
    }
}

impl Block {
    /// Create an empty [Block]
    pub fn new() -> Block {
        Block {
            data: Vec::new(),
            next: None,
        }
    }

    /// Create a [Block] with a given capacity
    pub fn with_capacity(cap: usize) -> Block {
        Block {
            data: Vec::with_capacity(cap),
            next: None,
        }
    }
}

impl NativeBlock {
    /// Create a [NativeBlock] from a pointer
    ///
    /// # Safety
    ///
    /// The pointer must be aligned, must be pointing to a valid `block_t` and
    /// the pointee must be properly initiliazed.
    pub(crate) unsafe fn from_ptr(ptr: NonNull<block_t>) -> NativeBlock {
        NativeBlock(ptr)
    }

    /// Transform the wrapper to it's "original" pointer
    pub(crate) fn into_raw_ptr(self) -> *mut block_t {
        let md = ManuallyDrop::new(self);
        md.0.as_ptr()
    }
}
