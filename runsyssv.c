#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#include "util.h"
#include "config.h"
static char *argv0;

void
usage(int exitnum)
{
	printf("usage: %s [svfile]\n", argv0);
	exit(exitnum);
}

int
main(int argc, char *argv[])
{
	argv0 = argv[0];
	char *sysdir, *rundir;
	sysdir = getenv_fallback("SYSDIR", sysdir_default);
	rundir = getenv_fallback("RUNDIR", rundir_default);

	if (argc == 2) {
		if (strncmp(argv[1], "-", 1) == 0)
			usage(0);
	} else
		usage(1);

	struct service sv;
	sv_init(&sv, argv[1]);
	mkdirp(sv.svrundir);

}
