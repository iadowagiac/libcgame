.PHONY: all clean dist install uninstall run
.SUFFIXES: .c .o

PREFIX ?= /usr/local

INCDIR ?= /include

include ${DESTDIR}${PREFIX}${INCDIR}/make/cgame-assets.mk

ifeq (${HOSTOS}undefined,undefined)
include ${DESTDIR}${PREFIX}${INCDIR}/make/cgame-default.mk
else
include ${DESTDIR}${PREFIX}${INCDIR}/make/cgame-${HOSTOS}.mk
endif

all: ${EXTRATARGETS}

clean: ${EXTRACLEAN}
	rm -rf *.o ${PACKAGE}-${VERSION}*

dist: clean
	mkdir -p ${PACKAGE}-${VERSION}
	cp -Rf Makefile config.mk ${PACKAGE}-${VERSION}
	tar -cf ${PACKAGE}-${VERSION}.tar ${PACKAGE}-${VERSION}
	gzip ${PACKAGE}-${VERSION}.tar
	rm -rf ${PACKAGE}-${VERSION}

install: all
	mkdir -p ${DESTDIR}${PREFIX}${BINDIR}
	cp -f ${BIN} ${DESTDIR}${PREFIX}${BINDIR}
	chmod 755 ${DESTDIR}${PREFIX}${BINDIR}/${BIN}

uninstall:
	rm -f ${DESTDIR}${PREFIX}${BINDIR}/${BIN}

run: all
	${RUN}

.c.o:
	${CC} ${CFLAGS} -c -o $@ $<

