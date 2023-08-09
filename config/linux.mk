LDADD = -lcgame -lc -lm -lasound -lglut -lGL -lGLU -lX11

# Binary File.
BIN ?= game

# Object Files for libcgame.
CGAMEOBJS = ${CGAMESRCS:.c=.o}

# Source Files for libcgame.
CGAMESRCS = driver/core/posix.c \
            driver/console/posix.c \
            driver/audio/alsa.c \
            driver/graphics/glut.c \
            driver/graphics/gl.c

RUN = ./${BIN}

EXTRATARGETS += ${BIN}
EXTRACLEAN   += clean-linux

.PHONY: clean-linux

default: all

clean-linux:
	rm -f ${EXTRATARGETS}

include/make/cgame-default.mk:
	mkdir -p ${shell dirname $@}
	ln -f -s cgame-linux.mk $@

${BIN}: ${ASSETOBJS}
	${CC} -L${shell pwd} ${LDFLAGS} -o $@ $^ ${LDADD}

