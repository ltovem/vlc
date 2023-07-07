#!/usr/bin/env python3

# This script is meant to find the required native libs of an empty Rust crate
# compiled as a static lib.
#
# Usage: ./buildsystem/cargo-rustc-static-libs.py /usr/bin/cargo
# Usage: ./buildsystem/cargo-rustc-static-libs.py /usr/bin/cargo --target=x86_64-unknown-linux-gnu
#
# The result is then printed to the standard output.

import os, sys, json, subprocess, tempfile, argparse

NATIVE_STATIC_LIBS="native-static-libs"

parser = argparse.ArgumentParser()
parser.add_argument("cargo_cmds", nargs=argparse.REMAINDER)

args = parser.parse_args()

cargo_argv = args.cargo_cmds
cargo_argv.append("rustc")
cargo_argv.append("--message-format=json")
cargo_argv.append("--crate-type=staticlib")
cargo_argv.append("--quiet")
cargo_argv.append("--")
cargo_argv.append("--print=native-static-libs")

with tempfile.TemporaryDirectory() as tmpdir:
    os.chdir(tmpdir)

    with open("Cargo.toml", "w") as cargo_toml:
        cargo_toml.write("""
[package]
name = "native-static-libs"
version = "0.0.0"

[lib]
path = "lib.rs"
""")
    
    with open("lib.rs", "w") as lib_rs:
        lib_rs.write("#![allow(dead_code)] fn main(){}")

    # Execute the cargo build and redirect stdout (and not stderr)
    cargo_r = subprocess.run(cargo_argv, stdout=subprocess.PIPE)

    # We don't use `check=True in run because it raise an execption
    # and outputing a python traceback isn't useful at all.
    #
    # We also exit here so that the output o tmp dir is not cleared when
    # there is an error.
    if cargo_r.returncode != 0:
        print("command: {cargo_argv}", file=sys.stderr)
        print("cwd: {tmpdir}", file=sys.stderr)
        sys.exit(cargo_r.returncode)

# Get the jsons output
cargo_stdout = cargo_r.stdout.decode('utf-8')
cargo_jsons = [json.loads(line) for line in cargo_stdout.splitlines()]

# Print the last message with a `NATIVE_STATIC_LIBS` message
for j in reversed(cargo_jsons):
    if j["reason"] == "compiler-message":
        msg = j["message"]["message"]
        if msg.startswith(NATIVE_STATIC_LIBS):
            print(msg[len(NATIVE_STATIC_LIBS + ": "):])
