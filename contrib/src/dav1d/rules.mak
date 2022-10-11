# libdav1d

PKGS += dav1d
ifeq ($(call need_pkg,"dav1d"),)
PKGS_FOUND += dav1d
endif

dav1d: $(SRC)/dav1d/unpack.mak

DAV1D_CONF = -D enable_tests=false -D enable_tools=false

.dav1d: dav1d crossfile.meson
	$(MESONCLEAN)
	$(HOSTVARS_MESON) $(MESON) $(DAV1D_CONF)
	$(MESONBUILD)
	touch $@
