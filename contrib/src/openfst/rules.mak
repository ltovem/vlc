# openfst

OPENFST_VERSION := 1.8.1
OPENFST_URL := http://www.openfst.org/twiki/pub/FST/FstDownload/openfst-$(OPENFST_VERSION).tar.gz

PKGS += openfst
ifeq ($(call need_pkg,"openfst"),)
PKGS_FOUND += openfst
endif

$(TARBALLS)/openfst-$(OPENFST_VERSION).tar.gz:
	$(call download_pkg,$(OPENFST_URL),openfst)

.sum-openfst: openfst-$(OPENFST_VERSION).tar.gz

openfst: openfst-$(OPENFST_VERSION).tar.gz .sum-openfst
	$(UNPACK)
	$(MOVE)

OPENFSTCONF := $(HOSTCONF) \
	--prefix="$(PREFIX)" \
	--enable-static \
	--enable-shared \
	--enable-far \
	--enable-ngram-fsts \
	--enable-lookahead-fsts \
	--with-pic \
	--disable-bin \
#	--host=${CROSS_TRIPLE} \
	--build=x86-linux-gnu
#De-comment if it doesn't work

OPENFST_CFLAGS := $(CFLAGS) 

DEPS_openfst = ogg $(DEPS_ogg)

.openfst: openfst 
	cd $< && $(AUTORECONF) -i
	cd $< && $(HOSTVARS) CFLAGS="$(OPENFST_CFLAGS)" ./configure $(OPENFSTCONF)
	cd $< && $(MAKE)
	cd $< && $(MAKE) install
	touch $@

#Store in "/contrib/x86_64-linux-gnu/include/fst" directory
