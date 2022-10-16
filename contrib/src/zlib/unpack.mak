# ZLIB
ZLIB_VERSION := 1.2.12
ZLIB_URL := $(GITHUB)/madler/zlib/archive/refs/tags/v$(ZLIB_VERSION).tar.gz

$(TARBALLS)/zlib-$(ZLIB_VERSION).tar.gz:
	$(call download_pkg,$(ZLIB_URL),zlib)

.sum-zlib: zlib-$(ZLIB_VERSION).tar.gz

zlib: $(SRC)/zlib/*.patch

zlib: zlib-$(ZLIB_VERSION).tar.gz .sum-zlib
	$(UNPACK)
	$(APPLY) $(SRC)/zlib/0001-Fix-mingw-static-library-name-on-mingw-and-Emscripte.patch
	$(APPLY) $(SRC)/zlib/0002-Add-an-option-to-enable-disable-building-examples.patch
	# disable the installation of the dynamic library since there's no option
	sed -e 's,install(TARGETS zlib zlibstatic,install(TARGETS zlibstatic,' -i.orig $(UNPACK_DIR)/CMakeLists.txt
	# only use the proper libz name for the static library
	sed -e 's,set_target_properties(zlib zlibstatic ,set_target_properties(zlibstatic ,' -i.orig $(UNPACK_DIR)/CMakeLists.txt
	$(MOVE)
