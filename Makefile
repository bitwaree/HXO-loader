#Build Script for HXO-loader-linux
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
	@ $(CC) -c src/*.c -fPIC -DVER_STR=\"$(VERSION)-linux\" $(CCFLAGS)
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
	@ #copy include file
	cp ./include/hxo-user.h $(PREFIX)/include/hxo-user.h
	@ #copy man pages
	mkdir -p $(PREFIX)/local/man/man1/ $(PREFIX)/local/man/man3/
	cp ./man/hxo*.1 $(PREFIX)/local/man/man1/
	cp ./man/hxo*.3 $(PREFIX)/local/man/man3/
	@ echo "\nHXO-loader-v$(VERSION): Successfully installed ..."

uninstall:
	@ #check if it's already installed or not
	@if [ ! -e $(PREFIX)/lib/$(LIBNAME) ]; then \
		echo "\nERROR: HXO-loader isn't installed at first place!!\n"; \
		exit 1; \
	fi
	rm $(PREFIX)/lib/$(LIBNAME)
	rm $(PREFIX)/bin/hxo-patch $(PREFIX)/bin/hxo-rmpatch $(PREFIX)/bin/hxo-run
	rm $(PREFIX)/local/man/man1/hxo* $(PREFIX)/local/man/man3/hxo*
	rm $(PREFIX)/include/hxo-user.h

	@ echo "\nSuccessfully uninstalled ..."


clean:
	@ rm -v *.o lib/*
