# winpthreads, dxvahd, winrt_headers, dcomp

ifdef HAVE_WIN32
PKGS += winpthreads

ifndef HAVE_VISUALSTUDIO
ifdef HAVE_WINSTORE
PKGS += winrt_headers
else  # !HAVE_WINSTORE
PKGS += d3d9 dcomp
endif # !HAVE_WINSTORE
PKGS += dxva dxvahd

ifeq ($(call mingw_at_least, 8), true)
PKGS_FOUND += d3d9 dxvahd
endif # MINGW 8
ifeq ($(call mingw_at_least, 9), true)
ifdef HAVE_WINSTORE
PKGS_FOUND += winrt_headers
endif # HAVE_WINSTORE
endif # MINGW 9
ifeq ($(call mingw_at_least, 10), true)
PKGS_FOUND += dxva
endif # MINGW 10
ifeq ($(call mingw_at_least, 10), true)
PKGS_FOUND += dcomp
endif
endif # !HAVE_VISUALSTUDIO

HAVE_WINPTHREAD := $(shell $(CC) $(CFLAGS) -E -dM -include pthread.h - < /dev/null >/dev/null 2>&1 || echo FAIL)
ifeq ($(HAVE_WINPTHREAD),)
PKGS_FOUND += winpthreads
endif

endif # HAVE_WIN32

PKGS_ALL += winpthreads winrt_headers d3d9 dxva dxvahd dcomp

mingw64: $(SRC)/mingw64/unpack.mak

.winpthreads: mingw64
	$(MAKEBUILDDIR)
	$(MAKECONFDIR)/mingw-w64-libraries/winpthreads/configure $(HOSTCONF)
	+$(MAKEBUILD)
	+$(MAKEBUILD) install
	touch $@

MINGW_HEADERS_WINRT := \
    windows.foundation.h \
    windows.storage.h \
    windows.storage.streams.h \
    windows.system.threading.h \
    windows.foundation.collections.h \
    eventtoken.h \
    asyncinfo.h \
    windowscontracts.h

.winrt_headers: mingw64
	install -d "$(PREFIX)/include"
	install $(addprefix $</mingw-w64-headers/include/,$(MINGW_HEADERS_WINRT)) "$(PREFIX)/include"
	touch $@

.dxvahd: mingw64
	install -d "$(PREFIX)/include"
	install $</mingw-w64-headers/include/dxvahd.h "$(PREFIX)/include"
	touch $@

.dcomp: mingw64
	install -d "$(PREFIX)/include"
	install $</mingw-w64-headers/include/dcomp.h "$(PREFIX)/include"
	touch $@

MINGW_HEADERS_D3D9 := d3d9.h d3d9caps.h

.d3d9: mingw64
	install -d "$(PREFIX)/include"
	install $(addprefix $</mingw-w64-headers/include/,$(MINGW_HEADERS_D3D9)) "$(PREFIX)/include"
	touch $@

.dxva: mingw64
	install -d "$(PREFIX)/include"
	install $</mingw-w64-headers/include/dxva.h "$(PREFIX)/include"
	touch $@

