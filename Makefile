include config.mk

.SUFFIXES:
.SUFFIXES: .o .c

HDR = \
	util.h

LIBUTILSRC = \
	libutil/enprintf.c \
	libutil/env.c \
	libutil/io.c \
	libutil/mkdirp.c \
	libutil/proc.c \
	libutil/service.c \
	libutil/strlcpy.c \
	libutil/strtonum.c \


BIN = \
      sysmgr \
      runsyssv

SRC = ${BIN:=.c}
BINOBJ = ${SRC:.c=.o}

LIBUTILOBJ = ${LIBUTILSRC:.c=.o}
LIBUTIL = libutil.a

OBJ = ${BINOBJ} ${LIBUTILOBJ}

all: ${BIN}

${BIN}: ${LIBUTIL}

${OBJ}: ${HDR}

${BINOBJ}: config.h

${LIBUTIL}: ${LIBUTILOBJ}
	${AR} rc $@ $?
	${RANLIB} $@

.o:
	${CC} ${LDFLAGS} -o $@ $< ${LIBUTIL} ${LIBS}

.c.o:
	${CC} ${CFLAGS} ${CPPFLAGS} -o $@ -c $<

config.h:
	cp config.def.h $@

clean:
	rm -f ${BIN} ${OBJ} ${LIBUTIL}

.PHONY: all clean
