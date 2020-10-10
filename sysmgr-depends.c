#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "util.h"
#include "config.h"
static char *argv0;

void
usage(void)
{
	printf("usage: %s [service...]\n\nsysmgr-%s\n", argv0, VERSION);
	exit(0);
}

int
main (int argc, char *argv[])
{
	int i;
	argv0 = argv[0];
	if (argc < 2 || strncmp(argv[1], "-", 1) == 0)
		usage();

	/* Make a check that the services are actually valid. */
	for (i=1; i < argc; i++) {
		struct service sv;
		sv_init(&sv, argv[i]);
		if (access(sv.svfile, R_OK) < 0)
			die("service file for '%s' could not be found", argv[i]);
	}

	for (i=1; i < argc; i++) {
		struct service sv;
		sv_init(&sv, argv[i]);
		while(1) {
			if (sv_check(&sv, 1) == 0)
				break;
			sleep(1);
		}
	}
}
