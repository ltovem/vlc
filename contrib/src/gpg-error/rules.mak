# GPGERROR
GPGERROR_VERSION := 1.27
GPGERROR_URL := $(GNUGPG)/libgpg-error/libgpg-error-$(GPGERROR_VERSION).tar.bz2

$(TARBALLS)/libgpg-error-$(GPGERROR_VERSION).tar.bz2:
	$(call download_pkg,$(GPGERROR_URL),gpg-error)

ifeq ($(call need_pkg,"gpg-error >= 1.27"),)
PKGS_FOUND += gpg-error
endif

DEPS_gpg-error =
ifdef HAVE_WINSTORE
# gpg-error uses CreateFileW, GetFileSize
DEPS_gpg-error += alloweduwp $(DEPS_alloweduwp)
endif

.sum-gpg-error: libgpg-error-$(GPGERROR_VERSION).tar.bz2

libgpg-error: libgpg-error-$(GPGERROR_VERSION).tar.bz2 .sum-gpg-error
	$(UNPACK)
	$(APPLY) $(SRC)/gpg-error/windres-make.patch
	$(APPLY) $(SRC)/gpg-error/winrt.patch
	$(APPLY) $(SRC)/gpg-error/missing-unistd-include.patch
	$(APPLY) $(SRC)/gpg-error/win32-unicode.patch
	$(APPLY) $(SRC)/gpg-error/version-bump-gawk-5.patch
	$(APPLY) $(SRC)/gpg-error/win32-extern-struct.patch
	$(APPLY) $(SRC)/gpg-error/darwin-triplet.patch
ifndef HAVE_WIN32
	cp -f -- "$(SRC)/gpg-error/lock-obj-pub.posix.h" \
		"$(UNPACK_DIR)/src/lock-obj-pub.native.h"
endif
	# gpg-error doesn't know about mingw32uwp but it's the same as mingw32
	cp -f -- "$(UNPACK_DIR)/src/syscfg/lock-obj-pub.mingw32.h" \
		"$(UNPACK_DIR)/src/syscfg/lock-obj-pub.mingw32uwp.h"
	$(APPLY) $(SRC)/gpg-error/gpg-error-uwp-fix.patch

	$(MOVE)

GPGERROR_CONF := \
	--disable-nls \
	--disable-languages \
	--disable-tests \
	--disable-doc

.gpg-error: libgpg-error
	$(RECONF)
	cd $< && $(HOSTVARS) ./configure $(HOSTCONF) $(GPGERROR_CONF)
	# pre_mkheader_cmds would delete our lock-obj-pub-native.h
	$(MAKE) -C $< pre_mkheader_cmds=true bin_PROGRAMS=
	$(MAKE) -C $< pre_mkheader_cmds=true bin_PROGRAMS= install
	touch $@
