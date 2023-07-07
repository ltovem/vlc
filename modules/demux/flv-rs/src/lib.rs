//! FLV Demux module

use flavors::parser::{AudioDataHeader, VideoDataHeader};
use vlcrs_core::{
    debug, error,
    error::Result,
    es::{EsFormat, EsFormatCategory},
    es_out::{EsOut, EsOutBaked, EsOutId},
    fourcc::FourCC,
    messages::Logger,
    module::{
        demux::{DemuxModule, Module, ThisDemux},
        ModuleArgs,
    },
    stream::{Demux, DemuxControl, Frame, Stream},
    tick::Tick,
    warn,
};
use vlcrs_core_macros::module;

pub struct FLV;

pub struct FLVDemuxer<'a> {
    source: &'a mut Stream,
    es_out: EsOutBaked<'a>,
    logger: &'a mut Logger,
    pcr: Tick,
    audio_es_out_id: Option<(AudioDataHeader, EsOutId)>,
    video_es_out_id: Option<(VideoDataHeader, EsOutId)>,
    metadata: MetaData,
}

#[derive(Debug, Default)]
struct MetaData {
    width: Option<u32>,
    height: Option<u32>,
    framerate: Option<f64>,
    audio_sample_rate: Option<u32>,
    duration_in_seconds: Option<f64>,
}

impl Module for FLV {
    fn open<'a>(
        _this_demux: ThisDemux<'a>,
        source: &'a mut Stream,
        es_out: &'a mut EsOut,
        logger: &'a mut Logger,
        _args: &mut ModuleArgs,
    ) -> Result<DemuxModule<'a>> {
        let buf = source.peek(3 + 1 + 1 + 4)?;

        match flavors::parser::header(buf.buf()) {
            Err(err) => {
                debug!(logger, "invalid header: {:?}", err);
                Err(vlcrs_core::error::CoreError::Unknown)
            }
            Ok((_remaining, h)) => {
                debug!(logger, "header: {:?}", h);
                source.seek(h.offset.into())?;

                Ok(DemuxModule::Demux(Box::new(FLVDemuxer {
                    source,
                    logger,
                    es_out: es_out.into(),
                    pcr: Tick::ZERO,
                    audio_es_out_id: None,
                    video_es_out_id: None,
                    metadata: MetaData::default(),
                })))
            }
        }
    }
}

impl DemuxControl for FLVDemuxer<'_> {
    fn source_stream(&mut self) -> Option<&mut Stream> {
        Some(self.source)
    }

    fn time(&mut self) -> Tick {
        self.pcr
    }

    fn length(&mut self) -> Option<Tick> {
        self.metadata
            .duration_in_seconds
            .map(|duration_in_seconds| Tick::from_seconds(duration_in_seconds.into()))
    }

    fn can_seek(&mut self) -> bool {
        false
    }
}

pub fn audio_codec_id_to_fourcc(id: flavors::parser::SoundFormat) -> FourCC {
    match id {
        flavors::parser::SoundFormat::PCM_NE => FourCC::VLC_CODEC_DVD_LPCM,
        flavors::parser::SoundFormat::ADPCM => FourCC::VLC_CODEC_ADPCM_SWF,
        flavors::parser::SoundFormat::MP3 => FourCC::VLC_CODEC_MP3,
        flavors::parser::SoundFormat::PCM_LE => FourCC::VLC_CODEC_DVD_LPCM,
        flavors::parser::SoundFormat::NELLYMOSER_16KHZ_MONO => FourCC::VLC_CODEC_NELLYMOSER,
        flavors::parser::SoundFormat::NELLYMOSER_8KHZ_MONO => FourCC::VLC_CODEC_NELLYMOSER,
        flavors::parser::SoundFormat::NELLYMOSER => FourCC::VLC_CODEC_NELLYMOSER,
        flavors::parser::SoundFormat::PCM_ALAW => FourCC::VLC_CODEC_ALAW,
        flavors::parser::SoundFormat::PCM_ULAW => vlcrs_core::fourcc!('u', 'l', 'a', 'w'),
        flavors::parser::SoundFormat::AAC => FourCC::VLC_CODEC_MP4A,
        flavors::parser::SoundFormat::SPEEX => FourCC::VLC_CODEC_SPEEX,
        flavors::parser::SoundFormat::MP3_8KHZ => FourCC::VLC_CODEC_MP3,
        flavors::parser::SoundFormat::DEVICE_SPECIFIC => FourCC::VLC_CODEC_UNKNOWN,
    }
}

