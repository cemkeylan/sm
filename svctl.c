#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <dirent.h>
#include <libgen.h>

#include "util.h"
#include "config.h"

static char *argv0;

void
usage(int exitnum)
{
	printf("usage: %s [operation] [service...]\n\n", argv0);
	fputs("  Operations:\n"
	      "    start/stop/restart  Start/stop/restart services\n"
	      "    once                Start services once\n"
	      "    status              Check service statuses\n"
	      "    up/down             Same as start/stop\n\n"
	      "sysmgr-"VERSION"\n", stdout);
	exit(exitnum);
}

int check_rundir(char *dir)
{
	DIR *rundirectory;
	rundirectory = opendir(dir);
	if (rundirectory == NULL)
		return -1;
	closedir(rundirectory);
	return 0;
}

int handle_service(char *operation, char *name)
{
	struct service sv;
	sv_init(&sv, basename(name));
	pid_t pid;

	if (strcmp(operation, "start") == 0 || strcmp(operation, "up") == 0) {
		if(sv_check(&sv, 1) == 0)
			return 0;
		return rm_rf(sv.svrundir);
	} else if (strcmp(operation, "stop") == 0 || strcmp(operation, "down") == 0) {
		pid = getsyspid(&sv);
		switch (pid) {
		case -1:
			perror(NULL);
			return 1;
		default:
			sv_writelock(sv.lockfile, SIGTERM);
			kill(pid, SIGTERM);
			unlink(sv.syspidfile);
			unlink(sv.pidfile);
		}
	} else if (strcmp(operation, "kill") == 0) {
		pid = getsyspid(&sv);
		switch (pid) {
		case -1:
			perror(NULL);
			return 1;
		default:
			sv_writelock(sv.lockfile, SIGKILL);
			kill(pid, SIGUSR1);
			unlink(sv.syspidfile);
			unlink(sv.pidfile);
		}
	} else if (strcmp(operation, "once") == 0) {
		if(sv_check(&sv, 1) == 0) {
			if (sv_writelock(sv.lockfile, 0) != 0) {
				perror(sv.lockfile);
				die("Failed to write lock.");
			}
		} else {
			rm_rf(sv.svrundir);
			sleep(1);
			return sv_writelock(sv.lockfile, 0);
		}
	} else if (strcmp(operation, "restart") == 0) {
		handle_service("stop", name);
		while (1) {
			if (sv_check(&sv, 1) != 0)
				break;
		}
		handle_service("start", name);

	} else if (strcmp(operation, "stat") == 0 || strcmp(operation, "status") == 0) {
		if (sv_check(&sv, 1) < 0) {
			fprintf(stderr, "%s: DOWN\n", name);
			return -1;
		} else
			fprintf(stderr, "%s: UP\n", name);
	} else
		die("Unknown operation: %s", operation);
	return 0;
}

int
main(int argc, char *argv[])
{
	/* Variable used by other functions but not in this file itself */
	(void)(sysdir_default);

	char *rundir;
	argv0 = argv[0];
	int i, exitnum;

	if (argc < 2 || strncmp(argv[1], "-", 1) == 0)
		usage(0);
	else if (argc < 3)
		usage(1);

	/* Check if the RUNDIR exists */
	rundir = getenv_fallback("RUNDIR", rundir_default);
	if (check_rundir(rundir) != 0)
		die("%s could not be found, are you sure sysmgr is running?", rundir);

	if (strncmp(argv[1], "stat", 4) == 0) {
		exitnum = 0;
		for (i=2; i < argc; i++)
			if(handle_service(argv[1], argv[i]) != 0)
				exitnum = 1;
		exit(exitnum);
	}

	for (i=2; i < argc; i++)
		if (handle_service(argv[1], argv[i]) != 0)
			die("Couldn't %s %s", argv[1], argv[i]);

	return 0;
}
