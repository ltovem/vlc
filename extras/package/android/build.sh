set -eux

VLC_BUILD_DIR="$(pwd)"
VLC_SRC_DIR="$(cd "$(dirname $0)/../../../"; pwd)"

VLC_MIN_SDK=${ANDROID_API}
VLC_SDK=${ANDROID_API}

VLC_BUILD_TOOLS_VERSION="30.0.3"
VLC_BUILD_ARCH="darwin-x86_64"
VLC_HOST_ARCH="armv7a"

TRIPLET_HOST="${VLC_HOST_ARCH}-linux-android"

ABI_SUFFIX=""
TOOL_PREFIX="${VLC_HOST_ARCH}-linux-android-"
if [ "$VLC_HOST_ARCH" = "armv7a" ]; then
    TOOL_PREFIX="arm-linux-androideabi-"
    ABI_SUFFIX="eabi"
fi

export PATH="${ANDROID_SDK}/build-tools/${VLC_BUILD_TOOLS_VERSION}/:${PATH}"
export PATH="${ANDROID_NDK}/toolchains/llvm/prebuilt/${VLC_BUILD_ARCH}/bin/:${PATH}"
export PATH="${VLC_SRC_DIR}/extras/tools/build/bin/:${PATH}"

export PKG_CONFIG_PATH=""
export PKG_CONFIG_LIBDIR=""
export PKG_CONFIG_SYSROOT_DIR=""

export CC=${VLC_HOST_ARCH}-linux-android${ABI_SUFFIX}${VLC_MIN_SDK}-clang
export CXX=${VLC_HOST_ARCH}-linux-android${ABI_SUFFIX}${VLC_MIN_SDK}-clang++
export AR="${TOOL_PREFIX}ar"
export AS="${TOOL_PREFIX}as"
export RANLIB="${TOOL_PREFIX}ranlib"

if [ ! -f ${VLC_BUILD_DIR}/config.h ]; then
${VLC_SRC_DIR}/configure \
    --host=${VLC_HOST_ARCH}-linux-android${ABI_SUFFIX}${ANDROID_API} \
    --with-android-jar=${ANDROID_SDK}/platforms/android-${VLC_SDK}/android.jar \
    --disable-vlc --disable-chromecast --disable-avcodec --disable-avformat \
    --disable-a52 --disable-swscale --disable-lua --disable-xcb --disable-v4l2
fi

V=1 VERBOSE=1 make
