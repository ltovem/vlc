use flate2;
use flate2::read::{GzDecoder, ZlibDecoder};
use std::io::Read;
use vlccore_sys::modules::stream_filter::StreamFilterModule;

pub enum Inflate<R: Read> {
    Gz(flate2::read::GzDecoder<R>),
    Zlib(flate2::read::ZlibDecoder<R>),
}

impl<R: Read> StreamFilterModule for Inflate<R> {
    fn read(&mut self, buf_out: &mut [u8]) -> Option<i64> {
        let val = match self {
            Inflate::Gz(gz) => gz.read(buf_out),
            Inflate::Zlib(zlib) => zlib.read(buf_out),
        };
        if val.is_ok() {
            Some(val.unwrap() as i64)
        } else {
            Some(-1)
        }
    }
    fn close(&mut self) {
        let rust_inflate = unsafe { Box::from_raw(self) };
        drop(rust_inflate);
    }
}

impl<R: Read> Inflate<R> {
    pub fn new(stream: R, deflate: u8) -> Option<Self> {
        match deflate {
            15 => Some(Inflate::Zlib(ZlibDecoder::new(stream))),
            47 => Some(Inflate::Gz(GzDecoder::new(stream))),
            _ => None,
        }
    }
}
