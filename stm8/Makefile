SUBDIRS := blinky spl-template

.PHONY: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

all clean: $(SUBDIRS)
