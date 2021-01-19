#![allow(non_camel_case_types, dead_code, non_snake_case)]

extern crate core;
extern crate flavors;
extern crate libc;
extern crate nom;
use nom::error::ErrorKind;
use std::boxed::Box;
use std::io::Read;
use vlccore_sys::es_format::*;
use vlccore_sys::es_out::CEsOut;
use vlccore_sys::fourcc::*;
use vlccore_sys::message::*;
use vlccore_sys::modules::demux::DemuxModule;
use vlccore_sys::stream::CStream;
use vlccore_sys::vlc_core::*;
use vlccore_sys::vlc_fourcc;
use vlccore_sys::*;
use vlccore_sys::modules::demux::RetDemux::*;
 use vlccore_sys::modules::demux::RetDemux;

const PLUGIN_NAME: &[u8; 13] = b"inrustwetrust";

use vlccore_sys::es_format::Format::Audio;

pub struct CFLV {
    i_pos: i32,
    i_size: i32,
    video_initialized: bool,
    video_es_format: EsFormat,
    video_es_id: Option<Box<es_out_id_t>>,
    audio_initialized: bool,
    audio_es_format: EsFormat,
    audio_es_id: Option<Box<es_out_id_t>>,
}

const VLC_TS_0: mtime_t = 1;

pub fn flv_open_flv(stream: &mut CStream) -> Option<CFLV> {
    LogDbg!(stream, PLUGIN_NAME, "in rust function before stream_Peek ");
    let sl = stream.peek(9);
    if sl.is_err() {
        return None;
    }
    let sl = sl.unwrap();
    match flavors::parser::header(&sl) {
        nom::IResult::Err(_) => {
            LogErr!(stream, PLUGIN_NAME, "parsing error");
            return None;
        }
        nom::IResult::Ok((_, h)) => {
            stream.seek(h.offset as u64);

            Some(CFLV {
                i_pos: h.offset as i32,
                i_size: 0,
                video_initialized: false,
                video_es_format: EsFormat::default_video(),
                video_es_id: None,
                audio_initialized: false,
                audio_es_format: EsFormat::default_audio(),
                audio_es_id: None,
            })
        }
    }
}

impl DemuxModule for CFLV {
    fn demux(&mut self, stream: &mut CStream, es_out: &mut CEsOut) -> RetDemux {
        let mut header = [0u8; 15];
        let sz = stream.read(&mut header).unwrap_or(0);
        if sz < 15 {
            if sz == 4 {
                LogDbg!(stream, PLUGIN_NAME, "got {} bytes, end of stream?", sz);
                return OK;
            } else {
                LogDbg!(stream, PLUGIN_NAME, "got {} bytes, end of stream?", sz);
                return ERR;
            }
        }

        let r = nom::number::streaming::be_u32::<(_, ErrorKind)>(&header[..4]);
        if let nom::IResult::Ok((_i, _o)) = r {
        } else {
            LogDbg!(stream, PLUGIN_NAME, "could not parse");
            return ERR;
        }

        let r = flavors::parser::tag_header(&header[4..]);
        match r {
            nom::IResult::Err(_) => {
                return ERR;
            }
            _ => {}
        }

        if let nom::IResult::Ok((_remaining, header)) = r {
            LogDbg!(
                stream,
                PLUGIN_NAME,
                "tag_header: type={},\tsize={},\ttimestamp:{},\tstream_id: {}",
                header.tag_type as u_int32_t,
                header.data_size,
                header.timestamp,
                header.stream_id
            );
            self.i_pos += 15;
            self.i_size = header.data_size as i32;
            match header.tag_type {
                flavors::parser::TagType::Audio => {
                    let mut a_header = [0u8; 1];
                    let sz = stream.read(&mut a_header).unwrap_or(0);
                    if sz < 1 {
                        return ERR;
                    }

                    if let nom::IResult::Ok((_, audio_header)) =
                        flavors::parser::audio_data_header(&a_header)
                    {
                        if !self.audio_initialized {
                            flv_init_audio(self, &audio_header, es_out);
                        }
                        return flv_send(stream, self, &header, es_out, true);
                    }
                    return ERR;
                }
                flavors::parser::TagType::Script => {
                    stream.seek(header.data_size as u64);
                    return OK;
                }
                flavors::parser::TagType::Video => {
                    let mut v_header = [0u8; 1];
                    let sz = (stream).read(&mut v_header).unwrap_or(0);
                    if sz < 1 {
                        LogErr!(
                            stream,
                            PLUGIN_NAME,
                            "coult not read header, got: {} bytes",
                            sz
                        );
                        return ERR;
                    }
                    if let nom::IResult::Ok((_, vheader)) =
                        flavors::parser::video_data_header(&v_header)
                    {
                        if !self.video_initialized {
                            self.video_es_format.init(
                                es_format_category_e::VIDEO_ES,
                                video_codec_id_to_fourcc(vheader.codec_id),
                            );
                            self.video_es_id = Some(es_out.add(&mut self.video_es_format));
                            self.video_initialized = true;
                        }
                        return flv_send(stream, self, &header, es_out, false);
                    }
                    return ERR;
                }
            }
        }
        ERR
    }

    fn start(&self) -> i64 {
        self.i_pos as i64
    }

