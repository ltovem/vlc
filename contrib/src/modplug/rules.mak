# modplug

MODPLUG_VERSION := 0.8.9.0
MODPLUG_URL := $(SF)/modplug-xmms/libmodplug-$(MODPLUG_VERSION).tar.gz

PKGS += modplug
ifeq ($(call need_pkg,"libmodplug >= 0.8.9.0"),)
PKGS_FOUND += modplug
endif

MODPLUG_CXXFLAGS := $(CXXFLAGS) -std=gnu++98

$(TARBALLS)/libmodplug-$(MODPLUG_VERSION).tar.gz:
	$(call download_pkg,$(MODPLUG_URL),modplug)

.sum-modplug: libmodplug-$(MODPLUG_VERSION).tar.gz

libmodplug: libmodplug-$(MODPLUG_VERSION).tar.gz .sum-modplug
	$(UNPACK)
	$(APPLY) $(SRC)/modplug/modplug-win32-static.patch
	$(APPLY) $(SRC)/modplug/macosx-do-not-force-min-version.patch
	$(APPLY) $(SRC)/modplug/fix-endianness-check.diff
ifdef HAVE_MACOSX
	$(APPLY) $(SRC)/modplug/mac-use-c-stdlib.patch
endif
	$(APPLY) $(SRC)/modplug/0001-Fix-bogus-cast-into-potential-WCHAR-string.patch
	$(APPLY) $(SRC)/modplug/0002-replace-lstrcpyn-with-strncpy.patch
	$(APPLY) $(SRC)/modplug/0003-replace-lstrcpy-with-strcpy.patch
	$(APPLY) $(SRC)/modplug/0004-replace-wsprintf-with-sprintf.patch
	$(APPLY) $(SRC)/modplug/0005-Define-WAVE_FORMAT_PCM-when-it-s-not-defined.patch
	$(APPLY) $(SRC)/modplug/0006-Replace-GlobalAllocPtr-calls-with-calloc.patch
	$(call pkg_static,"libmodplug.pc.in")
	$(MOVE)

MODPLUG_CONF := CXXFLAGS="$(MODPLUG_CXXFLAGS)"

.modplug: libmodplug
	$(RECONF)
	$(MAKEBUILDDIR)
	$(MAKECONFIGURE) $(MODPLUG_CONF)
	+$(MAKEBUILD)
	+$(MAKEBUILD) install
	touch $@
