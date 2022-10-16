# chromaprint

PKGS += chromaprint
ifeq ($(call need_pkg,"libchromaprint"),)
PKGS_FOUND += chromaprint
endif

chromaprint: $(SRC)/chromaprint/unpack.mak

DEPS_chromaprint = ffmpeg $(DEPS_ffmpeg)

.chromaprint: chromaprint toolchain.cmake
	$(CMAKECLEAN)
	$(HOSTVARS_PIC) $(CMAKE)
	+$(CMAKEBUILD)
	$(CMAKEINSTALL)
	touch $@
