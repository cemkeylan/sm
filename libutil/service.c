/* Service related operations
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
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

#include "../util.h"
#include "../config.h"

service*
sv_init(service *sv, char *sv_name)
{
	sprintf(sv->name, "%s", sv_name);
	sprintf(sv->sysdir, "%s", getenv_fallback("SYSDIR", sysdir_default));
	sprintf(sv->rundir, "%s", getenv_fallback("RUNDIR", rundir_default));
	sprintf(sv->pidfile, "%s/%s/pid", sv->rundir, sv->name);
	sprintf(sv->syspidfile, "%s/%s/syspid", sv->rundir, sv->name);
	sprintf(sv->svfile, "%s/%s", sv->sysdir, sv_name);
	sprintf(sv->svrundir, "%s/%s", sv->rundir, sv_name);
	sprintf(sv->lockfile, "%s/%s/lock", sv->rundir, sv->name);

	return sv;
}

void sv_start(service *sv)
{
	char *arg_list[] = {"runsyssv", sv->svfile, NULL};
	switch(fork()) {
	case 0:
		setsid();
		execvp("runsyssv", arg_list);
		perror("execvp");
		break;
	case -1:
		perror("fork");
		break;
	}
}

int sv_check(service *sv, int force)
{
	/* If force is specified '1', this will return the actual service status
	 * regardless of the lockfile. The lockfile serves the purpose of
	 * stopping services and making sure they are not restarted.
	 */
	pid_t pid;
	struct stat sb;

	if (lstat(sv->lockfile, &sb) == 0) {
		if (force == 1)
			return -1;
		else
			return 0;
	}

	pid = getsyspid(sv);

	if (pid < 0 || checkprocess(pid) != 0)
		return -1;

	return 0;
}

int sv_writelock(char *file, int sig)
{
	FILE *lockfile;

	lockfile = fopen(file, "w");
	if (lockfile == NULL) {
		perror(file);
		return -1;
	}
	fprintf(lockfile, "%d\n", sig);
	fclose(lockfile);

	return 0;
}
