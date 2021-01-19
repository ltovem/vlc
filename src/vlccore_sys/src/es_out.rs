use crate::block::*;
use crate::es_format::EsFormat;
use crate::vlc_core::*;
use crate::vlc_core_sys::*;

pub type CEsOut = es_out_t;
pub type CEsOutId = es_out_id_t;

impl CEsOut {
    /// Add a new es_format_t and returns his CEsOutId if it works
    pub fn add(&mut self, fmt: &EsFormat) -> Box<CEsOutId> {
        let fmt = fmt.as_ffi();
        let ptr = unsafe { es_out_Add(self, &fmt) };
        if ptr.is_null() {
            panic!("es_out_Add failed");
        } else {
            unsafe { Box::from_raw(ptr) }
        }
    }

    /// bytes sent.
    pub fn send(&mut self, id: &mut CEsOutId, data: Box<CBlock>) -> i32 {
        let data = Box::leak(data);
        unsafe { es_out_Send(self, id, data) }
    }

    /// Set pcr
    pub fn set_pcr(&mut self, pcr: vlc_tick_t) {
        unsafe {
            es_out_Control(self, es_out_query_e::ES_OUT_SET_PCR as i32, pcr);
        }
    }
}