    fn end(&self) -> i64 {
        (self.i_pos + self.i_size) as i64
    }
}
pub fn flv_send(
    stream: &mut stream_t,
    flv: &mut CFLV,
    header: &flavors::parser::TagHeader,
    es_out: &mut CEsOut,
    is_audio: bool,
) -> RetDemux {
    let mut p_block = stream.block((header.data_size - 1) as u64);

    p_block.i_dts = VLC_TS_0 + (header.timestamp as mtime_t * 1000);
    p_block.i_pts = VLC_TS_0 + (header.timestamp as mtime_t * 1000);

    es_out.set_pcr(p_block.i_pts);
    if is_audio {
        es_out.send(flv.audio_es_id.as_mut().unwrap(), p_block);
        LogDbg!(
            stream,
            PLUGIN_NAME,
            "audio block of size {} sent",
            header.data_size - 1
        );
    } else {
        es_out.send(flv.video_es_id.as_mut().unwrap(), p_block);
        LogDbg!(
            stream,
            PLUGIN_NAME,
            "video block of size {} sent",
            header.data_size - 1
        );
    }
    OK
}

pub fn flv_init_audio(
    flv: &mut CFLV,
    audio_header: &flavors::parser::AudioDataHeader,
    es_out: &mut CEsOut,
) {
    flv.audio_es_format.init(
        es_format_category_e::AUDIO_ES,
        audio_codec_id_to_fourcc(audio_header.sound_format),
    );
    match &mut flv.audio_es_format.format {
        Audio(AudioFormat) => {
            AudioFormat.channels = match audio_header.sound_type {
                flavors::parser::SoundType::SndMono => 1,
                flavors::parser::SoundType::SndStereo => 2,
            };

            AudioFormat.rate = match audio_header.sound_rate {
                flavors::parser::SoundRate::_5_5KHZ => 5500,
                flavors::parser::SoundRate::_11KHZ => 11000,
                flavors::parser::SoundRate::_22KHZ => 22050,
                flavors::parser::SoundRate::_44KHZ => 44000,
            };

            AudioFormat.bits_per_sample = match audio_header.sound_size {
                flavors::parser::SoundSize::Snd8bit => 8,
                flavors::parser::SoundSize::Snd16bit => 16,
            };
            AudioFormat.rate = AudioFormat.rate * AudioFormat.bits_per_sample;
            flv.audio_es_id = Some(es_out.add(&mut flv.audio_es_format));
            flv.audio_initialized = true;
        }
        _ => {}
    }
}

pub fn audio_codec_id_to_fourcc(id: flavors::parser::SoundFormat) -> FourCC {
    match id {
        flavors::parser::SoundFormat::PCM_NE => vlc_fourcc!('l', 'p', 'c', 'm'),
        flavors::parser::SoundFormat::ADPCM => vlc_fourcc!('S', 'W', 'F', 'a'),
        flavors::parser::SoundFormat::MP3 => vlc_fourcc!('m', 'p', '3', ' '),
        flavors::parser::SoundFormat::PCM_LE => vlc_fourcc!('l', 'p', 'c', 'm'),
        flavors::parser::SoundFormat::NELLYMOSER_16KHZ_MONO => vlc_fourcc!('N', 'E', 'L', 'L'),
        flavors::parser::SoundFormat::NELLYMOSER_8KHZ_MONO => vlc_fourcc!('N', 'E', 'L', 'L'),
        flavors::parser::SoundFormat::NELLYMOSER => vlc_fourcc!('N', 'E', 'L', 'L'),
        flavors::parser::SoundFormat::PCM_ALAW => vlc_fourcc!('a', 'l', 'a', 'w'),
        flavors::parser::SoundFormat::PCM_ULAW => vlc_fourcc!('u', 'l', 'a', 'w'),
        flavors::parser::SoundFormat::AAC => vlc_fourcc!('m', 'p', '4', 'a'),
        flavors::parser::SoundFormat::SPEEX => vlc_fourcc!('s', 'p', 'x', ' '),
        flavors::parser::SoundFormat::MP3_8KHZ => vlc_fourcc!('m', 'p', '3', ' '),
        flavors::parser::SoundFormat::DEVICE_SPECIFIC => vlc_fourcc!('u', 'n', 'd', 'f'),
    }
}

pub fn video_codec_id_to_fourcc(id: flavors::parser::CodecId) -> FourCC {
    match id {
        flavors::parser::CodecId::JPEG => vlc_fourcc!('j', 'p', 'e', 'g'),
        flavors::parser::CodecId::H263 => vlc_fourcc!('F', 'L', 'V', '1'),
        flavors::parser::CodecId::SCREEN => vlc_fourcc!('F', 'S', 'V', '1'),
        flavors::parser::CodecId::VP6 => vlc_fourcc!('V', 'P', '6', '0'),
        flavors::parser::CodecId::VP6A => vlc_fourcc!('V', 'P', '6', 'A'),
        flavors::parser::CodecId::SCREEN2 => vlc_fourcc!('F', 'S', 'V', '2'),
        flavors::parser::CodecId::H264 => vlc_fourcc!('F', 'L', 'V', '1'),
        flavors::parser::CodecId::SORENSON_H263 => vlc_fourcc!('F', 'L', 'V', '1'), //TODO right fourcc
        flavors::parser::CodecId::MPEG4Part2 => vlc_fourcc!('M', 'P', 'G', '2'), //TODO right fourcc
    }
}
