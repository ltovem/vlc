//! Streams management.

pub use block::Block;
pub use frame::Frame;
pub use impl_stream::Stream;

use crate::error::{CoreError, Result};
use crate::input_item::InputItemNode;
use crate::tick::{Miliseconds, Tick};

use self::block::NativeBlock;

pub mod block;
pub mod frame;
pub mod impl_stream;

/// This trait allows defining a stream for reading data in it.
pub trait ReadStream: StreamControl {
    /// Pull some bytes from this source into the specified buffer, returning how many bytes were read.
    #[doc(alias = "pf_read")]
    fn read(&mut self, buf: &mut [u8]) -> Result<usize>;

    /// Seek to an offset, in bytes, in a stream.
    #[doc(alias = "pf_seek")]
    fn seek(&mut self, _bytes: u64) -> Result<()> {
        Err(CoreError::Unimplemented)
    }
}

/// This trait allows defining a stream for reading data in it as a block.
pub trait ReadBlockStream: StreamControl {
    /// Pull some bytes from this source into a allocated block of data, returning the block.
    #[doc(alias = "pf_block")]
    fn block(&mut self) -> Result<NativeBlock>;

    /// Seek to an offset, in bytes, in a stream.
    #[doc(alias = "pf_seek")]
    fn seek(&mut self, _bytes: u64) -> Result<()> {
        Err(CoreError::Unimplemented)
    }
}

/// This trait allows defining a stream for reading a something directory-like.
pub trait ReadDirStream: StreamControl {
    /// Pull some data from the stream and output in the InputItemNode.
    #[doc(alias = "pf_readdir")]
    fn read_dir(&mut self, input_item_node: &mut InputItemNode) -> Result<()>;
}

/// This trait allows defining a stream for demuxing some datas.
pub trait Demux: DemuxControl {
    /// Pulls some demuxing.
    #[doc(alias = "pf_demux")]
    fn demux(&mut self) -> Result<()>;
}

/// This trait allows defining a stream for demuxing some datas.
pub trait ReadDirDemux: DemuxControl {
    /// Pull some data from the stream and output in the InputItemNode.
    #[doc(alias = "pf_readdir")]
    fn read_dir(&mut self, input_item_node: &mut InputItemNode) -> Result<()>;
}

pub trait DemuxControl {
    /// Get the source stream.
    fn source_stream(&mut self) -> Option<&mut Stream>;

    /// Get the current time
    fn time(&mut self) -> Tick;

    /// Get the current time
    fn length(&mut self) -> Option<Tick> {
        None
    }

    /// Does the stream support the seek operation
    fn can_seek(&mut self) -> bool {
        self.source_stream()
            .map_or(false, |source| source.can_seek())
    }

    /// Automatic done.
    fn pts_delay(&mut self) -> Tick {
        self.source_stream()
            .map_or(Tick::from_miliseconds(Miliseconds::from(200)), |source| {
                source.pts_delay()
            })
    }

    fn can_control_pace(&mut self) -> bool {
        self.source_stream()
            .map_or(false, |source| source.can_control_pace())
    }

    fn can_pause(&mut self) -> bool {
        self.source_stream()
            .map_or(false, |source| source.can_pause())
    }

    fn set_pause_state(&mut self, state: bool) -> Result<()> {
        self.source_stream()
            .ok_or(CoreError::Unimplemented)?
            .set_pause_state(state)
    }

    fn set_seek_point(&mut self, _pts: i32) -> Result<()> {
        Err(CoreError::Unimplemented)
    }
}

/// This trait allows defining some controls methods.
pub trait StreamControl {
    /// Get the source stream.
    fn source_stream(&mut self) -> Option<&mut Stream>;

    /// Does the stream support the seek operation
    fn can_seek(&mut self) -> bool {
        self.source_stream()
            .map_or(false, |source| source.can_seek())
    }

    fn can_fast_seek(&mut self) -> bool {
        self.source_stream()
            .map_or(false, |source| source.can_fast_seek())
    }

    fn can_pause(&mut self) -> bool {
        self.source_stream()
            .map_or(false, |source| source.can_pause())
    }

    fn can_control_pace(&mut self) -> bool {
        self.source_stream()
            .map_or(false, |source| source.can_control_pace())
    }

    fn size(&mut self) -> Result<u64> {
        self.source_stream().ok_or(CoreError::Unimplemented)?.size()
    }

    fn pts_delay(&mut self) -> Tick {
        self.source_stream()
            .map_or(Tick::from_miliseconds(Miliseconds::from(200)), |source| {
                source.pts_delay()
            })
    }

    fn content_type(&mut self) -> Result<String> {
        self.source_stream()
            .ok_or(CoreError::Unimplemented)?
            .content_type()
    }

    fn set_pause_state(&mut self, state: bool) -> Result<()> {
        self.source_stream()
            .ok_or(CoreError::Unimplemented)?
            .set_pause_state(state)
    }

    fn set_seek_point(&mut self, _pts: i32) -> Result<()> {
        Err(CoreError::Unimplemented)
    }

    // TODO:
    // TITLE_INFO
    // TITLE
    // META
    // SIGNAL
    // TAGS
    // TYPE
    // RECORD_STATE
}
