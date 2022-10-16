# Lua 5.4

LUA_SHORTVERSION := 5.4
LUA_VERSION := $(LUA_SHORTVERSION).4
LUA_URL := http://www.lua.org/ftp/lua-$(LUA_VERSION).tar.gz

$(TARBALLS)/lua-$(LUA_VERSION).tar.gz:
	$(call download_pkg,$(LUA_URL),lua)

.sum-lua: lua-$(LUA_VERSION).tar.gz

lua: lua-$(LUA_VERSION).tar.gz .sum-lua
	$(UNPACK)
	$(APPLY) $(SRC)/lua/Disable-dynamic-library-loading-support.patch
	$(APPLY) $(SRC)/lua/Avoid-usage-of-localeconv.patch
	$(APPLY) $(SRC)/lua/Create-an-import-library-needed-for-lld.patch
	$(APPLY) $(SRC)/lua/Disable-system-and-popen-for-windows-store-builds.patch
	$(APPLY) $(SRC)/lua/Add-version-to-library-name.patch
	$(APPLY) $(SRC)/lua/Add-a-Makefile-variable-to-override-the-strip-tool.patch
	$(APPLY) $(SRC)/lua/Create-and-install-a-.pc-file.patch
	$(APPLY) $(SRC)/lua/Add-EXE_EXT-to-allow-specifying-binary-extension.patch
	$(APPLY) $(SRC)/lua/Do-not-use-log2f-with-too-old-Android-API-level.patch
	$(APPLY) $(SRC)/lua/Do-not-use-large-file-offsets-with-too-old-Android-A.patch
	$(APPLY) $(SRC)/lua/Enforce-always-using-64bit-integers-floats.patch
	$(MOVE)

# Luac (lua bytecode compiler)
#
# If lua from contribs is used, luac has to be used from contribs
# as well to match the custom patched lua we use in contribs.

.sum-luac: .sum-lua
	touch $@

# DO NOT use the same intermediate directory as the lua target
luac: UNPACK_DIR=luac-$(LUA_VERSION)
luac: lua-$(LUA_VERSION).tar.gz .sum-luac
	$(RM) -Rf $@ $(UNPACK_DIR) && mkdir -p $(UNPACK_DIR)
	tar xvzfo $< -C $(UNPACK_DIR) --strip-components=1
	$(APPLY) $(SRC)/lua/Enforce-always-using-64bit-integers-floats.patch
	$(MOVE)
