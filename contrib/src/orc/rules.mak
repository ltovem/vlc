# orc

ORC_VERSION := 0.4.32
ORC_URL := https://gitlab.freedesktop.org/gstreamer/orc/-/archive/$(ORC_VERSION)/orc-$(ORC_VERSION).tar.bz2

ifeq ($(call need_pkg,"orc-0.4"),)
PKGS_FOUND += orc
endif

$(TARBALLS)/orc-$(ORC_VERSION).tar.bz2:
	$(call download_pkg,$(ORC_URL),orc)

.sum-orc: orc-$(ORC_VERSION).tar.bz2

orc: orc-$(ORC_VERSION).tar.bz2 .sum-orc
	$(UNPACK)
	$(APPLY) $(SRC)/orc/0001-meson-export-ORC_STATIC_COMPILATION-in-the-pkgconfig.patch
	$(MOVE)

.orc: orc
	cd $< && rm -rf ./build
	cd $< && $(HOSTVARS_MESON) $(MESON) -Dauto_features=disabled build
	cd $< && cd build && ninja install
	touch $@
