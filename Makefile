CC=gcc
LIBNAME=hxo_loader.so
FLAGS=
INSDIR=/usr/lib/hxo/

build:
	$(CC) -c *.c -fPIC
	$(CC) -s -o ./bin/$(LIBNAME) *.o -shared

install:
	mkdir -p $(INSDIR)
	cp ./bin/$(LIBNAME) $(INSDIR)

uninstall:
	rm -R $(INSDIR)
clean:
	rm *.o bin/*
