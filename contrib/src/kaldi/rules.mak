# kaldi

KALDI_VERSION := lookahead-1.8.0
KALDI_URL := https://github.com/alphacep/kaldi/archive/$(KALDI_VERSION).tar.gz

ifeq ($(call need_pkg,"kaldi"),)
PKGS_FOUND += kaldi
endif

$(TARBALLS)/kaldi-$(KALDI_VERSION).tar.gz:
	$(call download_pkg,$(KALDI_URL),kaldi)

.sum-kaldi: kaldi-$(KALDI_VERSION).tar.gz

kaldi: kaldi-$(KALDI_VERSION).tar.gz .sum-kaldi
	$(UNPACK)
	$(MOVE)

KALDICONF := --mathlib=OPENBLAS_CLAPACK\
	--shared \
	--use-cuda=no \
#	--mathlib=OPENBLAS_CLAPACK

KALDICONF += --fst-root="$(PREFIX)"
KALDICONF += --fst-version="1.8.0"
#KALDICONF += --static-math
#KALDICONF += --openblas-root="$(PREFIX)" 
#KALDICONF += --clapack-root="$(PREFIX)" 

KALDI_CFLAGS := $(CFLAGS)


.kaldi: kaldi toolchain.cmake
	cd $< && $(HOSTVARS_PIC) $(CMAKE) -DENABLE_WIN32_IO=OFF -DBLA_STATIC=ON -DBLA_VENDOR=OpenBLAS

#	cd $< && cd tools && extras/install_openblas_clapack.sh
#	cd $< && cd src && $(HOSTVARS) CFLAGS="$(KALDI_CFLAGS)" ./configure $(KALDICONF)
#	cd $< && cd src && $(MAKE) online2 lm rnnlm
#	cd $< && $(HOSTVARS_PIC) $(CMAKE) src/online2 src/lm src/rnnlm -DENABLE_WIN32_IO=OFF
#	cd $< && $(CMAKEBUILD) . --target install
#	touch $@
