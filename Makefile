#Build Script for HXO-loader
#Copyright (C) 2024 bitware
#Run "make help" for help statement
#Checkout README.md for more info
CC=gcc
LIBNAME=hxo_loader.so
CCFLAGS=
LDFLAGS=
PREFIX=/usr
VERSION := $(shell cat ./version)

build:
	@ $(CC) -c *.c -fPIC -DVER_STR=\"$(VERSION)\" $(FLAGS)
	@ echo "CC *.c ->>> *.o"
	@ $(CC) -s -o ./lib/$(LIBNAME) *.o -shared $(LDFLAGS)
	@ echo "LD *.o ->>> ./lib/$(LIBNAME)"
	@ echo "\nBuild Successful ....\nRun \"sudo make install\" to Install HXO-loader-v$(VERSION)"


install:
	@ #check if it's already build or not
	@if [ ! -e ./lib/$(LIBNAME) ]; then \
		echo "\nERROR: HXO-loader isn't compiled and built at first place!!"; \
		echo "Run \"make\" then re-run \"make install\" for a successful installation.\n"; \
		exit 1; \
	fi
	@ #proceed to the installation process ..
	cp ./lib/$(LIBNAME) $(PREFIX)/lib/
	cp ./scripts.sh/hxo-patch $(PREFIX)/bin/hxo-patch
	cp ./scripts.sh/hxo-rmpatch $(PREFIX)/bin/hxo-rmpatch
	cp ./scripts.sh/hxo-run $(PREFIX)/bin/hxo-run
	chmod +x $(PREFIX)/bin/hxo-patch $(PREFIX)/bin/hxo-rmpatch $(PREFIX)/bin/hxo-run
	@ echo "\nHXO-loader-v$(VERSION): Successfully installed ..."

uninstall:
	@ #check if it's already build or not
	@if [ ! -e $(PREFIX)/lib/$(LIBNAME) ]; then \
		echo "\nERROR: HXO-loader isn't installed at first place!!\n"; \
		exit 1; \
	fi
	rm $(PREFIX)/lib/$(LIBNAME)
	rm $(PREFIX)/bin/hxo-patch $(PREFIX)/bin/hxo-rmpatch $(PREFIX)/bin/hxo-run
	@ echo "\nSuccessfully uninstalled ..."
clean:
	@ rm -v *.o lib/*

