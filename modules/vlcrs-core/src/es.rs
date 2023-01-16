//! `es`

use std::mem::MaybeUninit;

use vlcrs_core_sys::{
    audio_format_t, es_format_Clean, es_format_Copy, es_format_Init, es_format_IsSimilar,
    es_format_t, subs_format_t, video_format_t,
};

use crate::fourcc::FourCC;

/// `es_format_t`
#[doc(alias = "es_format_t")]
#[repr(transparent)]
pub struct EsFormat(es_format_t);

/// `video_format_t`
#[doc(alias = "video_format_t")]
pub type VideoFormat = video_format_t;
// #[repr(transparent)]
// pub struct VideoFormat(video_format_t);

/// `audio_format_t`
#[doc(alias = "audio_format_t")]
pub type AudioFormat = audio_format_t;
// #[repr(transparent)]
// pub struct AudioFormat(audio_format_t);

/// `subs_format_t`
#[doc(alias = "subs_format_t")]
pub type SubsFormat = subs_format_t;
// #[repr(transparent)]
// pub struct SubsFormat(subs_format_t);

pub use vlcrs_core_sys::es_format_category_e as EsFormatCategory;

impl EsFormat {
    /// Create and initialize a [EsFormat].
    #[doc(alias = "es_format_Init")]
    pub fn new(cat: EsFormatCategory, fourcc: FourCC) -> EsFormat {
        let mut es_format = MaybeUninit::uninit();

        // SAFETY: All the arguments are point to properly initialized struct,
        // except for es_format which isn't but has the right layout.
        unsafe { es_format_Init(es_format.as_mut_ptr(), cat as _, fourcc.into()) };

        // SAFETY: Was properly initialized above by `es_format_Init`
        EsFormat(unsafe { es_format.assume_init() })
    }

    /// Test similary with another [Self].
    ///
    /// NOTE: All descriptive fields are ignored.
    #[doc(alias = "es_format_IsSimilar")]
    pub fn is_similar_to(&self, other: &EsFormat) -> bool {
        // SAFETY: Both arguments are properly initialized
        unsafe { es_format_IsSimilar(&self.0, &other.0) }
    }

    pub(crate) fn as_ptr(&self) -> *const es_format_t {
        &self.0
    }

    /// Set the original [FourCC]
    pub fn set_original_fourcc(&mut self, orig: FourCC) {
        self.0.i_original_fourcc = orig.into()
    }

    /// Access the audio format
    pub fn audio(&self) -> Option<&AudioFormat> {
        if self.0.i_cat != EsFormatCategory::AUDIO_ES {
            return None;
        }

        // SAFETY: We verified above that the category is indeed an audio
        Some(unsafe { &self.0.__bindgen_anon_1.__bindgen_anon_1.audio })
    }

    /// Mutable access to the audio format
    pub fn audio_mut(&mut self) -> Option<&mut AudioFormat> {
        if self.0.i_cat != EsFormatCategory::AUDIO_ES {
            return None;
        }

        // SAFETY: We verified above that the category is indeed an audio
        Some(unsafe { &mut self.0.__bindgen_anon_1.__bindgen_anon_1.audio })
    }

    /// Access the video format
    pub fn video(&self) -> Option<&VideoFormat> {
        if self.0.i_cat != EsFormatCategory::VIDEO_ES {
            return None;
        }

        // SAFETY: We verified above that the category is indeed an video
        Some(unsafe { &self.0.__bindgen_anon_1.video })
    }

    /// Mutable access to the audio format
    pub fn video_mut(&mut self) -> Option<&mut VideoFormat> {
        if self.0.i_cat != EsFormatCategory::VIDEO_ES {
            return None;
        }

        // SAFETY: We verified above that the category is indeed an video
        Some(unsafe { &mut self.0.__bindgen_anon_1.video })
    }
}

impl Clone for EsFormat {
    fn clone(&self) -> Self {
        let mut es_format = MaybeUninit::uninit();

        // SAFETY: All the arguments are point to properly initialized struct,
        // except for es_format which isn't but has the right layout.
        unsafe { es_format_Copy(es_format.as_mut_ptr(), &self.0) };

        // SAFETY: Was properly initialized above by `es_format_Init`
        EsFormat(unsafe { es_format.assume_init() })
    }
}

impl Drop for EsFormat {
    fn drop(&mut self) {
        // SAFETY: No one else is holding to it so cleaning it is fine.
        unsafe { es_format_Clean(&mut self.0) }
    }
}
