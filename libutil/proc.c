#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>

#include "../util.h"

pid_t
getsvpid(service *sv)
{
	pid_t pid;

	if (access(sv->pidfile, R_OK) == -1)
		return -1;
	FILE *pidfile;

	pidfile = fopen(sv->pidfile, "r");
	if (pidfile == NULL)
		return -2;

	fscanf(pidfile, "%d", &pid);
	fclose(pidfile);

	return pid;
}

int
writesvpid(char *file, pid_t pid)
{
	if (access(file, W_OK) == -1)
		return -1;

	FILE *pidfile;

	pidfile = fopen(file, "w");
	if (pidfile == NULL)
		/* perror(file); */
		return -2;

	fprintf(pidfile, "%d\n", pid);
	fclose(pidfile);

	return 0;
}

int
checkprocess(int pid)
{
	if (kill(pid, 0) == 0) return 0;
	else {
		switch (errno) {
		case 1:
			/* EPERM is only produced if the process exists, but the
			 * user running the program doesn't have the permissions
			 * to kill the process. We can safely assume that the
			 * process exists in this case and return 0.
			 */
			return 0;
			break;
		default:
			perror("kill");
			break;
		}

		return 1;
	}
}
