# clapack

CLAPACK_VERSION := 3.2.1
CLAPACK_URL := https://github.com/alphacep/clapack/archive/v$(CLAPACK_VERSION).tar.gz

ifeq ($(call need_pkg,"clapack"),)
PKGS_FOUND += clapack
endif

$(TARBALLS)/clapack-$(CLAPACK_VERSION).tar.gz:
	$(call download_pkg,$(CLAPACK_URL),clapack)

.sum-clapack: clapack-$(CLAPACK_VERSION).tar.gz

clapack: clapack-$(CLAPACK_VERSION).tar.gz .sum-clapack
	$(UNPACK)
	$(MOVE)

.clapack: clapack toolchain.cmake
	cd $< && $(HOSTVARS_PIC) $(CMAKE) -DENABLE_WIN32_IO=OFF
	cd $< && $(CMAKEBUILD) . --target install
	touch $@
