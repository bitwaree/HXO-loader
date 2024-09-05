#Build Script for HXO-loader-android
#Copyright (C) 2024 bitware
#Run "make help" for help statement
#Checkout README.md for more info
CC=gcc
LIBNAME=libhxo.so
CCFLAGS=
LDFLAGS=
PREFIX=/usr
VERSION := $(shell cat ../version)

build:
	@ $(CC) -c ../src/*.c -fPIC -DVER_STR=\"$(VERSION)-android\" -D__ANDROID__ $(CCFLAGS)
	@ echo "CC *.c ->>> *.o"
	@ $(CC) -s -o ./$(LIBNAME) *.o -shared $(LDFLAGS)
	@ echo "LD *.o ->>> ./$(LIBNAME)"
	@ echo "\nBuild Successful ....\nRun \"sudo make install\" to Install HXO-loader-v$(VERSION)-android"

clean:
	@ rm -v *.o *.so

