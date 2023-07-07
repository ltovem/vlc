//! Rational.

use vlcrs_core_sys::vlc_rational_t;

/// A representation of a rational number
#[doc(alias = "vlc_rational_t")]
#[derive(Debug, Clone, Copy)]
pub struct Rational {
    pub num: u32,
    pub den: u32,
}

impl Rational {
    /// Create a new rational number from a numerator and denominator
    pub fn new(num: u32, den: u32) -> Rational {
        Rational { num, den }
    }
}

impl From<vlc_rational_t> for Rational {
    fn from(vlc_rational: vlc_rational_t) -> Self {
        Rational {
            num: vlc_rational.num,
            den: vlc_rational.den,
        }
    }
}

impl Into<vlc_rational_t> for Rational {
    fn into(self) -> vlc_rational_t {
        vlc_rational_t {
            num: self.num,
            den: self.den,
        }
    }
}
