VULKAN_LOADER_VERSION := 1.3.227
VULKAN_LOADER_URL := $(GITHUB)/KhronosGroup/Vulkan-Loader/archive/v$(VULKAN_LOADER_VERSION).tar.gz

DEPS_vulkan-loader = vulkan-headers $(DEPS_vulkan-headers)

# On WIN32 platform, we don't know where to find the loader
# so always build it for the Vulkan module.
ifdef HAVE_WIN32_DESKTOP
PKGS += vulkan-loader
endif

ifeq ($(call need_pkg,"vulkan"),)
PKGS_FOUND += vulkan-loader
endif

# On Android, vulkan-loader is available on the system itself.
ifdef HAVE_ANDROID
PKGS_FOUND += vulkan-loader
endif

ifndef HAVE_ANDROID
ifdef HAVE_LINUX
DEPS_vulkan-loader += xcb $(DEPS_xcb)
endif
endif

VULKAN_LOADER_CONF := \
	-DENABLE_STATIC_LOADER=ON \
	-DBUILD_SHARED_LIBS=OFF \
	-DENABLE_WERROR=OFF \
	-DBUILD_TESTS=OFF \
	-DBUILD_LOADER=ON \
	-DCMAKE_ASM_COMPILER="$(AS)"

ifndef HAVE_VISUALSTUDIO
# can only use masm or jwasm on Windows
VULKAN_LOADER_CONF += -DUSE_MASM=OFF
endif

$(TARBALLS)/Vulkan-Loader-$(VULKAN_LOADER_VERSION).tar.gz:
	$(call download_pkg,$(VULKAN_LOADER_URL),vulkan-loader)

.sum-vulkan-loader: Vulkan-Loader-$(VULKAN_LOADER_VERSION).tar.gz

vulkan-loader: Vulkan-Loader-$(VULKAN_LOADER_VERSION).tar.gz .sum-vulkan-loader
	$(UNPACK)
ifeq ($(HOST),i686-w64-mingw32)
	cp -v $(SRC)/vulkan-loader/libvulkan-32.def $(UNPACK_DIR)/loader/vulkan-1.def
endif
	$(MOVE)

# Needed for the loader's cmake script to find the registry files
VULKAN_LOADER_ENV_CONF = \
	VULKAN_HEADERS_INSTALL_DIR="$(PREFIX)"

.vulkan-loader: vulkan-loader toolchain.cmake
	$(CMAKECLEAN)
	$(VULKAN_LOADER_ENV_CONF) $(HOSTVARS) $(CMAKE) $(VULKAN_LOADER_CONF)
	+$(CMAKEBUILD)

	$(call pkg_static,"build/loader/vulkan.pc")
	+$(CMAKEBUILD)
	+$(CMAKEBUILD) --target install
	touch $@
