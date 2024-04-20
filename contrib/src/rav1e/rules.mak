# rav1e

RAV1E_VERSION := 0.7.1
RAV1E_URL := https://crates.io/api/v1/crates/rav1e/$(RAV1E_VERSION)/download

ifdef BUILD_RUST
ifdef BUILD_ENCODERS
# Rav1e is not linking correctly on iOS arm64
ifndef HAVE_IOS
PKGS += rav1e
endif
endif
endif

ifeq ($(call need_pkg,"rav1e"),)
PKGS_FOUND += rav1e
endif

$(TARBALLS)/rav1e-$(RAV1E_VERSION).tar.gz:
	$(call download_pkg,$(RAV1E_URL),rav1e)

.sum-rav1e: rav1e-$(RAV1E_VERSION).tar.gz

RAV1E_FEATURES=--features=asm

DEPS_rav1e = rav1e-vendor $(DEPS_rav1e-vendor)

rav1e: rav1e-$(RAV1E_VERSION).tar.gz .sum-rav1e
	$(UNPACK)
ifdef HAVE_WIN32
ifndef HAVE_WIN64
	$(APPLY) $(SRC)/rav1e/unwind-resume-stub.patch
endif
endif
	$(call cargo_vendor_setup,$(UNPACK_DIR),$@)
	$(MOVE)

.rav1e: rav1e .cargo
	+cd $< && $(CARGOC_INSTALL) --no-default-features $(RAV1E_FEATURES)
# No gcc in Android NDK25
ifdef HAVE_ANDROID
	sed -i -e 's/ -lgcc//g' $(PREFIX)/lib/pkgconfig/rav1e.pc
endif
	touch $@
