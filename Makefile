.PHONY: all clean dist install uninstall run \
        include/ include/make/cgame-default.mk
.SUFFIXES: .c .o

include config.mk
include config/assets.mk
include config/${HOSTOS}.mk

# Include Files.
INCS = ./include/cgame.h ./include/make/cgame*

# Library.
LIB = libcgame.a

# Object Files.
OBJS = ${COMMONOBJS} ${CGAMEOBJS}

COMMONOBJS = ${COMMONSRCS:.c=.o}
COMMONSRCS = entities.c scenes.c cameras.c

all: ${LIB}

clean: ${EXTRACLEAN}
	rm -rf *.o include/ driver/*/*.o \
	       ${LIB} ${PACKAGE}-${VERSION}*

dist: clean
	mkdir -p ${PACKAGE}-${VERSION}
	cp -Rf README Makefile config.mk config/ driver/ \
	       *.c *.h ${PACKAGE}-${VERSION}
	tar -cf ${PACKAGE}-${VERSION}.tar ${PACKAGE}-${VERSION}
	gzip ${PACKAGE}-${VERSION}.tar
	rm -rf ${PACKAGE}-${VERSION}

install: all include/
	mkdir -p ${DESTDIR}${PREFIX}${INCDIR}/make
	cp -Rf cgame.h include/* ${DESTDIR}${PREFIX}${INCDIR}
	mkdir -p ${DESTDIR}${PREFIX}${LIBDIR}
	cp -f ${LIB} ${DESTDIR}${PREFIX}${LIBDIR}
	rm -rf include

uninstall:
	cd ${DESTDIR}${PREFIX} && \
	rm -f ${INCS}
	rm -f ${DESTDIR}${PRefix}${LIBDIR}/${LIB}

run: all ${EXTRATARGETS}
	${RUN}

include/: include/make/cgame-default.mk
	cp -f ./config/common.mk include/make/cgame.mk
	cp -f ./config/assets.mk include/make/cgame-assets.mk
ifeq ("${PREFIX}","/usr/local")
	cp -f ./config/linux.mk include/make/cgame-linux.mk
	cp -f ./config/windows.mk include/make/cgame-windows.mk
	cp -f ./config/psp.mk include/make/cgame-psp.mk
else
ifeq ("${PREFIX}","/usr")
	cp -f ./config/linux.mk include/make/cgame-linux.mk
	cp -f ./config/windows.mk include/make/cgame-windows.mk
	cp -f ./config/psp.mk include/make/cgame-psp.mk
else
	cp -f ./config/${HOSTOS}.mk include/make/cgame-${HOSTOS}.mk
endif
endif

${LIB}: ${OBJS}
	${AR} rcf $@ $^
	${RANLIB} $@

.c.o:
	${CC} ${CFLAGS} -I${shell pwd} -c -o $@ $<
