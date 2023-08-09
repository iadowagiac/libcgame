# This makefile is designed for use with mingw32-w64

# If the BINPREFIX is undefined, try to get it from the CPU
# or PREFIX variable.
ifeq (${BINPREFIX}undefined,undefined)
ifeq (${CPU}undefined,undefined)
BINPREFIX = ${shell basename ${PREFIX}}-
else
BINPREFIX = ${CPU}-w64-mingw32-
endif
endif

# If the BINPREFIX is something unexpected, default to the
# x86_64-w64-mingw32 toolchain.
ifneq (${BINPREFIX},i686-w64-mingw32-)
ifneq (${BINPREFIX},x86_64-w64-mingw32-)
BINPREFIX = x86_64-w64-mingw32-
endif
endif

# If the CPU is undefined, get it from the BINPREFIX.
ifeq (${CPU}undefined,undefined)
CPU = ${shell echo ${BINPREFIX} | sed 's/-.*//'}
endif

# Utilities.
CC     = ${BINPREFIX}gcc
AR     = ${BINPREFIX}ar
RANLIB = ${BINPREFIX}ranlib

# The LD hack is necessary in order to allow WinMain
# to be loaded from a static library.
LDADD  = -lcgame
LDHACK = -Wl,-u,WinMain,-u,_WinMain@16

LDFLAGS := ${LDHACK} ${LDFLAGS}

# Binary File.
BIN ?= game

# Object Files for libcgame.
CGAMEOBJS = ${CGAMESRCS:.c=.o}

# Source Files for libcgame.
CGAMESRCS = driver/core/windows.c \
            driver/console/windows.c \
            driver/graphics/win32.c \
            driver/graphics/gl.c

# Get the wine prefix if it's undefined.
ifeq (${WINEPREFIX}undefined,undefined)
ifeq ("${CPU}","x86_64")
WINEPREFIX = ~/.wine64
else
WINEPREFIX = ~/.wine
endif
endif

# Get the wine executable if it's undefined.
ifeq (${WINE}undefined,undefined)
ifeq ("${CPU}","x86_64")
WINE = wine64
else
WINE = wine
endif
endif

RUN = WINPREFIX=${WINEPREFIX} ${WINE} ./${BIN}.exe

EXTRATARGETS += ${BIN}.exe
EXTRACLEAN   += clean-windows

.PHONY: clean-windows

default: all

clean-windows:
	rm -f ${EXTRATARGETS}

include/make/cgame-default.mk:
	mkdir -p ${shell dirname $@}
	ln -f -s cgame-windows.mk $@

${BIN}.exe:
	${CC} -L${shell pwd} ${LDFLAGS} -o $@ ${LDADD}

