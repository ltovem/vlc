# ebml

EBML_VERSION := 1.4.3
EBML_URL := http://dl.matroska.org/downloads/libebml/libebml-$(EBML_VERSION).tar.xz

$(TARBALLS)/libebml-$(EBML_VERSION).tar.xz:
	$(call download_pkg,$(EBML_URL),ebml)

.sum-ebml: libebml-$(EBML_VERSION).tar.xz

ebml: libebml-$(EBML_VERSION).tar.xz .sum-ebml
	$(UNPACK)
	$(MOVE)
