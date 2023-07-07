//! es_out

use vlcrs_core_sys::es_out_Add;
use vlcrs_core_sys::es_out_Del;
use vlcrs_core_sys::es_out_GetESState;
use vlcrs_core_sys::es_out_ResetPCR;
use vlcrs_core_sys::es_out_RestartES;
use vlcrs_core_sys::es_out_Send;
use vlcrs_core_sys::es_out_SetES;
use vlcrs_core_sys::es_out_SetESDefault;
use vlcrs_core_sys::es_out_SetESState;
use vlcrs_core_sys::es_out_SetNextDisplayTime;
use vlcrs_core_sys::es_out_SetPCR;
use vlcrs_core_sys::es_out_UnsetES;

use std::ptr::NonNull;

use vlcrs_core_sys::es_out_id_t;
use vlcrs_core_sys::es_out_t;

use crate::es::EsFormat;
use crate::stream::Frame;
use crate::{
    error::{cvp, cvr, cvt, Result},
    tick::Tick,
};

/// `es_out_id_it`
// NOTE: This is volontarely *not* Clone and Copy to avoid misuse.
#[derive(Debug, PartialEq)]
#[doc(alias = "es_out_id_t")]
pub struct EsOutId(pub(crate) usize);

/// A Elementary Output Stream
///
/// Nearly an owned one but not quite.
#[repr(transparent)]
#[doc(alias = "es_out_t")]
#[derive(Debug)]
pub struct EsOut(pub(crate) NonNull<es_out_t>);

/// A Elementary Output Stream with facilities to handle `es_out_id_t`
#[derive(Debug)]
pub struct EsOutBaked<'a>(
    pub(crate) &'a mut EsOut,
    pub(crate) Vec<Option<NonNull<es_out_id_t>>>,
);

impl<'a> From<&'a mut EsOut> for EsOutBaked<'a> {
    fn from(value: &'a mut EsOut) -> Self {
        EsOutBaked(value, Vec::new())
    }
}

impl EsOutBaked<'_> {
    pub fn add(&mut self, es_format: &EsFormat) -> Result<EsOutId> {
        // SAFETY: The es_out and es_format are well-defined
        let res = unsafe { es_out_Add(self.0 .0.as_ptr(), es_format.as_ptr()) };

        let es_out_id_t = cvp(res)?;

        let id = self.1.len();
        self.1.push(Some(es_out_id_t));
        Ok(EsOutId(id))
    }

    pub fn del(&mut self, es_out_id: EsOutId) -> Result<()> {
        // TODO: Return error instead
        let es_out_id_t = std::mem::take(&mut self.1[es_out_id.0]).unwrap();

        // SAFETY: The es_out and es_format are well-defined
        unsafe { es_out_Del(self.0 .0.as_ptr(), es_out_id_t.as_ptr()) };

        Ok(())
    }

    pub fn send(&mut self, es_out_id: &EsOutId, frame: Frame) -> Result<usize> {
        // TODO: Return error instead
        let es_out_id_t = self.1[es_out_id.0].unwrap();

        // SAFETY: The es_out and es_format are well-defined
        let res = unsafe {
            es_out_Send(
                self.0 .0.as_ptr(),
                es_out_id_t.as_ptr(),
                frame.into_raw_ptr(),
            )
        };

        cvr(res as _)
    }

    pub fn set_pcr(&mut self, tick: Tick) -> Result<()> {
        cvt(unsafe { es_out_SetPCR(self.0 .0.as_ptr(), tick.0) })
    }

    pub fn reset_pcr(&mut self) -> Result<()> {
        cvt(unsafe { es_out_ResetPCR(self.0 .0.as_ptr()) })
    }

    pub fn set_next_display_time(&mut self, ndt: Tick) -> Result<()> {
        cvt(unsafe { es_out_SetNextDisplayTime(self.0 .0.as_ptr(), ndt.0) })
    }

    pub fn set_es(&mut self, es_out_id: &EsOutId) -> Result<()> {
        // TODO: Return error instead
        let es_out_id_t = self.1[es_out_id.0].unwrap();

        cvt(unsafe { es_out_SetES(self.0 .0.as_ptr(), es_out_id_t.as_ptr()) })
    }

    pub fn unset_es(&mut self, es_out_id: &EsOutId) -> Result<()> {
        // TODO: Return error instead
        let es_out_id_t = self.1[es_out_id.0].unwrap();

        cvt(unsafe { es_out_UnsetES(self.0 .0.as_ptr(), es_out_id_t.as_ptr()) })
    }

    pub fn restart_es(&mut self, es_out_id: &EsOutId) -> Result<()> {
        // TODO: Return error instead
        let es_out_id_t = self.1[es_out_id.0].unwrap();

        cvt(unsafe { es_out_RestartES(self.0 .0.as_ptr(), es_out_id_t.as_ptr()) })
    }

    pub fn default_es(&mut self, es_out_id: &EsOutId) -> Result<()> {
        // TODO: Return error instead
        let es_out_id_t = self.1[es_out_id.0].unwrap();

        cvt(unsafe { es_out_SetESDefault(self.0 .0.as_ptr(), es_out_id_t.as_ptr()) })
    }

    pub fn set_state_es(&mut self, es_out_id: &EsOutId, forced: bool) -> Result<()> {
        // TODO: Return error instead
        let es_out_id_t = self.1[es_out_id.0].unwrap();

        cvt(unsafe { es_out_SetESState(self.0 .0.as_ptr(), es_out_id_t.as_ptr(), forced) })
    }

    pub fn get_state_es(&mut self, es_out_id: &EsOutId) -> bool {
        // TODO: Return error instead
        let es_out_id_t = self.1[es_out_id.0].unwrap();

        unsafe { es_out_GetESState(self.0 .0.as_ptr(), es_out_id_t.as_ptr()) }
    }
}
