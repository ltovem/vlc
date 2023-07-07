//! sout - Stream out

use std::{ffi::CString, marker::PhantomData, ptr::NonNull};

use vlcrs_core_sys::{
    sout_AccessOutCanControlPace, sout_AccessOutDelete, sout_AccessOutNew, sout_AccessOutSeek,
    sout_AccessOutWrite, sout_access_out_t, vlc_object_t,
};

use crate::{
    error::{cvp, cvr, cvt, Result},
    object::VlcObjectRef,
    stream::block::NativeBlock,
};

/// `sout_access_out_t`
#[doc(alias = "sout_access_out_t")]
pub struct SoutAccessOut<'a>(
    NonNull<sout_access_out_t>,
    PhantomData<&'a mut vlc_object_t>,
);

impl SoutAccessOut<'_> {
    /// Create a new [SoutAccessOut] from a [VlcObject]
    #[doc(alias = "sout_AccessOutNew")]
    pub fn new<'a>(obj: VlcObjectRef<'a>, access: &str, name: &str) -> Result<SoutAccessOut<'a>> {
        let access = CString::new(access).expect("cannot create access");
        let name = CString::new(name).expect("cannot create access name");

        // SAFETY: access and name are c-string, and obj is a valid and viling vlc_object_t
        let ptr =
            cvp(unsafe { sout_AccessOutNew(obj.into_raw(), access.as_ptr(), name.as_ptr()) })?;

        Ok(SoutAccessOut(ptr, PhantomData))
    }

    /// Seek to a new position
    #[doc(alias = "sout_AccessOutSeek")]
    pub fn seek(&mut self, pos: usize) -> Result<()> {
        // SAFETY: The sout_access_out_t is valid
        cvt(unsafe { sout_AccessOutSeek(self.0.as_ptr(), pos as u64) })
    }

    /// Write a block into it
    #[doc(alias = "sout_AccessOutWrite")]
    pub fn write(&mut self, block: impl Into<NativeBlock>) -> Result<usize> {
        let block = block.into();

        // SAFETY: The block has been correctly initialized and the sout_access_out_t is valid
        cvr(unsafe { sout_AccessOutWrite(self.0.as_ptr(), block.into_raw_ptr()) })
    }

    // TODO: read

    /// Can control pace
    #[doc(alias = "sout_AccessOutCanControlPace")]
    pub fn can_control_pace(&mut self) -> bool {
        // SAFETY: The sout_access_out_t is valid
        (unsafe { sout_AccessOutCanControlPace(self.0.as_ptr()) }) as bool
    }
}

impl Drop for SoutAccessOut<'_> {
    fn drop(&mut self) {
        // SAFETY: We are in drop no-one else has access to it, we can delete it safely
        unsafe { sout_AccessOutDelete(self.0.as_ptr()) }
    }
}
