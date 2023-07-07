//! FourCC.

use core::slice;
use std::ptr::NonNull;

use libc::c_char;
use vlcrs_core_sys::{
    vlc_chroma_description_t, vlc_chroma_description_t__bindgen_ty_1,
    vlc_fourcc_AreUVPlanesSwapped, vlc_fourcc_GetChromaDescription, vlc_fourcc_GetCodec,
    vlc_fourcc_GetCodecAudio, vlc_fourcc_GetCodecFromString, vlc_fourcc_GetFallback,
    vlc_fourcc_GetRGBFallback, vlc_fourcc_GetYUVFallback, vlc_fourcc_IsYUV, vlc_fourcc_t,
};

use crate::rational::Rational;

/// FourCC ("four-character code") is a sequence of four bytes (typically ASCII)
/// used to uniquely identify data formats.
#[derive(Copy, Clone, Debug, PartialEq, Eq)]
#[doc(alias = "vlc_fourcc_t")]
#[repr(transparent)]
pub struct FourCC(vlc_fourcc_t);

/// Chroma related informations
#[doc(alias = "vlc_chroma_description_t")]
#[derive(Debug)]
pub struct ChromaDescription(NonNull<vlc_chroma_description_t>);

/// Chroma size description
#[derive(Debug)]
#[repr(transparent)]
pub struct ChromaDescriptionSize(vlc_chroma_description_t__bindgen_ty_1);

/// FourCC macro-creation
///
/// ```
/// # use vlcrs_core::fourcc;
/// let mp4 = fourcc!('m', 'p', '4', 'v');
/// assert_eq!(mp4.as_u32(), 0x7634706d);
/// ```
#[macro_export]
macro_rules! fourcc {
    ($a:expr, $b:expr, $c:expr, $d:expr) => {{
        $crate::fourcc::FourCC::from_u32(u32::from_ne_bytes([
            $a as u8, $b as u8, $c as u8, $d as u8,
        ]))
    }};
}

impl From<vlc_fourcc_t> for FourCC {
    fn from(value: vlc_fourcc_t) -> Self {
        FourCC(value)
    }
}

impl Into<vlc_fourcc_t> for FourCC {
    fn into(self) -> vlc_fourcc_t {
        self.0
    }
}

impl FourCC {
    // Only exist because we cannot yet define `From::from` as const.
    #[doc(hidden)]
    pub const fn from_u32(value: u32) -> FourCC {
        FourCC(value)
    }

    /// Return the inner representation of a fourcc
    #[inline]
    pub fn as_u32(&self) -> u32 {
        self.0
    }

    /// Check if the current FourCC is of [Self::VLC_CODEC_UNKNOWN]
    #[inline]
    pub fn is_unknown(&self) -> bool {
        *self == Self::VLC_CODEC_UNKNOWN
    }

    /// It returns the codec associated to a fourcc within an ES category.
    ///
    /// Returns [Self::VLC_CODEC_UNKNOWN] if unknow.
    #[inline]
    #[doc(alias = "vlc_fourcc_GetCodec")]
    pub fn get_codec(cat: i32, fourcc: FourCC) -> FourCC {
        // SAFETY: Allways valid to call and always return a fourcc
        FourCC(unsafe { vlc_fourcc_GetCodec(cat, fourcc.0) })
    }

    /// It converts the given fourcc to an audio codec when possible.
    ///
    /// Returns [Self::VLC_CODEC_UNKNOWN] if unknow.
    #[inline]
    #[doc(alias = "vlc_fourcc_GetCodecAudio")]
    pub fn get_codec_audio(fourcc: FourCC, bits: i32) -> FourCC {
        // SAFETY: Allways valid to call and always return a fourcc
        FourCC(unsafe { vlc_fourcc_GetCodecAudio(fourcc.0, bits) })
    }

    /// It converts the given fourcc to an audio codec when possible.
    ///
    /// Returns [Self::VLC_CODEC_UNKNOWN] if unknow.
    #[doc(alias = "vlc_fourcc_GetCodecAudio")]
    pub fn get_codec_from_string(cat: i32, s: Option<&str>) -> FourCC {
        let buf = if let Some(s) = s {
            if s.len() == 4 {
                let mut bytes = s.bytes();
                // SAFETY: We just verified that the lenght is indeed 4
                let buf: [c_char; 4] = unsafe {
                    [
                        bytes.next().unwrap_unchecked() as c_char,
                        bytes.next().unwrap_unchecked() as c_char,
                        bytes.next().unwrap_unchecked() as c_char,
                        bytes.next().unwrap_unchecked() as c_char,
                    ]
                };
                buf.as_ptr()
            } else {
                std::ptr::null()
            }
        } else {
            std::ptr::null()
        };

        // SAFETY: Allways valid to call and always return a fourcc
        FourCC(unsafe { vlc_fourcc_GetCodecFromString(cat, buf) })
    }

