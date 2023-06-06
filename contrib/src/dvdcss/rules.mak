# DVDCSS
DVDCSS_VERSION := 1.4.3
DVDCSS_URL := $(VIDEOLAN)/libdvdcss/$(DVDCSS_VERSION)/libdvdcss-$(DVDCSS_VERSION).tar.bz2

ifeq ($(call need_pkg,"libdvdcss"),)
PKGS_FOUND += dvdcss
endif

$(TARBALLS)/libdvdcss-$(DVDCSS_VERSION).tar.bz2:
	$(call download,$(DVDCSS_URL))

.sum-dvdcss: libdvdcss-$(DVDCSS_VERSION).tar.bz2


dvdcss: libdvdcss-$(DVDCSS_VERSION).tar.bz2 .sum-dvdcss
	$(UNPACK)
	$(APPLY) $(SRC)/dvdcss/0001-Use-wide-chars-explicitly-to-open-the-drive.patch
	$(APPLY) $(SRC)/dvdcss/0002-Read-the-drive-handle-as-a-HANDLE.patch
	$(APPLY) $(SRC)/dvdcss/0003-Use-CreateFile2-when-building-for-Win8.patch
	$(APPLY) $(SRC)/dvdcss/0004-Disable-cache-for-in-Universal-Windows-Platform-buil.patch
	$(MOVE)

DVDCSS_CONF := --disable-doc

.dvdcss: dvdcss
	$(RECONF)
	$(MAKEBUILDDIR)
	$(MAKECONFIGURE) $(DVDCSS_CONF)
	+$(MAKEBUILD)
	+$(MAKEBUILD) install
	touch $@
