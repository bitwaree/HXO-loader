CC=gcc
LIBNAME=hxo_loader.so
FLAGS=
INSDIR=/usr/lib/hxo/
VERSION := $(shell cat ./version)

build:
	$(CC) -c *.c -fPIC -DVER_STR=\"$(VERSION)\"
	$(CC) -s -o ./bin/$(LIBNAME) *.o -shared

install:
	mkdir -p $(INSDIR)
	cp ./bin/$(LIBNAME) $(INSDIR)

uninstall:
	rm -R $(INSDIR)
clean:
	rm *.o bin/*
