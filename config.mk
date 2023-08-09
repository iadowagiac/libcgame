# Package Information.
PACKAGE = libcgame
VERSION = 1.0

# Installation Path Prefixes.
PREFIX = /usr/local

# Installation Paths.
INCDIR = /include
LIBDIR = /lib

# Host Selection.
HOSTOS = linux

# Utilities.
CC     = cc
AR     = ar
RANLIB = ranlib

# Compilation Flags.
CFLAGS  = ${IPATHS} -O2
LDFLAGS = ${LPATHS} -s

