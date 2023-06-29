# libiconv
LIBICONV_VERSION := 1.17
LIBICONV_URL := $(GNU)/libiconv/libiconv-$(LIBICONV_VERSION).tar.gz

PKGS += iconv
# iconv cannot be detect with pkg-config, but it is mandated by POSIX.
# Hard-code based on the operating system.
ifndef HAVE_WIN32
ifndef HAVE_ANDROID
PKGS_FOUND += iconv
else
ifeq ($(shell expr "$(ANDROID_API)" '>=' '28'), 1)
PKGS_FOUND += iconv
endif
endif
endif

DEPS_iconv :=
ifdef HAVE_WINSTORE
# gnulib uses GetFileInformationByHandle, CreateFileW
DEPS_iconv += alloweduwp $(DEPS_alloweduwp)
endif

$(TARBALLS)/libiconv-$(LIBICONV_VERSION).tar.gz:
	$(call download_pkg,$(LIBICONV_URL),iconv)

.sum-iconv: libiconv-$(LIBICONV_VERSION).tar.gz

iconv: libiconv-$(LIBICONV_VERSION).tar.gz .sum-iconv
	$(UNPACK)
	$(APPLY) $(SRC)/iconv/bins.patch

	$(UPDATE_AUTOCONFIG)
	cd $(UNPACK_DIR) && cp config.guess config.sub build-aux \
	                 && mv config.guess config.sub libcharset/build-aux
	$(MOVE)

ICONV_CONF := --disable-nls

.iconv: iconv
	$(MAKEBUILDDIR)
	$(MAKECONFIGURE) $(ICONV_CONF)
	+$(MAKEBUILD)
	+$(MAKEBUILD) install
	touch $@
