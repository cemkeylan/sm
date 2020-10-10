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
	libutil/rm.c \
	libutil/service.c \
	libutil/strlcpy.c


BIN = \
	sysmgr \
	runsyssv \
	svctl \
	sysmgr-depends

MAN = \
	man/runsyssv.1 \
	man/svctl.1 \
	man/sysmgr-depends.1 \
	man/sysmgr.8


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

install: all
	mkdir -p ${DESTDIR}${BINDIR}
	cp ${BIN} ${DESTDIR}${BINDIR}
	for bin in ${BIN}; do chmod 755 ${DESTDIR}${BINDIR}/$${bin}; done
	for man in ${MAN}; do \
		mkdir -p ${DESTDIR}${MANPREFIX}/man$${man##*.}; \
		cp $${man} ${DESTDIR}${MANPREFIX}/man$${man##*.}/$${man##*/}; \
		chmod 644 ${DESTDIR}${MANPREFIX}/man$${man##*.}/$${man##*/}; \
	done

uninstall:
	for bin in ${BIN}; do rm -f ${DESTDIR}${BINDIR}/$${bin}; done
	for man in ${MAN}; do rm -f ${DESTDIR}${MANPREFIX}/man$${man##*.}/$${man##*/}; done

.PHONY: all clean install uninstall
