C=gcc
CFLAGS=-I.
OUTPUT=build
TARGET=umod
SRC=src

$(OUTPUT)/$(TARGET): $(SRC)/umodule.c
	     $(CC) -o $@ $< -I/usr/include -I/usr/src/$(uname -r)/include/linux
