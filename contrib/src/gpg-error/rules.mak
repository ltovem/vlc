# GPGERROR
GPGERROR_VERSION := 1.47
GPGERROR_URL := $(GNUGPG)/libgpg-error/libgpg-error-$(GPGERROR_VERSION).tar.bz2

$(TARBALLS)/libgpg-error-$(GPGERROR_VERSION).tar.bz2:
	$(call download_pkg,$(GPGERROR_URL),gpg-error)

ifeq ($(call need_pkg,"gpg-error >= 1.27"),)
PKGS_FOUND += gpg-error
endif

.sum-gpg-error: libgpg-error-$(GPGERROR_VERSION).tar.bz2

libgpg-error: libgpg-error-$(GPGERROR_VERSION).tar.bz2 .sum-gpg-error
	$(UNPACK)

	# Set/GetThreadLocale() is not available in UWP we can use the default name
	$(APPLY) $(SRC)/gpg-error/libgpg-error-uwp-thread-lcid.patch

	# CreateProcess is limited in UWP, we should not use is in most cases
	$(APPLY) $(SRC)/gpg-error/libgpg-error-uwp-spawn-api.patch

	# Disable registry access in UWP as it's forbidden
	$(APPLY) $(SRC)/gpg-error/libgpg-error-uwp-registry.patch

	$(APPLY) $(SRC)/gpg-error/darwin-triplet.patch
ifndef HAVE_WIN32
	cp -f -- "$(SRC)/gpg-error/lock-obj-pub.posix.h" \
		"$(UNPACK_DIR)/src/lock-obj-pub.native.h"
endif
	cp -f -- "$(SRC)/gpg-error/lock-obj-pub.posix.h" \
		"$(UNPACK_DIR)/src/syscfg/lock-obj-pub.linux-android.h"

	# gpg-error doesn't know about mingw32uwp but it's the same as mingw32
	cp -f -- "$(UNPACK_DIR)/src/syscfg/lock-obj-pub.mingw32.h" \
		"$(UNPACK_DIR)/src/syscfg/lock-obj-pub.mingw32uwp.h"
	$(APPLY) $(SRC)/gpg-error/gpg-error-uwp-fix.patch

	$(APPLY) $(SRC)/gpg-error/0001-Use-a-WCHAR-string-for-the-temporary-file-path.patch

	# use CreateFile2 in Win8 as CreateFileW is forbidden in UWP
	$(APPLY) $(SRC)/gpg-error/gpg-error-createfile2.patch

	$(APPLY) $(SRC)/gpg-error/0003-Disable-GetUserNameW-calls-in-UWP-builds.patch
	$(APPLY) $(SRC)/gpg-error/0004-Fix-environment-usage-when-compiled-with-UNICODE-def.patch
	$(APPLY) $(SRC)/gpg-error/0005-Favor-GetCurrentProcessId-on-Windows.patch

	# don't use GetFileSize on UWP
	$(APPLY) $(SRC)/gpg-error/gpg-error-uwp-GetFileSize.patch

	$(APPLY) $(SRC)/gpg-error/0001-Disable-execv-fork-calls-on-tvOS.patch

	$(MOVE)

GPGERROR_CONF := \
	--disable-nls \
	--disable-languages \
	--disable-tests \
	--disable-doc

.gpg-error: libgpg-error
	$(MAKEBUILDDIR)
	$(MAKECONFIGURE) $(GPGERROR_CONF)
	# pre_mkheader_cmds would delete our lock-obj-pub-native.h
	+$(MAKEBUILD) pre_mkheader_cmds=true bin_PROGRAMS=
	+$(MAKEBUILD) pre_mkheader_cmds=true bin_PROGRAMS= install
	touch $@