    /// Returns a slice of YUV fourccs in decreasing priority order for the given chroma
    pub fn get_yuv_fallback(&self) -> Option<&[FourCC]> {
        // SAFETY: Allways valid to call and always return a list fourcc
        let fallback = unsafe { vlc_fourcc_GetYUVFallback(self.0) };

        // SAFETY: The pointer points to a valid fourcc list
        unsafe { fourcc_list(fallback) }
    }

    /// Returns a slice of RGB fourccs in decreasing priority order for the given chroma
    pub fn get_rgb_fallback(&self) -> Option<&[FourCC]> {
        // SAFETY: Allways valid to call and always return a list fourcc
        let fallback = unsafe { vlc_fourcc_GetRGBFallback(self.0) };

        // SAFETY: The pointer points to a valid fourcc list
        unsafe { fourcc_list(fallback) }
    }

    /// Returns a slice of YUV fourccs in decreasing priority order for the given chroma
    pub fn get_fallback(&self) -> Option<&[FourCC]> {
        // SAFETY: Allways valid to call and always return a list fourcc
        let fallback = unsafe { vlc_fourcc_GetFallback(self.0) };

        // SAFETY: The pointer points to a valid fourcc list
        unsafe { fourcc_list(fallback) }
    }

    /// Returns true if the given fourcc is YUV and false otherwise.
    #[inline]
    pub fn is_yuv(&self) -> bool {
        // SAFETY: All fine!
        unsafe { vlc_fourcc_IsYUV(self.0) }
    }

    /// Returns true if the two fourccs are equivalent if their U&V planes are swapped
    #[inline]
    pub fn are_uvplanes_swapped(&self, rhs: FourCC) -> bool {
        // SAFETY: All fine!
        unsafe { vlc_fourcc_AreUVPlanesSwapped(self.0, rhs.0) }
    }

    /// Get the chroma description of the fourcc
    #[inline]
    pub fn get_chroma_description(&self) -> Option<ChromaDescription> {
        // SAFETY: The fourcc is valid (I hope)
        let ptr = unsafe { vlc_fourcc_GetChromaDescription(self.0) };
        NonNull::new(ptr as *mut _).map(ChromaDescription)
    }
}

impl ChromaDescription {
    /// Get the plane count
    #[inline]
    pub fn plane_count(&self) -> u32 {
        // SAFETY: The ptr is non-null and points to a valid vlc_chroma_description_t
        unsafe { (*self.0.as_ptr()).plane_count }
    }

    /// Get the pixel size
    #[inline]
    pub fn pixel_size(&self) -> u32 {
        // SAFETY: The ptr is non-null and points to a valid vlc_chroma_description_t
        unsafe { (*self.0.as_ptr()).pixel_size }
    }

    /// Get the pixel bits
    #[inline]
    pub fn pixel_bits(&self) -> u32 {
        // SAFETY: The ptr is non-null and points to a valid vlc_chroma_description_t
        unsafe { (*self.0.as_ptr()).pixel_bits }
    }

    /// Get the sizes for the chroma
    #[inline]
    pub fn sizes(&self) -> [ChromaDescriptionSize; 4] {
        (unsafe { (*self.0.as_ptr()).p }).map(ChromaDescriptionSize)
    }
}

impl ChromaDescriptionSize {
    /// Get the width
    #[inline]
    pub fn width(&self) -> Rational {
        self.0.w.into()
    }

    /// Get the height
    #[inline]
    pub fn height(&self) -> Rational {
        self.0.h.into()
    }
}

/// Convert a fourcc C "vector" to a optional Rust slice.
unsafe fn fourcc_list(fallback: *const u32) -> Option<&'static [FourCC]> {
    if fallback.is_null() {
        None
    } else {
        let mut i = 0;
        let mut ptr = fallback;

        // SAFETY: The pointer is valid and not out-of-bounds
        while unsafe { *ptr != 0 } {
            // SAFETY: The pointer is valid and not out-of-bounds
            ptr = unsafe { ptr.add(1) };
            i += 1;
        }

        // SAFETY: The pointer is valid, not-null and the lenght as just been calculated
        Some(unsafe { slice::from_raw_parts(fallback as *const FourCC, i) })
    }
}

impl FourCC {
    vlcrs_core_sys::fourcc_consts!();
}
