VERSION = git

PREFIX = /usr/local
BINDIR = ${PREFIX}/bin
MANPREFIX = ${PREFIX}/share/man

CC     = cc
AR     = ar
RANLIB = ranlib

CPPFLAGS = -D_XOPEN_SOURCE=700 -DVERSION=\"${VERSION}\"
CFLAGS   = -std=c99 -Wpedantic -Wall -Wextra
LDFLAGS  = -static
