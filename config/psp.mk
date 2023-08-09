# This makefile is designed for use with pspdev
# https://github.com/pspdev

PSPSDK := ${shell psp-config --pspsdk-path}
PSPDIR := ${shell psp-config --psp-prefix}

# Utilities.
CC       = psp-gcc
AR       = psp-ar
RANLIB   = psp-ranlib
STRIP    = psp-strip
MKSFO    = mksfoex
PACK_PBP = pack-pbp
FIXUP    = psp-fixup-imports
ENC		 = PrxEncrypter

# Compilation Flags.
CFLAGS  += -G0 -D_PSP_FW_VERSION=${PSP_FW_VERSION}
LDFLAGS += -Wl,-zmax-page-size=128

# Include Paths.
IPATHS += -I${PSPDEV}/psp/include -I${PSPSDK}/include

# Library Paths.
LPATHS += -L${PSPDEV}/psp/lib -L${PSPSDK}/lib

LDADD += -lcgame -lpspdebug -lpspdisplay -lpspge -lpspctrl \
         -lpspnet -lpspnet_apctl

# Binary File.
BIN ?= EBOOT

# Object Files for libcgame.
CGAMEOBJS = ${CGAMESRCS:.c=.o}

# Source Files for libcgame.
CGAMESRCS = driver/core/psp.c \
            driver/console/psp.c

# PPSSPP is used to execute the EBOOT.PBP file.
# On my machine, it's installed as a flatpak.
PPSSPP = flatpak run --command=PPSSPPSDL org.ppsspp.PPSSPP

RUN  = ${PPSSPP} ./${PSP_EBOOT}

EXTRATARGETS = ${PSP_EBOOT}
EXTRACLEAN   = clean-psp

GAME_TITLE ?= "CGame"

PSP_FW_VERSION ?= 150

PSP_EBOOT        ?= ${BIN}.PBP
PSP_EBOOT_TITLE  ?= ${GAME_TITLE}
PSP_EBOOT_SFO    ?= PARAM.SFO
PSP_EBOOT_ICON   ?= NULL    # png file
PSP_EBOOT_ICON1  ?= NULL
PSP_EBOOT_UNKPNG ?= NULL
PSP_EBOOT_PIC1   ?= NULL    # png file
PSP_EBOOT_SND0   ?= NULL    # at3 file
PSP_EBOOT_PSAR   ?= NULL

EXPAND_MEMORY ?= 0

SFOFLAGS = -d MEMSIZE=${EXPAND_MEMORY}

.PHONY: clean-psp

default: all

clean-psp:
	rm -f moduleinfo.c PARAM.SFO ${BIN}.ELF ${BIN}_strip.ELF \
	      ${PSP_EBOOT}

include/make/cgame-default.mk:
	mkdir -p ${shell dirname $@}
	ln -f -s cgame-psp.mk $@

moduleinfo.c:
	echo "#include <pspmoduleinfo.h>" >> $@
	echo "PSP_MODULE_INFO(\"${GAME_TITLE}\", 0, 1, 1);" >> $@

${BIN}.ELF: moduleinfo.o
	${CC} -L${shell pwd} ${LDFLAGS} -o $@ $< ${LDADD}
	${FIXUP} $@
	-rm -f moduleinfo.c moduleinfo.o

${PSP_EBOOT}: ./${BIN}.ELF ${PSP_EBOOT_SFO}
	${STRIP} ${BIN}.ELF -o ${BIN}_strip.ELF
	${PACK_PBP} ${PSP_EBOOT} ${PSP_EBOOT_SFO} ${PSP_EBOOT_ICON}  \
		${PSP_EBOOT_ICON1} ${PSP_EBOOT_UNKPNG} ${PSP_EBOOT_PIC1}  \
		${PSP_EBOOT_SND0}  ${BIN}_strip.ELF ${PSP_EBOOT_PSAR}
	-rm -f PARAM.SFO ${BIN}.ELF ${BIN}_strip.ELF

${PSP_EBOOT_SFO}:
	${MKSFO} ${SFOFLAGS} '${PSP_EBOOT_TITLE}' $@

