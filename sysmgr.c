#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <dirent.h>
#include <string.h>

#include "util.h"
#include "config.h"
static char *argv0;

void
usage(void)
{
	printf("usage: %s\n\nSee sysmgr(8) for detailed information.\n\nVersion: %s\n", argv0, VERSION);
	exit(1);
}


int main(int argc, char *argv[])
{
	argv0 = argv[0];
	char *sysdir, *rundir;
	char sysmgr_pidfile[PATH_MAX];

	if (argc > 1)
		usage();

	sysdir = getenv_fallback("SYSDIR", sysdir_default);
	rundir = getenv_fallback("RUNDIR", rundir_default);
	snprintf(sysmgr_pidfile, PATH_MAX, "%s/pid", rundir);

	mkdirp(rundir);

	pid_t pid = getpid();

	printf("RUNDIR: %s\nSYSDIR: %s\nselfpid: %d\n", rundir, sysdir, pid);
	if (writesvpid(sysmgr_pidfile, pid) != 0)
		die("%s:", sysmgr_pidfile);

	/* Go to the service directory and get all the service entries. */
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(sysdir)) == NULL)
		die("%s:", sysdir);
	while ((ent = readdir(dir)) != NULL) {
		if (strncmp(ent->d_name, ".", 1) == 0)
			continue;
		printf("%s\n", ent->d_name);

		struct service sv;
		sv_init(&sv, ent->d_name);

	}
	closedir(dir);

	return 0;
}
