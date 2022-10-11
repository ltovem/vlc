# ebml

ifeq ($(call need_pkg,"libebml >= 1.3.8"),)
PKGS_FOUND += ebml
endif

ebml: $(SRC)/ebml/unpack.mak

.ebml: ebml toolchain.cmake
	$(CMAKECLEAN)
	$(HOSTVARS_PIC) $(CMAKE)
	+$(CMAKEBUILD)
	+$(CMAKEBUILD) --target install
	touch $@
