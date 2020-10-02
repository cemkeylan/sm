CC     = cc
AR     = ar
RANLIB = ranlib

CPPFLAGS = -D_XOPEN_SOURCE=700 -D_GNU_SOURCE
CFLAGS   = -std=c99 -Wpedantic -Wall -Wextra
LDFLAGS  = -static

