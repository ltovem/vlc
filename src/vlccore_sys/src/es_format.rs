use crate::fourcc::*;
use crate::vlc_core::{ES_PRIORITY_SELECTABLE_MIN, FIELD_OF_VIEW_DEGREES_DEFAULT, *};
use std::ffi::CString;
use std::os::raw::{c_int, c_uint};
use std::ptr;

pub type CEsFormat = es_format_t;

#[derive(Debug)]
pub enum Priority {
    NotSelectable,
    NotSelected,
    Value(c_int),
}

#[derive(Debug)]
pub struct Language {
    pub name: CString,
    pub description: Option<CString>,
}

#[derive(Debug)]
pub struct Ratio {
    pub num: c_uint,
    pub den: c_uint,
}

#[derive(Debug)]
pub struct RGBMask {
    pub rmask: u32,
    pub gmask: u32,
    pub bmask: u32,
}

#[derive(Debug, Copy, Clone)]
pub struct ReplayGainItem {
    pub peak: Option<f32>,
    pub gain: Option<f32>,
}

#[derive(Debug, Copy, Clone)]
pub struct ReplayGain {
    pub track: ReplayGainItem,
    pub album: ReplayGainItem,
}

#[derive(Debug)]
pub struct Chan;

impl Chan {
    pub const CENTER: u16 = 0x1;
    pub const LEFT: u16 = 0x2;
    pub const RIGHT: u16 = 0x4;
    pub const REARCENTER: u16 = 0x10;
    pub const REARLEFT: u16 = 0x20;
    pub const REARRIGHT: u16 = 0x40;
    pub const MIDDLELEFT: u16 = 0x100;
    pub const MIDDLERIGHT: u16 = 0x200;
    pub const LFE: u16 = 0x100;
}

#[derive(Debug)]
pub struct VideoFormat {
    pub chroma: FourCC,
    pub width: c_uint,
    pub height: c_uint,
    pub x_offset: c_uint,
    pub y_offset: c_uint,
    pub visible_width: c_uint,
    pub visible_height: c_uint,
    pub bits_per_pixel: c_uint,
    pub sar: Ratio,
    pub frame_rate: Ratio,
    pub rgb_mask: Option<RGBMask>,
    pub orientation: video_orientation_t,
    pub primaries: video_color_primaries_t,
    pub transfer: video_transfer_func_t,
    pub color_space: video_color_space_t,
    pub color_range: video_color_range_t,
    pub chroma_location: video_chroma_location_t,
    // ignored: palette, multiview, projection, cubemap...
}

#[derive(Debug)]
pub struct AudioFormat {
    pub format: FourCC,
    pub rate: c_uint,
    pub physical_channels: u16,
    pub chan_mode: u16,
    pub channel_type: audio_channel_type_t,
    pub bytes_per_frame: c_uint,
    pub frame_length: c_uint,
    pub bits_per_sample: c_uint,
    pub block_align: c_uint,
    pub channels: u8,

    // replay gain in the audio format for simplicity
    pub replay_gain: ReplayGain,
}

#[derive(Debug)]
pub struct SubsFormat {
    pub encoding: CString,
    pub x_origin: c_int,
    pub y_origin: c_int,
    // TODO
}

#[derive(Debug)]
pub enum Format {
    Video(VideoFormat),
    Audio(AudioFormat),
    Subs(SubsFormat),
}

#[derive(Debug)]
pub struct EsFormat {
    pub cat: es_format_category_e,
    pub codec: FourCC,
    pub original_fourcc: FourCC,
    pub id: Option<c_int>,
    pub group: Option<c_int>,
    pub priority: Priority,
    pub language: Option<Language>,
    pub extra_languages: Vec<Language>,
    pub format: Format,
    pub bitrate: c_uint,
    pub profile: c_int,
    pub level: c_int,
    pub packetized: bool,
}

impl Default for VideoFormat {
    fn default() -> Self {
        VideoFormat {
            chroma: FourCC(0),
            width: 0,
            height: 0,
            x_offset: 0,
            y_offset: 0,
            visible_width: 0,
            visible_height: 0,
            bits_per_pixel: 0,
            sar: Ratio { num: 0, den: 0 },
            frame_rate: Ratio { num: 0, den: 0 },
            rgb_mask: None,
            orientation: video_orientation_t::ORIENT_NORMAL,
            primaries: video_color_primaries_t::COLOR_PRIMARIES_UNDEF,
            transfer: video_transfer_func_t::TRANSFER_FUNC_UNDEF,
            color_space: video_color_space_t::COLOR_SPACE_UNDEF,
            color_range: video_color_range_t::COLOR_RANGE_UNDEF,
            chroma_location: video_chroma_location_t::CHROMA_LOCATION_UNDEF,
        }
    }
}

