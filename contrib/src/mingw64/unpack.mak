# winpthreads, dxvahd, winrt_headers, dcomp

MINGW64_VERSION := 10.0.0
MINGW64_URL := https://sourceforge.net/projects/mingw-w64/files/mingw-w64/mingw-w64-release/mingw-w64-v$(MINGW64_VERSION).tar.bz2/download
MINGW64_HASH=2c35e8ff0d33916bd490e8932cba2049cd1af3d0
MINGW64_GITURL := https://git.code.sf.net/p/mingw-w64/mingw-w64

$(TARBALLS)/mingw-w64-$(MINGW64_HASH).tar.xz:
	$(call download_git,$(MINGW64_GITURL),,$(MINGW64_HASH))

$(TARBALLS)/mingw-w64-v$(MINGW64_VERSION).tar.bz2:
	$(call download_pkg,$(MINGW64_URL),winpthreads)

.sum-mingw64: mingw-w64-v$(MINGW64_VERSION).tar.bz2
# .sum-mingw64: mingw-w64-$(MINGW64_HASH).tar.xz

mingw64: mingw-w64-v$(MINGW64_VERSION).tar.bz2 .sum-mingw64
# mingw64: mingw-w64-$(MINGW64_HASH).tar.xz .sum-mingw64
	$(UNPACK)
	$(MOVE)

.mingw64: mingw64
	touch $@

.sum-winpthreads: .sum-mingw64
	touch $@

.sum-winrt_headers: .sum-mingw64
	touch $@

.sum-dxvahd: .sum-mingw64
	touch $@

.sum-dcomp: .sum-mingw64
	touch $@

.sum-d3d9: .sum-mingw64
	touch $@

.sum-dxva: .sum-mingw64
	touch $@
