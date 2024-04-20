# vncclient

VNCCLIENT_VERSION := 0.9.14
VNCCLIENT_URL := $(GITHUB)/LibVNC/libvncserver/archive/LibVNCServer-$(VNCCLIENT_VERSION).tar.gz

ifdef GPL
ifdef BUILD_NETWORK
PKGS += vncclient
endif
ifeq ($(call need_pkg,"libvncclient"),)
PKGS_FOUND += vncclient
endif
endif

$(TARBALLS)/LibVNCServer-$(VNCCLIENT_VERSION).tar.gz:
	$(call download_pkg,$(VNCCLIENT_URL),vncclient)

.sum-vncclient: LibVNCServer-$(VNCCLIENT_VERSION).tar.gz

vncclient: LibVNCServer-$(VNCCLIENT_VERSION).tar.gz .sum-vncclient
	$(UNPACK)
	mv libvncserver-LibVNCServer-$(VNCCLIENT_VERSION)  LibVNCServer-$(VNCCLIENT_VERSION)
	$(APPLY) $(SRC)/vncclient/vnc-gnutls-anon.patch
	$(APPLY) $(SRC)/vncclient/build-add-install-components-for-client-server.patch
	$(APPLY) $(SRC)/vncclient/build-properly-remove-a-suffix-for-pc-file.patch
	$(call pkg_static,"libvncclient.pc.cmakein")
	$(MOVE)

DEPS_vncclient = gcrypt $(DEPS_gcrypt) jpeg $(DEPS_jpeg) png $(DEPS_png) gnutls $(DEPS_gnutls)

VNCCLIENT_CONF := \
	-DWITH_OPENSSL:BOOL=OFF \
	-DWITH_GCRYPT:BOOL=ON \
	-DWITH_ZLIB:BOOL=ON \
	-DWITH_JPEG:BOOL=ON \
	-DWITH_PNG:BOOL=ON \
	-DWITH_SDL:BOOL=OFF \
	-DWITH_GTK:BOOL=OFF \
	-DWITH_LIBSSH2:BOOL=OFF \
	-DWITH_THREADS:BOOL=ON \
	-DWITH_SYSTEMD:BOOL=OFF \
	-DWITH_FFMPEG:BOOL=OFF \
	-DWITH_WEBSOCKETS:BOOL=OFF \
	-DWITH_EXAMPLES:BOOL=OFF \
	-DWITH_TESTS:BOOL=OFF

ifdef HAVE_WIN32
VNCCLIENT_CONF += -DPREFER_WIN32THREADS:BOOL=ON
endif

.vncclient: vncclient toolchain.cmake
	$(REQUIRE_GPL)
	$(CMAKECLEAN)
	$(HOSTVARS) $(CMAKE) $(VNCCLIENT_CONF)
	+$(CMAKEBUILD) --target vncclient
	$(CMAKEINSTALL) --component vncclient
	touch $@