pub fn video_codec_id_to_fourcc(id: flavors::parser::CodecId) -> FourCC {
    match id {
        flavors::parser::CodecId::JPEG => FourCC::VLC_CODEC_JPEG,
        flavors::parser::CodecId::H263 => FourCC::VLC_CODEC_H263,
        flavors::parser::CodecId::SCREEN => FourCC::VLC_CODEC_FLASHSV,
        flavors::parser::CodecId::VP6 => FourCC::VLC_CODEC_VP6,
        flavors::parser::CodecId::VP6A => FourCC::VLC_CODEC_VP6A,
        flavors::parser::CodecId::SCREEN2 => FourCC::VLC_CODEC_FLASHSV2,
        flavors::parser::CodecId::H264 => FourCC::VLC_CODEC_H264,
        flavors::parser::CodecId::SORENSON_H263 => FourCC::VLC_CODEC_H263,
        flavors::parser::CodecId::MPEG4Part2 => FourCC::VLC_CODEC_MPEGH,
    }
}

fn audio_codec_to_es_format(
    audio_header: &flavors::parser::AudioDataHeader,
    metadata: &MetaData,
) -> EsFormat {
    let mut es_format = EsFormat::new(
        EsFormatCategory::AUDIO_ES,
        audio_codec_id_to_fourcc(audio_header.sound_format),
    );

    if let Some(audio_format) = es_format.audio_mut() {
        audio_format.i_channels = match audio_header.sound_type {
            flavors::parser::SoundType::SndMono => 1,
            flavors::parser::SoundType::SndStereo => 2,
        };

        // Prefer the metadata variant since the audio header sound rate is
        // too small and some encoder lie (ffmpeg mp3 48kHz which set 44kHz).
        if let Some(audio_sample_rate) = metadata.audio_sample_rate {
            audio_format.i_rate = audio_sample_rate;
        } else {
            audio_format.i_rate = match audio_header.sound_rate {
                flavors::parser::SoundRate::_5_5KHZ => 5500,
                flavors::parser::SoundRate::_11KHZ => 11000,
                flavors::parser::SoundRate::_22KHZ => 22050,
                flavors::parser::SoundRate::_44KHZ => 44100,
            };
        }

        audio_format.i_bitspersample = match audio_header.sound_size {
            flavors::parser::SoundSize::Snd8bit => 8,
            flavors::parser::SoundSize::Snd16bit => 16,
        };
    }

    es_format
}

fn video_codec_to_es_format(
    video_header: &flavors::parser::VideoDataHeader,
    metadata: &MetaData,
) -> EsFormat {
    let mut es_format = EsFormat::new(
        EsFormatCategory::VIDEO_ES,
        video_codec_id_to_fourcc(video_header.codec_id),
    );

    if video_header.codec_id == flavors::parser::CodecId::H264 {
        es_format.set_original_fourcc(vlcrs_core::fourcc!('a', 'v', 'c', '1'));
    }

    if let Some(video_format) = es_format.video_mut() {
        video_format.i_width = metadata.width.unwrap_or(0);
        video_format.i_height = metadata.height.unwrap_or(0);
        video_format.i_visible_width = video_format.i_width;
        video_format.i_visible_height = video_format.i_height;

        if let Some(framerate) = metadata.framerate {
            video_format.i_frame_rate = framerate as _;
            video_format.i_frame_rate_base = 1;
        }
        video_format.i_sar_num = 1;
        video_format.i_sar_den = 1;
    }

    es_format
}

impl FLVDemuxer<'_> {
    fn send_audio(
        &mut self,
        audio_header: flavors::parser::AudioDataHeader,
        audio_data: Frame,
    ) -> Result<usize> {
        match &self.audio_es_out_id {
            Some((current_audio_header, _es_out_id)) if current_audio_header != &audio_header => {
                let Some((_audio_header, es_out_id)) = self.audio_es_out_id.take() else { unreachable!() };
                self.es_out.del(es_out_id)?;
            }
            _ => {}
        }

        if let None = &self.audio_es_out_id {
            let es_format = audio_codec_to_es_format(&audio_header, &self.metadata);
            let es_out_id = self.es_out.add(&es_format)?;
            self.audio_es_out_id = Some((audio_header, es_out_id));
        }

        let audio_es_out_id = &self.audio_es_out_id.as_ref().unwrap().1;
        self.es_out.send(audio_es_out_id, audio_data)
    }

    fn send_video(
        &mut self,
        video_header: flavors::parser::VideoDataHeader,
        video_data: Frame,
    ) -> Result<usize> {
        match &self.video_es_out_id {
            Some((current_video_header, _es_out_id)) if current_video_header != &video_header => {
                let Some((_video_header, es_out_id)) = self.video_es_out_id.take() else { unreachable!() };
                self.es_out.del(es_out_id)?;
            }
            _ => {}
        }

        if let None = &self.video_es_out_id {
            let es_format = video_codec_to_es_format(&video_header, &self.metadata);
            let es_out_id = self.es_out.add(&es_format)?;
            self.video_es_out_id = Some((video_header, es_out_id));
        }

        let video_es_out_id = &self.video_es_out_id.as_ref().unwrap().1;
        self.es_out.send(video_es_out_id, video_data)
    }
}