impl VideoFormat {
    pub fn new(chroma: FourCC) -> Self {
        let mut new = VideoFormat::default();
        new.chroma = chroma;
        new
    }

    pub(crate) fn as_ffi(&self) -> video_format_t {
        let Ratio {
            num: sar_num,
            den: sar_den,
        } = self.sar;
        let Ratio {
            num: frame_rate,
            den: frame_rate_base,
        } = self.frame_rate;
        let (rmask, gmask, bmask) = self
            .rgb_mask
            .as_ref()
            .map_or((0, 0, 0), |mask| (mask.rmask, mask.gmask, mask.bmask));
        video_format_t {
            i_chroma: self.chroma.0,
            i_width: self.width,
            i_height: self.height,
            i_x_offset: self.x_offset,
            i_y_offset: self.y_offset,
            i_visible_width: self.visible_width,
            i_visible_height: self.visible_height,
            i_bits_per_pixel: self.bits_per_pixel,
            i_sar_num: sar_num,
            i_sar_den: sar_den,
            i_frame_rate: frame_rate,
            i_frame_rate_base: frame_rate_base,
            i_rmask: rmask,
            i_gmask: gmask,
            i_bmask: bmask,
            p_palette: ptr::null_mut(),
            orientation: self.orientation,
            primaries: self.primaries,
            transfer: self.transfer,
            space: self.color_space,
            color_range: self.color_range,
            chroma_location: self.chroma_location,
            // default values
            multiview_mode: video_multiview_mode_t::MULTIVIEW_2D,
            b_multiview_right_eye_first: Default::default(),
            projection_mode: video_projection_mode_t::PROJECTION_MODE_RECTANGULAR,
            pose: vlc_viewpoint_t {
                yaw: Default::default(),
                pitch: Default::default(),
                roll: Default::default(),
                fov: FIELD_OF_VIEW_DEGREES_DEFAULT as f32,
            },
            mastering: video_format_t__bindgen_ty_1 {
                primaries: Default::default(),
                white_point: Default::default(),
                max_luminance: Default::default(),
                min_luminance: Default::default(),
            },
            lighting: video_format_t__bindgen_ty_2 {
                MaxCLL: Default::default(),
                MaxFALL: Default::default(),
            },
            i_cubemap_padding: Default::default(),
        }
    }
}

impl From<ReplayGain> for audio_replay_gain_t {
    fn from(replay_gain: ReplayGain) -> Self {
        let extract = |option: Option<f32>| option.map_or((false, 0f32), |f| (true, f));
        let (tb_peak, tf_peak) = extract(replay_gain.track.peak);
        let (tb_gain, tf_gain) = extract(replay_gain.track.gain);
        let (ab_peak, af_peak) = extract(replay_gain.album.peak);
        let (ab_gain, af_gain) = extract(replay_gain.album.gain);
        Self {
            pb_peak: [tb_peak, ab_peak],
            pf_peak: [tf_peak, af_peak],
            pb_gain: [tb_gain, ab_gain],
            pf_gain: [tf_gain, af_gain],
        }
    }
}

impl Default for AudioFormat {
    fn default() -> Self {
        AudioFormat {
            format: FourCC(0),
            rate: 0,
            physical_channels: 0,
            chan_mode: 0,
            channel_type: audio_channel_type_t::AUDIO_CHANNEL_TYPE_BITMAP,
            bytes_per_frame: 0,
            frame_length: 0,
            bits_per_sample: 0,
            block_align: 0,
            channels: 0,

            // replay gain in the audio format for simplicity
            replay_gain: ReplayGain {
                track: ReplayGainItem {
                    peak: None,
                    gain: None,
                },
                album: ReplayGainItem {
                    peak: None,
                    gain: None,
                },
            },
        }
    }
}

impl AudioFormat {
    pub(crate) fn as_ffi(&self) -> audio_format_t {
        audio_format_t {
            i_format: self.format.0,
            i_rate: self.rate,
            i_physical_channels: self.physical_channels,
            i_chan_mode: self.chan_mode,
            channel_type: self.channel_type,
            i_bytes_per_frame: self.bytes_per_frame,
            i_frame_length: self.frame_length,
            i_bitspersample: self.bits_per_sample,
            i_blockalign: self.block_align,
            i_channels: self.channels,
        }
    }
}

