# ZLIB

PKGS += zlib
ifeq ($(call need_pkg,"zlib"),)
PKGS_FOUND += zlib
endif

include $(SRC)/zlib/unpack.mak

zlib: $(SRC)/zlib/unpack.mak

ZLIB_CONF = -DINSTALL_PKGCONFIG_DIR:STRING=$(PREFIX)/lib/pkgconfig -DBUILD_EXAMPLES=OFF

# ASM is disabled as the necessary source files are not in the tarball nor the git
# ifeq ($(ARCH),i386)
# ZLIB_CONF += -DASM686=ON
# endif
# ifeq ($(ARCH),x86_64)
# ZLIB_CONF += -DAMD64=ON
# endif

.zlib: zlib toolchain.cmake
	$(CMAKECLEAN)
	$(HOSTVARS) $(CMAKE) $(ZLIB_CONF)
	+$(CMAKEBUILD)
	+$(CMAKEBUILD) --target install
	touch $@
