.SUFFIXES:
.SUFFIXES: .o .c

CC     = cc
AR     = ar
RANLIB = ranlib

HDR = \
	util.h

LIBUTILSRC = \
	libutil/enprintf.c \
	libutil/env.c \
	libutil/io.c \
	libutil/proc.c \
	libutil/service.c \
	libutil/strtonum.c \

BIN = \
      sysmgr

LIBUTILOBJ = ${LIBUTILSRC:.c=.o}
LIBUTIL = libutil.a

SRC = ${BIN:=.c}
OBJ = ${SRC:.c=.o} ${LIBUTILOBJ}

all: ${BIN}

${BIN}: ${LIBUTIL}

${OBJ}: ${HDR}

${LIBUTIL}: ${LIBUTILOBJ}
	${AR} rc $@ $?
	${RANLIB} $@

.o:
	${CC} ${LDFLAGS} -o $@ $< ${LIBUTIL} ${LIBS}

.c.o:
	${CC} ${CFLAGS} ${CPPFLAGS} -o $@ -c $<

clean:
	rm -f ${BIN} ${OBJ} ${LIBUTIL}

.PHONY: all clean