impl EsFormat {
    pub fn default_audio() -> Self {
        EsFormat {
            cat: es_format_category_e::UNKNOWN_ES,
            codec: FourCC(0),
            original_fourcc: FourCC(0),
            id: None,
            group: None,
            priority: Priority::Value(0),
            language: None,
            extra_languages: Vec::new(),
            format: Format::Audio(AudioFormat::default()),
            bitrate: 0,
            profile: -1,
            level: -1,
            packetized: true,
        }
    }

    pub fn default_video() -> Self {
        EsFormat {
            cat: es_format_category_e::UNKNOWN_ES,
            codec: FourCC(0),
            original_fourcc: FourCC(0),
            id: None,
            group: None,
            priority: Priority::Value(0),
            language: None,
            extra_languages: Vec::new(),
            format: Format::Video(VideoFormat::default()),
            bitrate: 0,
            profile: -1,
            level: -1,
            packetized: true,
        }
    }

    pub fn init(&mut self, cat: es_format_category_e, codec: FourCC) {
        self.cat = cat;
        self.codec = codec;
        self.original_fourcc = FourCC(0);
        self.id = Some(-1);
        self.group = Some(0);
        self.priority = Priority::Value(ES_PRIORITY_SELECTABLE_MIN as i32);
        self.language = None;
        self.extra_languages = Vec::new();
        if cat == es_format_category_e::VIDEO_ES {
            self.format = Format::Video(VideoFormat::default());
        } else if cat == es_format_category_e::AUDIO_ES {
            self.format = Format::Audio(AudioFormat::default());
        }
        self.bitrate = 0;
        self.profile = -1;
        self.level = -1;
        self.packetized = true;
    }

    pub fn as_ffi(&self) -> es_format_t {
        assert!(self.id.unwrap_or(-1) >= -1);
        assert!(self.group.unwrap_or(-1) >= -1);

        let (lang, desc) =
            self.language
                .as_ref()
                .map_or((ptr::null_mut(), ptr::null_mut()), |lang| {
                    (
                        lang.name.as_ptr() as *mut _,
                        lang.description
                            .as_ref()
                            .map_or(ptr::null_mut(), |s| s.as_ptr() as *mut _),
                    )
                });

        let mut extra_languages = self
            .extra_languages
            .iter()
            .map(|lang| extra_languages_t {
                psz_language: lang.name.as_ptr() as *mut _,
                psz_description: lang
                    .description
                    .as_ref()
                    .map_or(ptr::null_mut(), |s| s.as_ptr() as *mut _),
            })
            .collect::<Vec<_>>();

        es_format_t {
            i_cat: self.cat,
            i_codec: self.codec.0,
            i_original_fourcc: self.original_fourcc.0,
            i_id: self.id.map_or(-1, |id| id as c_int),
            i_group: self.group.map_or(-1, |id| id as c_int),
            i_priority: match self.priority {
                Priority::NotSelectable => -2,
                Priority::NotSelected => -1,
                Priority::Value(priority) => {
                    assert!(priority >= 0);
                    priority as c_int
                }
            },
            psz_language: lang,
            psz_description: desc,
            i_extra_languages: extra_languages.len() as c_uint,
            p_extra_languages: if extra_languages.len() == 0 {
                ptr::null_mut()
            } else {
                extra_languages.as_mut_ptr()
            },
            __bindgen_anon_1: match &self.format {
                Format::Video(video_fmt) => es_format_t__bindgen_ty_1 {
                    video: video_fmt.as_ffi(),
                },
                Format::Audio(audio_fmt) => {
                    let replay_gain = audio_fmt.replay_gain;
                    es_format_t__bindgen_ty_1 {
                        __bindgen_anon_1: es_format_t__bindgen_ty_1__bindgen_ty_1 {
                            audio: audio_fmt.as_ffi(),
                            audio_replay_gain: replay_gain.into(),
                        },
                    }
                }
                Format::Subs(_sub_fmt) => unreachable!(), // TODO
            },
            i_bitrate: self.bitrate,
            i_profile: self.profile,
            i_level: self.level,
            b_packetized: self.packetized,
            i_extra: 0,
            p_extra: ptr::null_mut(),
        }
    }
}
