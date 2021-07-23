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
	mkdir -p clapack/BUILD
	cd $< && $(HOSTVARS_PIC) $(CMAKE) 
	cd $< && $(CMAKEBUILD) . --target install
	touch $@


#	cd $< && $(CMAKEBUILD) . --target install
#	cd $< && make -j 2