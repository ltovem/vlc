# OpenBLAS

OPENBLAS_VERSION := 0.3.13
OPENBLAS_URL := https://github.com/xianyi/OpenBLAS/archive/v$(OPENBLAS_VERSION).tar.gz

ifeq ($(call need_pkg,"openblas"),)
PKGS_FOUND += openblas
endif

$(TARBALLS)/OpenBLAS-$(OPENBLAS_VERSION).tar.gz:
	$(call download_pkg,$(OPENBLAS_URL),openblas)

.sum-openblas: OpenBLAS-$(OPENBLAS_VERSION).tar.gz

openblas: OpenBLAS-$(OPENBLAS_VERSION).tar.gz .sum-openblas
	$(UNPACK)
	$(MOVE)

.openblas: openblas toolchain.cmake
	cd $< && $(HOSTVARS_PIC) $(CMAKE) -DENABLE_WIN32_IO=OFF
	cd $< && $(CMAKEBUILD) . --target install
	touch $@
