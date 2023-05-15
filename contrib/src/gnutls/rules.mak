# GnuTLS

GNUTLS_VERSION_MAJ := 3.7
GNUTLS_VERSION := $(GNUTLS_VERSION_MAJ).9
GNUTLS_URL := $(GNUGPG)/gnutls/v$(GNUTLS_VERSION_MAJ)/gnutls-$(GNUTLS_VERSION).tar.xz

ifdef BUILD_NETWORK
ifndef HAVE_DARWIN_OS
PKGS += gnutls
endif
endif
ifeq ($(call need_pkg,"gnutls >= 3.5.0"),)
PKGS_FOUND += gnutls
endif

$(TARBALLS)/gnutls-$(GNUTLS_VERSION).tar.xz:
	$(call download_pkg,$(GNUTLS_URL),gnutls)

.sum-gnutls: gnutls-$(GNUTLS_VERSION).tar.xz

gnutls: gnutls-$(GNUTLS_VERSION).tar.xz .sum-gnutls
	$(UNPACK)

	# fix forbidden UWP call which can't be upstreamed as they won't
	# differentiate for winstore, only _WIN32_WINNT
	$(APPLY) $(SRC)/gnutls/0001-fcntl-do-not-call-GetHandleInformation-in-Winstore-a.patch

	# forbidden RtlSecureZeroMemory call in winstore builds
	$(APPLY) $(SRC)/gnutls/0001-explicit_bzero-Do-not-call-SecureZeroMemory-on-UWP-b.patch

	# disable the dllimport in static linking (pkg-config --static doesn't handle Cflags.private)
	sed -i.orig -e s/"_SYM_EXPORT __declspec(dllimport)"/"_SYM_EXPORT"/g $(UNPACK_DIR)/lib/includes/gnutls/gnutls.h.in

	# use CreateFile2 in Win8 as CreateFileW is forbidden in UWP
	$(APPLY) $(SRC)/gnutls/0001-Use-CreateFile2-in-UWP-builds.patch

	# $(UPDATE_AUTOCONFIG)
	$(MOVE)

GNUTLS_CONF := \
	--disable-gtk-doc \
	--without-p11-kit \
	--disable-cxx \
	--disable-srp-authentication \
	--disable-anon-authentication \
	--disable-openssl-compatibility \
	--disable-guile \
	--disable-nls \
	--without-libintl-prefix \
	--disable-doc \
	--disable-tools \
	--disable-tests \
	--with-included-libtasn1 \
	--with-included-unistring

DEPS_gnutls = nettle $(DEPS_nettle)
ifdef HAVE_WINSTORE
# gnulib uses GetFileInformationByHandle
DEPS_gnutls += alloweduwp $(DEPS_alloweduwp)
endif

ifdef HAVE_ANDROID
GNUTLS_ENV := gl_cv_header_working_stdint_h=yes
endif
ifdef HAVE_WIN32
	GNUTLS_CONF += --without-idn
ifeq ($(ARCH),aarch64)
	# Gnutls' aarch64 assembly unconditionally uses ELF specific directives
	GNUTLS_CONF += --disable-hardware-acceleration
endif
endif

.gnutls: gnutls
	$(MAKEBUILDDIR)
	$(GNUTLS_ENV) $(MAKECONFIGURE) $(GNUTLS_CONF)
ifdef HAVE_DARWIN_OS
	# Add missing frameworks to Libs.private for Darwin
	cd $< && sed -i.orig -e s/"Libs.private:"/"Libs.private: -framework Security -framework CoreFoundation"/g $(BUILD_DIRUNPACK)/lib/gnutls.pc
endif
	$(call pkg_static,"$(BUILD_DIRUNPACK)/lib/gnutls.pc")
	+$(MAKEBUILD) -C gl
	+$(MAKEBUILD) -C lib
	+$(MAKEBUILD) -C gl install
	+$(MAKEBUILD) -C lib install
	touch $@
