# Lua 5.4

# Reverse priority order
LUA_TARGET := generic
ifdef HAVE_BSD
LUA_TARGET := bsd
endif
ifdef HAVE_LINUX
LUA_TARGET := linux
endif
ifdef HAVE_MACOSX
LUA_TARGET := macosx
endif
ifdef HAVE_IOS
LUA_TARGET := ios
endif
ifdef HAVE_WIN32
LUA_TARGET := mingw
endif
ifdef HAVE_SOLARIS
LUA_TARGET := solaris
endif

# Feel free to add autodetection if you need to...
PKGS += lua luac
PKGS_TOOLS += luac
PKGS_ALL += luac

LUAC_IF_NOT_CROSS =
ifndef HAVE_CROSS_COMPILE
LUAC_IF_NOT_CROSS += luac
endif

ifeq ($(call need_pkg,"lua >= 5.1"),)
PKGS_FOUND += lua $(LUAC_IF_NOT_CROSS)
else
ifeq ($(call need_pkg,"lua5.2"),)
PKGS_FOUND += lua $(LUAC_IF_NOT_CROSS)
else
ifeq ($(call need_pkg,"lua5.1"),)
PKGS_FOUND += lua $(LUAC_IF_NOT_CROSS)
endif
endif
endif

ifeq ($(shell $(HOST)-luac -v 2>/dev/null | head -1 | sed  -E 's/Lua ([0-9]+).([0-9]+).*/\1.\2/'),$(LUA_SHORTVERSION))
PKGS_FOUND += luac
endif
ifeq ($(shell $(HOST)-luac -v 2>/dev/null | head -1 | sed  -E 's/Lua ([0-9]+).([0-9]+).*/\1.\2/'),5.2)
PKGS_FOUND += luac
endif

include $(SRC)/lua/unpack.mak

lua: $(SRC)/lua/unpack.mak
luac: $(SRC)/lua/unpack.mak

LUA_MAKEFLAGS := \
	$(HOSTTOOLS) \
	AR="$(AR) rcu" \
	MYCFLAGS="$(CFLAGS)" \
	MYLDFLAGS="$(LDFLAGS)" \
	CPPFLAGS="$(CPPFLAGS)"

# Make sure we do not use the cross-compiler when building
# the native luac for the host.
LUA_BUILD_MAKEFLAGS := \
	$(BUILDTOOLS) \
	AR="$(BUILDAR) rcu" \
	MYCFLAGS="$(BUILDCFLAGS)" \
	MYLDFLAGS="$(BUILDLDFLAGS)" \
	CPPFLAGS="$(BUILDCPPFLAGS)"

ifdef HAVE_WIN32
	LUA_MAKEFLAGS += EXE_EXT=.exe
endif

.lua: lua
	$(HOSTVARS_PIC) $(MAKE) -C $< $(LUA_TARGET) $(LUA_MAKEFLAGS)
ifdef HAVE_WIN32
	$(HOSTVARS) $(MAKE) -C $< -C src liblua$(LUA_SHORTVERSION).a $(LUA_MAKEFLAGS)
endif

	$(HOSTVARS) $(MAKE) -C $< install \
		INSTALL_INC="$(PREFIX)/include/lua$(LUA_SHORTVERSION)" \
		INSTALL_TOP="$(PREFIX)" \
		$(LUA_MAKEFLAGS)
ifdef HAVE_WIN32
	$(RANLIB) "$(PREFIX)/lib/liblua$(LUA_SHORTVERSION).a"
endif

	# Configure scripts might search for lua >= 5.4 or lua5.4 so expose both
	cp "$(PREFIX)/lib/pkgconfig/lua.pc" "$(PREFIX)/lib/pkgconfig/lua$(LUA_SHORTVERSION).pc"
	touch $@

# Luac (lua bytecode compiler)
#
# If lua from contribs is used, luac has to be used from contribs
# as well to match the custom patched lua we use in contribs.

.luac: luac
	$(MAKE) -C $< $(LUA_BUILD_MAKEFLAGS) generic
	mkdir -p -- $(BUILDBINDIR)
	install -m 0755 -s -- $</src/luac $(BUILDBINDIR)/$(HOST)-luac
	touch $@
