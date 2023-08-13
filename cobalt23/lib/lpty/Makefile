# simple Makefile for lpty. Works for Linux, MacOS X, probably other unixen
#
# Gunnar Zötl <gz@tset.de>, 2011.
# Released under MIT/X11 license. See file LICENSE for details.

# try some automatic discovery
OS = $(shell uname -s)
LUAVERSION = cobalt
LUADIR = $(shell dirname `which cobalt`)
LUAROOT = $(shell dirname $(LUADIR))

# Defaults
CC = gcc
TARGET = lpty.so
DEBUG= #-g -lefence
CFLAGS=-O2 -fPIC $(DEBUG)
INCDIRS=-I/usr/local/include/cobalt/
LIBDIRS=-L$(LUAROOT)/lib
LDFLAGS=-shared $(DEBUG)

INSTALL_ROOT=/usr/local
SO_INST_ROOT=$(INSTALL_ROOT)/lib/lua/$(LUAVERSION)
LUA_INST_ROOT=$(INSTALL_ROOT)/share/lua/$(LUAVERSION)

# OS specialities
ifeq ($(OS),Darwin)
LDFLAGS = -bundle -undefined dynamic_lookup -all_load
endif

all: $(TARGET)

$(TARGET): lpty.o
	$(CC) $(LDFLAGS) -o $@ $(LIBDIRS) $<

lpty.o: lpty.c
	$(CC) $(CFLAGS) $(INCDIRS) -c $< -o $@

install: all
	cp $(TARGET) $(SO_INST_ROOT)

test: all
	cd samples && LUA_CPATH=../\?.so lua lptytest.lua

clean:
	find . -name "*~" -exec rm {} \;
	find . -name .DS_Store -exec rm {} \;
	find . -name "._*" -exec rm {} \;
	rm -f *.o *.so core