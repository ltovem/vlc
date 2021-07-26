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


#Delete --prefix
TEMP_PREFIX := --prefix="$(PREFIX)"
HOSTCONF_NOPREFIX := $(filter-out $(TEMP_PREFIX) , $(HOSTCONF) )

KALDICONF := --mathlib=OPENBLAS_CLAPACK \
	--shared \
	--use-cuda=no \
	--fst-root="$(PREFIX)" \
	--openblas-root="$(PREFIX)" \
	--clapack-root="$(PREFIX)" \
#	--fst-version=STR \
#	$(HOSTCONF_NOPREFIX) 
	


KALDI_CFLAGS := $(CFLAGS)


.kaldi: kaldi toolchain.cmake
#	cd $< && $(HOSTVARS_PIC) $(CMAKE) -DENABLE_WIN32_IO=OFF  
#	cd $< && $(CMAKEBUILD) . --target install
#	@echo "Prefix is $(TEMP_PREFIX)"
#	@echo "Path is $(HOSTCONF_NOPREFIX)"
#	@echo "Second Path is $(HOSTCONF)"
#	@echo "FIND $(filter-out $(TEMP_PREFIX), $(HOSTCONF) )"
	cd $< && cd src && ./configure $(KALDICONF)
	cd $< && cd src && $(MAKE) online2 lm rnnlm
	touch $@
