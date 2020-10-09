#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <libgen.h>
#include <string.h>
#include <dirent.h>

#include "util.h"
#include "config.h"
static char *argv0, *lockfile, *svrundir;
static pid_t syspid, svpid;
static struct service sv;

void
usage(int exitnum)
{
	printf("usage: %s [svfile]\n", argv0);
	exit(exitnum);
}

void
term(int sig)
{
	switch(sig) {
	case SIGUSR1:
		kill(svpid, SIGKILL);
		break;
	default:
		kill(svpid, SIGTERM);
		break;
	}
	if (sv_writelock(lockfile, sig) != 0)
		rm_rf(svrundir);
	exit(0);
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

	syspid = getpid();

	struct service sv;
	sv_init(&sv, basename(argv[1]));

	DIR *dir;
	if ((dir = opendir(sv.svrundir)) != NULL) {
		closedir(dir);
		return 1;
	}

	mkdirp(sv.svrundir);

	int sv_signals[] = {SIGTERM, SIGINT, SIGHUP, SIGQUIT, SIGABRT};
	lockfile = sv.lockfile;
	svrundir = sv.svrundir;

	svpid = fork();
	switch(svpid) {
	case -1:
		die("fork:");
		break;
	case 0:
		execvp(sv.svfile, argv);
		perror(sv.svfile);
		break;
	default:
		writesvpid(sv.syspidfile, syspid);
		writesvpid(sv.pidfile, svpid);
		for (long unsigned int i=0; i < sizeof(sv_signals); i++)
			signal(sv_signals[i], term);
		wait(NULL);
		if (rm_rf(sv.svrundir) != 0)
			return 1;
		break;
	}
	return 0;
}
