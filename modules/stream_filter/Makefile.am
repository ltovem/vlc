stream_filterdir = $(pluginsdir)/stream_filter

stream_filter_LTLIBRARIES =

libcache_read_plugin_la_SOURCES = stream_filter/cache_read.c
stream_filter_LTLIBRARIES += libcache_read_plugin.la

libcache_block_plugin_la_SOURCES = stream_filter/cache_block.c
stream_filter_LTLIBRARIES += libcache_block_plugin.la

libdecomp_plugin_la_SOURCES = stream_filter/decomp.c
if !HAVE_WIN32
if !HAVE_TVOS
stream_filter_LTLIBRARIES += libdecomp_plugin.la
endif
endif

if BUILD_RUST
libinflate_rust_plugin_la = \
	stream_filter/inflate_rust/src/capi.rs \
	stream_filter/inflate_rust/src/inflate.rs \
	stream_filter/inflate_rust/src/lib.rs \
	stream_filter/inflate_rust/inflate_rust.c

@abs_top_builddir@/modules/stream_filter/inflate_rust/inflate_rust.h: $(libinflate_rust_plugin_la)
	mkdir -p stream_filter/inflate_rust && cd stream_filter/inflate_rust && \
	$(CARGO_C) --library-type=staticlib --destdir=. --includedir=.. --libdir=. \
	--manifest-path=$(abs_src_dir)/stream_filter/inflate_rust/Cargo.toml

BUILT_SOURCES += @abs_top_builddir@/modules/stream_filter/inflate_rust/inflate_rust.h
libinflate_rust_plugin_la_SOURCES = stream_filter/inflate_rust/inflate_rust.c \
									@abs_top_builddir@/modules/stream_filter/inflate_rust/inflate_rust.h
libinflate_rust_plugin_la_CFLAGS = $(AM_CFLAGS) $(LIBDL) -I@abs_top_builddir@/modules/stream_filter
libinflate_rust_plugin_la_LIBADD = $(RUST_RUNTIME_LIBS) \
								   @abs_top_builddir@/modules/stream_filter/inflate_rust/libinflate_rust.a
stream_filter_LTLIBRARIES += libinflate_rust_plugin.la
else
libinflate_plugin_la_SOURCES = stream_filter/inflate.c
libinflate_plugin_la_LIBADD = -lz
if HAVE_ZLIB
stream_filter_LTLIBRARIES += libinflate_plugin.la
endif
endif

libprefetch_plugin_la_SOURCES = stream_filter/prefetch.c
if !HAVE_WINSTORE
stream_filter_LTLIBRARIES += libprefetch_plugin.la
endif

libhds_plugin_la_SOURCES = \
    stream_filter/hds/hds.c

libhds_plugin_la_CFLAGS = $(AM_CFLAGS)
stream_filter_LTLIBRARIES += libhds_plugin.la

librecord_plugin_la_SOURCES = stream_filter/record.c
stream_filter_LTLIBRARIES += librecord_plugin.la

libaribcam_plugin_la_SOURCES = stream_filter/aribcam.c
libaribcam_plugin_la_CFLAGS = $(AM_CFLAGS) $(ARIBB25_CFLAGS)
libaribcam_plugin_la_LDFLAGS = $(AM_LDFLAGS) $(ARIBB25_LDFLAGS) -rpath '$(stream_filterdir)'
libaribcam_plugin_la_LIBADD = $(ARIBB25_LIBS)
stream_filter_LTLIBRARIES += $(LTLIBaribcam)
EXTRA_LTLIBRARIES += libaribcam_plugin.la

libaccesstweaks_plugin_la_SOURCES = stream_filter/accesstweaks.c
libaccesstweaks_plugin_la_LDFLAGS = $(AM_LDFLAGS) -rpath '$(stream_filterdir)'
check_LTLIBRARIES += libaccesstweaks_plugin.la

libadf_plugin_la_SOURCES = stream_filter/adf.c
stream_filter_LTLIBRARIES += libadf_plugin.la

libskiptags_plugin_la_SOURCES = stream_filter/skiptags.c
stream_filter_LTLIBRARIES += libskiptags_plugin.la
