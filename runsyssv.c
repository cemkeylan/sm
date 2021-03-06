/* runsyssv -- Run and control service scripts
 *
 * Copyright (C) 2020 Cem Keylan <cem@ckyln.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
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

void
usage(int exitnum)
{
	printf("usage: %s [svfile]\n\nsysmgr-%s\n", argv0, VERSION);
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
	/* Variables used by other functions but not in this file itself */
	(void)(sysdir_default);
	(void)(rundir_default);

	argv0 = argv[0];

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
