# aom
AOM_VERSION := 3.5.0
AOM_URL := https://storage.googleapis.com/aom-releases/libaom-$(AOM_VERSION).tar.gz

$(TARBALLS)/libaom-$(AOM_VERSION).tar.gz:
	$(call download_pkg,$(AOM_URL),aom)

.sum-aom: libaom-$(AOM_VERSION).tar.gz

aom: $(SRC)/aom/*.patch

aom: libaom-$(AOM_VERSION).tar.gz .sum-aom
	$(UNPACK)
ifdef HAVE_ANDROID
	$(APPLY) $(SRC)/aom/aom-android-pthreads.patch
	$(APPLY) $(SRC)/aom/aom-android-cpufeatures.patch
endif
	$(MOVE)
ifdef HAVE_ANDROID
	cp $(ANDROID_NDK)/sources/android/cpufeatures/cpu-features.c $(ANDROID_NDK)/sources/android/cpufeatures/cpu-features.h aom/aom_ports/
endif
