use std::env;
use std::fs::File;
use std::io::{BufWriter, Result, Write};
use std::path::{Path, PathBuf};

fn main() {
    let vlc_fourcc_h_path = Path::new("../../../include/vlc_fourcc.h");
    let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());

    println!("cargo:rerun-if-changed={}", vlc_fourcc_h_path.display());

    generate_fourcc(&vlc_fourcc_h_path, out_path.join("fourcc.rs"))
        .expect("coundn't generate the fourcc bindings!");
}

fn generate_fourcc(from: &Path, to: PathBuf) -> Result<()> {
    let mut output_file = BufWriter::new(File::create(to).expect("cannot open vlc_fourcc.h"));
    let fourcc_include_contents = std::fs::read_to_string(from).unwrap();

    writeln!(
        output_file,
        "// These constants have been automaticly generated by vlcrs-fourcc"
    )?;
    writeln!(
        output_file,
        "// in it's build.rs, do not modify them manually!\n"
    )?;
    writeln!(output_file, "{}", "impl FourCC {")?;

    for line in fourcc_include_contents.lines() {
        if line.starts_with("#define VLC_CODEC") {
            // VLC_CODEC_....
            let name = line
                .strip_prefix("#define ")
                .unwrap()
                .split_whitespace()
                .next()
                .unwrap();

            // ('u','n','d','f')
            let fourcc = line.rsplit("VLC_FOURCC").next().unwrap();

            writeln!(
                output_file,
                "    pub const {}: FourCC = fourcc!{};",
                name, fourcc
            )
            .unwrap();
        }
    }

    writeln!(output_file, "{}", "}")?;

    Ok(())
}
