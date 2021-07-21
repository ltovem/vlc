# openfst

OPENFST_VERSION := master
OPENFST_URL := https://github.com/alphacep/openfst/archive/$(OPENFST_VERSION).tar.gz

ifeq ($(call need_pkg,"openfst"),)
PKGS_FOUND += openfst
endif

$(TARBALLS)/openfst-$(OPENFST_VERSION).tar.gz:
	$(call download_pkg,$(OPENFST_URL),openfst)

.sum-openfst: openfst-$(OPENFST_VERSION).tar.gz

openfst: openfst-$(OPENFST_VERSION).tar.gz .sum-openfst
	$(UNPACK)
	$(MOVE)

.openfst: openfst toolchain.cmake
	cd $< && $(HOSTVARS_PIC) $(CMAKE) -DENABLE_WIN32_IO=OFF
	cd $< && $(CMAKEBUILD) . --target install
	touch $@
