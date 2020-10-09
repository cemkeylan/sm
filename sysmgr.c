/* sysmgr -- A simplistic service supervisor.
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
#include <dirent.h>
#include <string.h>

#include "util.h"
#include "config.h"
static char *argv0, *rundir, *sysdir;

void
term(int sig)
{
	/* We are ignoring the sig variable */
	(void)(sig);

	DIR *dir;
	struct dirent *ent;

	if ((dir = opendir(rundir)) == NULL)
		die("%s:", rundir);

	while ((ent = readdir(dir)) != NULL) {
		char realfile[PATH_MAX];
		sprintf(realfile, "%s/%s", rundir, ent->d_name);

		printf("%s\n", ent->d_name);

		if (strncmp(ent->d_name, ".", 1) == 0 ||
		    strcmp(ent->d_name, "pid") == 0)
			continue;

		printf("%s\n", ent->d_name);
		struct service sv;
		sv_init(&sv, ent->d_name);
		pid_t pid = getsyspid(&sv);
		if (pid == -1) {
			perror(sv.name);
			continue;
		}
		if (kill(pid, SIGTERM) != 0)
			perror("kill");
	}
	sleep(1);
	rm_rf(rundir);
	exit(0);
}

void
usage(void)
{
	printf("usage: %s\n\nSee sysmgr(8) for detailed information.\n\nVersion: %s\n", argv0, VERSION);
	exit(1);
}


int main(int argc, char *argv[])
{
	argv0 = argv[0];
	char sysmgr_pidfile[PATH_MAX];

	if (argc > 1)
		usage();

	sysdir = getenv_fallback("SYSDIR", sysdir_default);
	rundir = getenv_fallback("RUNDIR", rundir_default);
	snprintf(sysmgr_pidfile, PATH_MAX, "%s/pid", rundir);

	mkdirp(rundir);

	/* Trap signals */
	int sv_signals[] = {SIGTERM, SIGINT, SIGHUP, SIGQUIT, SIGABRT};
	for (long unsigned int i=0; i < sizeof(sv_signals); i++)
		signal(sv_signals[i], term);

	pid_t pid = getpid();

	if (writesvpid(sysmgr_pidfile, pid) != 0)
		die("%s:", sysmgr_pidfile);

	/* Go to the service directory and get all the service entries. */
	DIR *dir;
	struct dirent *ent;
	while(1) {
		if ((dir = opendir(sysdir)) == NULL)
			die("%s:", sysdir);
		while ((ent = readdir(dir)) != NULL) {
			if (strncmp(ent->d_name, ".", 1) == 0)
				continue;
			struct service sv;
			sv_init(&sv, ent->d_name);
			if (sv_check(&sv, 0) != 0) {
				sv_start(&sv);
			}
		}
		closedir(dir);
		sleep(1);
	}
	wait(NULL);
	term(SIGTERM);

	return 0;
}
