# kaldi

KALDI_VERSION := lookahead-1.8.0
KALDI_URL := https://github.com/alphacep/kaldi/archive/$(KALDI_VERSION).tar.gz

ifeq ($(call need_pkg,"kaldi"),)
PKGS_FOUND += kaldi
endif

$(TARBALLS)/libkaldi-$(KALDI_VERSION).tar.gz:
	$(call download_pkg,$(KALDI_URL),kaldi)

.sum-kaldi: libkaldi-$(KALDI_VERSION).tar.gz

kaldi: kaldi-$(KALDI_VERSION).tar.gz .sum-kaldi
	$(UNPACK)
	$(MOVE)

.kaldi: kaldi toolchain.cmake
	cd $< && $(HOSTVARS_PIC) $(CMAKE) -DENABLE_WIN32_IO=OFF
	cd $< && $(CMAKEBUILD) . --target install
	touch $@
