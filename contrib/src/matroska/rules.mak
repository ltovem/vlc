# matroska

PKGS += matroska

ifeq ($(call need_pkg,"libmatroska"),)
PKGS_FOUND += matroska
endif

DEPS_matroska = ebml $(DEPS_ebml)

matroska: $(SRC)/matroska/unpack.mak

.matroska: matroska toolchain.cmake
	$(CMAKECLEAN)
	$(HOSTVARS_PIC) $(CMAKE)
	+$(CMAKEBUILD)
	+$(CMAKEBUILD) --target install
	touch $@
