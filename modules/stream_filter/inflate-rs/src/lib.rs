use std::io::Read;

use flate2::read::{GzDecoder, ZlibDecoder};
use vlcrs_core::error::{CoreError, Result};
use vlcrs_core::module::stream_filter::{Module, StreamModule, ThisStream};
use vlcrs_core::stream::{ReadStream, Stream, StreamControl};
use vlcrs_core_macros::module;

struct Inflate;

struct GzStreamReader<'a> {
    gz_decoder: GzDecoder<&'a mut Stream>,
}

struct ZlibStreamReader<'a> {
    zlib_decoder: ZlibDecoder<&'a mut Stream>,
}

impl Module for Inflate {
    fn open<'m>(
        _this_stream: ThisStream<'m>,
        source: &'m mut Stream,
        _logger: &'m mut vlcrs_core::messages::Logger,
        _args: &mut vlcrs_core::module::ModuleArgs,
    ) -> Result<StreamModule<'m>> {
        let buf = source.peek(2)?;

        match buf.buf() {
            [0xF8, _] => {
                let zlib = ZlibStreamReader::new(source);
                Ok(StreamModule::Read(Box::new(zlib)))
            }
            [0x1F, 0x8B] => {
                let gz = GzStreamReader::new(source);
                Ok(StreamModule::Read(Box::new(gz)))
            }
            _ => Err(CoreError::Unknown),
        }
    }
}

impl GzStreamReader<'_> {
    fn new(stream: &mut Stream) -> GzStreamReader<'_> {
        GzStreamReader {
            gz_decoder: GzDecoder::new(stream),
        }
    }
}

impl ZlibStreamReader<'_> {
    fn new(stream: &mut Stream) -> ZlibStreamReader<'_> {
        ZlibStreamReader {
            zlib_decoder: ZlibDecoder::new(stream),
        }
    }
}

impl ReadStream for GzStreamReader<'_> {
    fn read(&mut self, buf: &mut [u8]) -> Result<usize> {
        self.gz_decoder.read(buf).map_err(|_| CoreError::Unknown)
    }
}

impl ReadStream for ZlibStreamReader<'_> {
    fn read(&mut self, buf: &mut [u8]) -> Result<usize> {
        self.zlib_decoder.read(buf).map_err(|_| CoreError::Unknown)
    }
}

impl StreamControl for GzStreamReader<'_> {
    fn source_stream(&mut self) -> Option<&mut Stream> {
        Some(self.gz_decoder.get_mut())
    }

    fn can_seek(&mut self) -> bool {
        false
    }

    fn can_fast_seek(&mut self) -> bool {
        false
    }
}

impl StreamControl for ZlibStreamReader<'_> {
    fn source_stream(&mut self) -> Option<&mut Stream> {
        Some(self.zlib_decoder.get_mut())
    }

    fn can_seek(&mut self) -> bool {
        false
    }

    fn can_fast_seek(&mut self) -> bool {
        false
    }
}

module! {
    type: Inflate,
    capability: "stream_filter" @ 331,
    category: SUBCAT_INPUT_STREAM_FILTER,
    description: "Zlib decompression filter - Built in Rust",
}
