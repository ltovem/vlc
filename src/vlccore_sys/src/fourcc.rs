#[derive(Debug)]
pub struct FourCC(pub u32);

#[macro_export]
macro_rules! vlc_fourcc (
  ($a: expr, $b: expr, $c: expr, $d: expr) => {
    FourCC($a as u32 | (($b as u32) << 8) |
      (($c as u32) << 16) | (($d as u32) << 24))
  }
);
