/* sysmgr-depends -- Wait until given services are up and running
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
	/* Variables used by other functions but not in this file itself */
	(void)(sysdir_default);
	(void)(rundir_default);

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
