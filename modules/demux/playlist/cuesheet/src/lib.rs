mod cuesheet;
mod parse;

#[cfg(test)]
mod tests {
    use crate::parse::parse_line_alt;
    #[test]
    fn test_parse_line_alt() {
        let parts = parse_line_alt("FILE \"Live in Berlin\" MP3");
        assert_eq!(3, parts.len());
        assert_eq!("FILE", parts[0]);
        assert_eq!("Live in Berlin", parts[1]);
        assert_eq!("MP3", parts[2]);
    }
}

#[cfg(cargo_c)]
mod capi;
