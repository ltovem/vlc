extern crate bindgen;
use std::collections::HashSet;

// This is IgnoredMacro with his ParseCallbacks impl is used to ignore macros
// to avoid collisions.
#[derive(Debug)]
struct IgnoreMacros(HashSet<String>);

impl bindgen::callbacks::ParseCallbacks for IgnoreMacros {
    fn will_parse_macro(&self, name: &str) -> bindgen::callbacks::MacroParsingBehavior {
        if self.0.contains(name) {
            bindgen::callbacks::MacroParsingBehavior::Ignore
        } else {
            bindgen::callbacks::MacroParsingBehavior::Default
        }
    }
}

fn main() {
    let ignored_macros = IgnoreMacros(
        vec![
            "FP_INFINITE".into(),
            "FP_NAN".into(),
            "FP_NORMAL".into(),
            "FP_SUBNORMAL".into(),
            "FP_ZERO".into(),
            "IPPORT_RESERVED".into(),
        ]
        .into_iter()
        .collect(),
    );

    /// Bindgen is used to generated FFI bindgins. We split this job in 2 parts
    /// - Generation of enum/struct
    /// - Generation of functions
    /// By doing so we can make functions private to limit error usage of them
    bindgen::builder()
        .clang_arg("-I../../include")
        .header("../../include/vlc_common.h")
        .header("../../include/vlc_block.h")
        .header("../../include/vlc_block_helper.h")
        .header("../../include/vlc_config.h")
        .header("../../include/vlc_demux.h")
        .header("../../include/vlc_es.h")
        .header("../../include/vlc_es_out.h")
        .header("../../include/vlc_fourcc.h")
        .header("../../include/vlc_input.h")
        .header("../../include/vlc_tick.h")
        .header("../../include/vlc_input_item.h")
        .header("../../include/vlc_interface.h")
        .header("../../include/vlc_memstream.h")
        .header("../../include/vlc_messages.h")
        .header("../../include/vlc_meta.h")
        .header("../../include/vlc_objects.h")
        .header("../../include/vlc_player.h")
        .header("../../include/vlc_playlist.h")
        .header("../../include/vlc_stream_extractor.h")
        .header("../../include/vlc_stream.h")
        .header("../../include/vlc_variables.h")
        .header("../../include/vlc_xml.h")
        .default_enum_style(bindgen::EnumVariation::Rust {
            non_exhaustive: true,
        })
        .blacklist_function("*")
        .parse_callbacks(Box::new(ignored_macros))
        .layout_tests(false)
        // Remove warnings during build time to improve readabilty
        .raw_line("#![allow(warnings)]")
        // To avoid generate useless code
        .whitelist_recursively(false)
        .generate()
        .unwrap()
        .write_to_file("src/vlc_core.rs");

    bindgen::builder()
        .clang_arg("-I../../include")
        .clang_arg("-lm")
        .header("../../include/vlc_common.h")
        .header("../../include/vlc_block.h")
        .header("../../include/vlc_block_helper.h")
        .header("../../include/vlc_config.h")
        .header("../../include/vlc_demux.h")
        .header("../../include/vlc_es.h")
        .header("../../include/vlc_es_out.h")
        .header("../../include/vlc_fourcc.h")
        .header("../../include/vlc_input.h")
        .header("../../include/vlc_tick.h")
        .header("../../include/vlc_input_item.h")
        .header("../../include/vlc_interface.h")
        .header("../../include/vlc_memstream.h")
        .header("../../include/vlc_messages.h")
        .header("../../include/vlc_meta.h")
        .header("../../include/vlc_objects.h")
        .header("../../include/vlc_player.h")
        .header("../../include/vlc_playlist.h")
        .header("../../include/vlc_stream.h")
        .header("../../include/vlc_url.h")
        .header("../../include/vlc_variables.h")
        .header("../../include/vlc_xml.h")
        .default_enum_style(bindgen::EnumVariation::Rust {
            non_exhaustive: true,
        })
        .whitelist_recursively(false)
        .blacklist_type("*")
        .raw_line("#![allow(warnings)]")
        .raw_line("use crate::vlc_core::*;")
        .generate()
        .unwrap()
        .write_to_file("src/vlc_core_sys.rs");
}
