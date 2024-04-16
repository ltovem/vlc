# ninja

NINJA_VERSION := 1.11.1
NINJA_BUILD_NAME := $(NINJA_VERSION).g95dee.kitware.jobserver-1
NINJA_URL := $(GITHUB)/Kitware/ninja/archive/refs/tags/v$(NINJA_BUILD_NAME).tar.gz

PKGS += ninja-jobserver
PKGS_TOOLS += ninja-jobserver
ifneq ($(call system_tool_version, ninja --version, grep .jobserver),)
PKGS_FOUND += ninja-jobserver
endif

$(TARBALLS)/ninja-$(NINJA_BUILD_NAME).tar.gz:
	$(call download,$(NINJA_URL))

.sum-ninja-jobserver: ninja-$(NINJA_BUILD_NAME).tar.gz

ninja-jobserver: ninja-$(NINJA_BUILD_NAME).tar.gz .sum-ninja-jobserver
	$(UNPACK)
	$(MOVE)

.ninja-jobserver: BUILD_DIR=$</vlc_native
.ninja-jobserver: ninja-jobserver
	$(MAKEBUILDDIR)
	cd $(BUILD_DIR); $(BUILDVARS) python3 ../configure.py --bootstrap
	install -d "$(BUILDBINDIR)"
	install $(BUILD_DIR)/ninja "$(BUILDBINDIR)"
	touch $@
