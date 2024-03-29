# vpl (

VPL_VERSION := 2023.3.0
VPL_URL := $(GITHUB)/oneapi-src/oneVPL/archive/refs/tags/$(VPL_VERSION).tar.gz

ifeq ($(call need_pkg,"vpl"),)
PKGS_FOUND += vpl
endif
ifdef HAVE_WIN32
ifeq ($(filter arm aarch64, $(ARCH)),)
PKGS += vpl
endif
endif

DEPS_vpl :=
ifdef HAVE_WINSTORE
DEPS_vpl += alloweduwp $(DEPS_alloweduwp)
endif

ifdef HAVE_WINSTORE
VPL_CONF := CFLAGS="$(CFLAGS) -DMEDIASDK_UWP_DISPATCHER"
VPL_CONF += CXXFLAGS="$(CXXFLAGS) -DMEDIASDK_UWP_DISPATCHER"
endif

$(TARBALLS)/oneVPL-$(VPL_VERSION).tar.gz:
	$(call download_pkg,$(VPL_URL),vpl)

.sum-vpl: oneVPL-$(VPL_VERSION).tar.gz

vpl: oneVPL-$(VPL_VERSION).tar.gz .sum-vpl
	$(UNPACK)
	$(MOVE)

.vpl: vpl
	$(RECONF)
	$(MAKEBUILDDIR)
	$(MAKECONFIGURE) $(VPL_CONF)
	+$(MAKEBUILD)
	+$(MAKEBUILD) install
	touch $@
