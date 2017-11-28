all: checkmakefiles
	cd src && $(MAKE)

clean: checkmakefiles
	cd src && $(MAKE) clean

cleanall: checkmakefiles
	cd src && $(MAKE) MODE=release clean
	cd src && $(MAKE) MODE=debug clean
	rm -f src/Makefile

makefiles:
	cd src && opp_makemake -f --deep

checkmakefiles:
	@if [ ! -f src/Makefile ]; then \
	make makefiles;\
	echo "Execute make all again"; \
	fi