impl Demux for FLVDemuxer<'_> {
    fn demux(&mut self) -> Result<()> {
        let mut header = [0u8; 4 + 11];
        let sz = self.source.read(&mut header)?;

        if sz < 15 {
            debug!(self.logger, "end of stream?");
            return Err(vlcrs_core::error::CoreError::Eof);
        }

        // let prev_size = u32::from_be_bytes(header[..4].try_into().unwrap());
        let Ok((_remaining, header)) = flavors::parser::tag_header(&header[4..]) else {
            error!(self.logger, "unable to decode the tag header: {:?}", header);
            return Err(vlcrs_core::error::CoreError::Eof);
        };

        debug!(
            self.logger,
            "tag_header: type={:?}, data_size={}, timestamp:{}, stream_id: {}",
            header.tag_type,
            header.data_size,
            header.timestamp,
            header.stream_id
        );

        let ts = Tick::from_miliseconds(header.timestamp.into());
        self.pcr = std::cmp::max(self.pcr, ts);
        self.es_out.set_pcr(self.pcr)?;

        match header.tag_type {
            flavors::parser::TagType::Audio => {
                let mut audio_header = [0u8; 1];
                let sz = self.source.read(&mut audio_header)?;

                if sz < 1 {
                    warn!(self.logger, "end of stream?");
                    return Err(vlcrs_core::error::CoreError::Eof);
                }

                let Ok((_remaining, audio_header)) =
                        flavors::parser::audio_data_header(&audio_header) else {
                    error!(self.logger, "unable to decode the audio header: {:?}", audio_header);
                    return Err(vlcrs_core::error::CoreError::Eof);
                };

                debug!(self.logger, "audio_header: {:?}", &audio_header);

                let mut frame: Frame = self.source.block((header.data_size - 1) as _)?.into();
                frame.set_pts(ts);
                frame.set_dts(ts);

                self.send_audio(audio_header, frame)?;
            }
            flavors::parser::TagType::Video => {
                let mut video_header = [0u8; 1];
                let sz = self.source.read(&mut video_header)?;

                if sz < 1 {
                    warn!(self.logger, "end of stream? {:?}", video_header);
                    return Err(vlcrs_core::error::CoreError::Eof);
                }

                let Ok((_remaining, video_header)) =
                        flavors::parser::video_data_header(&video_header) else {
                    error!(self.logger, "unable to decode the video header: {:?}", video_header);
                    return Err(vlcrs_core::error::CoreError::Eof);
                };

                debug!(self.logger, "video_header: {:?}", &video_header);

                if video_header.codec_id == flavors::parser::CodecId::H264 {
                    let mut avc_video_header = [0u8; 4];
                    let sz = self.source.read(&mut avc_video_header)?;

                    if sz < 4 {
                        warn!(self.logger, "end of stream?");
                        return Err(vlcrs_core::error::CoreError::Eof);
                    }

                    let Ok((_remaining, _avc)) =
                            flavors::parser::avc_video_packet_header(&avc_video_header) else {
                        error!(self.logger, "unable to decode the avc video header: {:?}", avc_video_header);
                        return Err(vlcrs_core::error::CoreError::Eof);
                    };
                }

                let mut frame: Frame = self.source.block((header.data_size - 5) as _)?.into();

                frame.set_pts(ts);
                frame.set_dts(ts);
                if video_header.frame_type == flavors::parser::FrameType::Key {
                    frame.set_flag(vlcrs_core::stream::frame::FrameFlag::TypeI);
                }

                self.send_video(video_header, frame)?;
            }
            flavors::parser::TagType::Script => {
                let script_data = self.source.block(header.data_size as _)?;
                let Ok((_remaining, script_data)) =
                        flavors::parser::script_data(&script_data[..]) else {
                    error!(self.logger, "unable to decode the script data");
                    return Err(vlcrs_core::error::CoreError::Eof);
                };

                if script_data.name == "onMetaData" {
                    match script_data.arguments {
                        flavors::parser::ScriptDataValue::ECMAArray(objs) => {
                            for obj in objs {
                                use flavors::parser::ScriptDataValue::*;
                                match obj.data {
                                    Number(num) if obj.name == "audiosamplerate" => {
                                        self.metadata.audio_sample_rate = Some(num as _);
                                    }
                                    Number(num) if obj.name == "width" => {
                                        self.metadata.width = Some(num as _);
                                    }
                                    Number(num) if obj.name == "height" => {
                                        self.metadata.height = Some(num as _);
                                    }
                                    Number(num) if obj.name == "framerate" => {
                                        self.metadata.framerate = Some(num);
                                    }
                                    Number(num) if obj.name == "duration" => {
                                        self.metadata.duration_in_seconds = Some(num);
                                    }
                                    _ => {}
                                }
                            }
                        }
                        _ => {
                            warn!(
                                self.logger,
                                "wrong type for `onMetaData` {:?}", script_data.arguments
                            );
                        }
                    }
                }
            }
        }

        Ok(())
    }
}

module! {
    type: FLV,
    capability: "demux" @ 200,
    category: SUBCAT_INPUT_DEMUX,
    description: "FLV Rust demuxer",
}
