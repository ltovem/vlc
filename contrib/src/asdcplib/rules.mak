# asdcplib

ASDCPLIB_VERSION := 2_12_3
ASDCPLIB_URL := $(GITHUB)/cinecert/asdcplib/archive/refs/tags/rel_$(ASDCPLIB_VERSION).tar.gz

ifndef HAVE_IOS
ifndef HAVE_ANDROID
PKGS += asdcplib
endif
endif

DEPS_asdcplib = nettle $(DEPS_nettle)
ifdef HAVE_WINSTORE
# asdcplib uses CreateFileW
DEPS_asdcplib += alloweduwp $(DEPS_alloweduwp)
endif

ifeq ($(call need_pkg,"asdcplib >= 1.12"),)
PKGS_FOUND += asdcplib
endif

ASDCPLIB_CXXFLAGS := $(CXXFLAGS) -Dregister=
ifdef HAVE_WIN32
ASDCPLIB_CXXFLAGS += -DKM_WIN32_UTF8
endif

$(TARBALLS)/asdcplib-rel_$(ASDCPLIB_VERSION).tar.gz:
	$(call download_pkg,$(ASDCPLIB_URL),asdcplib)

.sum-asdcplib: asdcplib-rel_$(ASDCPLIB_VERSION).tar.gz

asdcplib: asdcplib-rel_$(ASDCPLIB_VERSION).tar.gz .sum-asdcplib
	$(UNPACK)
	$(APPLY) $(SRC)/asdcplib/port-to-nettle.patch
	$(APPLY) $(SRC)/asdcplib/static-programs.patch
	$(APPLY) $(SRC)/asdcplib/adding-pkg-config-file.patch
	$(APPLY) $(SRC)/asdcplib/win32-cross-compilation.patch
	$(APPLY) $(SRC)/asdcplib/win32-dirent.patch
	$(APPLY) $(SRC)/asdcplib/0001-call-GetModuleFileNameA-explicitly.patch
	$(MOVE)

ASDCPLIB_CONF := --enable-freedist --enable-dev-headers --with-nettle=$(PREFIX)

ASDCPLIB_CONF += CXXFLAGS="$(ASDCPLIB_CXXFLAGS)"

.asdcplib: asdcplib
	$(RECONF)
	$(MAKEBUILDDIR)
	$(MAKECONFIGURE) $(ASDCPLIB_CONF)
	+$(MAKEBUILD) bin_PROGRAMS=
	+$(MAKEBUILD) bin_PROGRAMS= install
	touch $@
