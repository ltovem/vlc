#![deny(unsafe_op_in_unsafe_fn)]

//! The `vlcrs-core` crate.
//!
//! This crate contains the vlc core APIs that have been ported or
//! wrapped for usage by Rust code in the modules and is shared by all of them.
//!
//! If you need a vlc core C API that is not ported or wrapped yet here,
//! then do so first instead of bypassing this crate.

pub mod error;
pub mod es;
pub mod es_out;
pub mod fourcc;
pub mod input_item;
pub mod messages;
pub mod module;
pub mod object;
pub mod rational;
pub mod sout;
pub mod stream;
pub mod tick;
pub mod url;
