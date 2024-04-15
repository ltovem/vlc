# qtshadertools
# required for Qt5Compat, and for qtdeclarative.

QTSHADERTOOLS_VERSION_MAJOR := 6.6
QTSHADERTOOLS_VERSION := $(QTSHADERTOOLS_VERSION_MAJOR).2
QTSHADERTOOLS_URL := $(QT)/$(QTSHADERTOOLS_VERSION_MAJOR)/$(QTSHADERTOOLS_VERSION)/submodules/qtshadertools-everywhere-src-$(QTSHADERTOOLS_VERSION).tar.xz

DEPS_qtshadertools-tools := qt-tools $(DEPS_qt-tools)

ifdef HAVE_WIN32
DEPS_qtshadertools-tools += fxc2 $(DEPS_fxc2)
endif

ifneq ($(findstring qt,$(PKGS)),)
PKGS_TOOLS += qtshadertools-tools
endif
PKGS_ALL += qtshadertools-tools

ifeq ($(shell qsb --version 2>/dev/null | head -1 | sed s/'.* '// | cut -d '.' -f -2),$(QTSHADERTOOLS_VERSION_MAJOR))
PKGS_FOUND += qtshadertools-tools
endif

$(TARBALLS)/qtshadertools-everywhere-src-$(QTSHADERTOOLS_VERSION).tar.xz:
	$(call download,$(QTSHADERTOOLS_URL))

.sum-qtshadertools-tools: qtshadertools-everywhere-src-$(QTSHADERTOOLS_VERSION).tar.xz

qtshadertools: qtshadertools-everywhere-src-$(QTSHADERTOOLS_VERSION).tar.xz .sum-qtshadertools-tools
	$(UNPACK)
	$(APPLY) $(SRC)/qtshadertools/0001-Use-fxc2-through-wine-instead-of-fxc.patch
	$(APPLY) $(SRC)/qtshadertools/0001-Backport-a-change-to-SPIRV-Cross-to-add-a-missing-in.patch
	$(MOVE)

.qtshadertools-tools: BUILD_DIR=$</vlc_native
.qtshadertools-tools: qtshadertools
	$(CMAKECLEAN)
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && $(BUILDVARS) $(BUILDPREFIX)/bin/qt-configure-module $(BUILD_SRC)
	+$(CMAKEBUILD)
	$(CMAKEINSTALL)
	touch $@
