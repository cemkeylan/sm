sysmgr
======

sysmgr is a simplistic service manager written in C99. It reads the service
scripts from the given SYSDIR and executes them asynchronously via the runsyssv
program.

The default service directory and run directory can be configured by editing
the config.h file.


Installation
------------

Edit config.mk to match your setup, and then install by doing the following.

    make
    make install


Using sysmgr
------------

Detailed information on how to use sysmgr can be found on manual pages. See
sysmgr(8).
