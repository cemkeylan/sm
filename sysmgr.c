#include <sys/types.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <libgen.h>

#include "util.h"
static char *argv0;

void
usage(void)
{
	printf("usage: %s\n\nSee sysmgr(8) for detailed information.\n\nVersion: %s\n", argv0, VERSION);
	exit(0);
}


int main(int argc, char *argv[])
{
	argv0 = basename(argv[0]);

	if (argc > 1)
		usage();

	if (setenv("SYSDIR", "/var/sysmgr", 0) == -1)
		die("setenv:");
	if (setenv("RUNDIR", "/run/sysmgr", 0) == -1)
		die("setenv:");

	char *rundir = getenv("RUNDIR");
	char *sysdir = getenv("SYSDIR");

	pid_t pid = getpid();

}
