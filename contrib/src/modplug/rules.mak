# modplug

MODPLUG_GITURL := $(GITHUB)/Konstanty/libmodplug.git
MODPLUG_HASH := 63133e82b6cfc76f50cc76ffa2cf4866d1e08ec5
MODPLUG_VERSION := 0.8.9.0
MODPLUG_URL := $(SF)/modplug-xmms/libmodplug-$(MODPLUG_VERSION).tar.gz

PKGS += modplug
ifeq ($(call need_pkg,"libmodplug >= 0.8.9.0"),)
PKGS_FOUND += modplug
endif

$(TARBALLS)/libmodplug-$(MODPLUG_VERSION).tar.gz:
	$(call download_pkg,$(MODPLUG_URL),modplug)

$(TARBALLS)/libmodplug-$(MODPLUG_HASH).tar.xz:
	$(call download_git,$(MODPLUG_GITURL),,$(MODPLUG_HASH))

# .sum-modplug: libmodplug-$(MODPLUG_VERSION).tar.gz

.sum-modplug: libmodplug-$(MODPLUG_HASH).tar.xz
	$(call check_githash,$(MODPLUG_HASH))
	touch $@

libmodplug: libmodplug-$(MODPLUG_HASH).tar.xz .sum-modplug
	$(UNPACK)
	$(APPLY) $(SRC)/modplug/modplug-win32-static.patch
ifdef HAVE_MACOSX
	$(APPLY) $(SRC)/modplug/mac-use-c-stdlib.patch
endif
	$(APPLY) $(SRC)/modplug/0001-Fix-libm-detection-when-compiling-with-Android-CMake.patch
	$(call pkg_static,"libmodplug.pc.in")
	$(MOVE)

.modplug: libmodplug toolchain.cmake
	$(CMAKECLEAN)
	$(HOSTVARS) $(CMAKE)
	+$(CMAKEBUILD)
	$(CMAKEINSTALL)
	touch $@
