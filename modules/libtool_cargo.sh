#!/bin/bash

# This script generate a libtool convinence library from a Cargo (Rust project)

set -e # Automaticly exit on error

CARGO_RUSTC_CMD=${@:1:$#-2} # all other args
MODULE_PROJECT_DIR=${@:$#-1:1} # second last argument
LT_CONVENIENCE=${@:$#:1} # last argument

# Build the project a first time without anything special
$CARGO_RUSTC_CMD --manifest-path="$MODULE_PROJECT_DIR/Cargo.toml"

# "Build" the project a second time and fetch the native-static-libs to link with
NATIVE_STATIC_LIBS=$($CARGO_RUSTC_CMD --manifest-path="$MODULE_PROJECT_DIR/Cargo.toml" --quiet -- --print native-static-libs 2>&1 | grep "native-static-libs" | sed "s/note: native-static-libs://" | sed "s/-lvlccore//")

STATIC_LIB_NAME=$(echo $LT_CONVENIENCE | sed "s/\.la/\.a/")
STATIC_LIB_DEP=$(echo $LT_CONVENIENCE | sed "s/\.la/\.d/")
CARGO_STATIC_LIB_PATH="$CARGO_TARGET_DIR/$RUSTTARGET/release/$STATIC_LIB_NAME"
CARGO_STATIC_DEP_PATH="$CARGO_TARGET_DIR/$RUSTTARGET/release/$STATIC_LIB_DEP"

LT_VERSION=$(../libtool --version | grep "libtool")

cat <<EOF > $LT_CONVENIENCE
# $LT_CONVENIENCE - a libtool library file
# Generated by $LT_VERSION

# The name that we can dlopen(3).
dlname=''

# Names of this library.
library_names=''

# The name of the static archive.
old_library='$STATIC_LIB_NAME'

# Linker flags that cannot go in dependency_libs.
inherited_linker_flags=''

# Libraries that this one depends upon.
dependency_libs=' $NATIVE_STATIC_LIBS'

# Names of additional weak libraries provided by this library
weak_library_names=''

# Version information
current=0
age=0
revision=0

# Is this an already installed library?
installed=no

# Should we warn about portability when linking against -modules?
shouldnotlink=no

# Files to dlopen/dlpreopen
dlopen=''
dlpreopen=''

# Directory that this library needs to be installed in:
libdir=''
EOF

mkdir -p "./.libs"
ln -sf "../$LT_CONVENIENCE" "./.libs/$LT_CONVENIENCE"
cp "$CARGO_STATIC_LIB_PATH" "./.libs/$STATIC_LIB_NAME"

echo -n "$LT_CONVENIENCE:" > "./.libs/$STATIC_LIB_DEP"
cat "$CARGO_STATIC_DEP_PATH" | cut -d ':' -f2 >> "./.libs/$STATIC_LIB_DEP"
