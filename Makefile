TOPTARGETS := all clean install uninstall
SUBDIRS := libmdblocales localenormalizer

$(TOPTARGETS): $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: $(TOPTARGETS) $(SUBDIRS)
