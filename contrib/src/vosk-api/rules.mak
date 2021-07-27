# vosk-api

VOSK-API_VERSION := master
VOSK-API_URL := https://github.com/alphacep/vosk-api/archive/$(VOSK-API_VERSION).tar.gz

ifeq ($(call need_pkg,"vosk-api"),)
PKGS_FOUND += vosk-api
endif

$(TARBALLS)/vosk-api-$(VOSK-API_VERSION).tar.gz:
	$(call download_pkg,$(VOSK-API_URL),vosk-api)

.sum-vosk-api: vosk-api-$(VOSK-API_VERSION).tar.gz

vosk-api: vosk-api-$(VOSK-API_VERSION).tar.gz .sum-vosk-api
	$(UNPACK)
	$(MOVE)

.vosk-api: vosk-api toolchain.cmake
	cd $< && cd src && $(HOSTVARS_PIC) $(MAKE)
	touch $@
