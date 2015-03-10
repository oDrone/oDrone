SUBDIRS = Emission \
		  Reception

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

clean mrproper distclean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir $@; \
	done

.PHONY: clean mrproper distclean subdirs $(SUBDIRS)
