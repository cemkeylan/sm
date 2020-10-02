#include <stdio.h>

#include "../util.h"

service*
sv_init(service *sv, char *sv_name)
{
	sprintf(sv->name, "%s", sv_name);
	sprintf(sv->sysdir, "%s", getenv_fallback("SYSDIR", "/var/sysmgr"));
	sprintf(sv->rundir, "%s", getenv_fallback("RUNDIR", "/run/sysmgr"));
	sprintf(sv->pidfile, "%s/%s/pid", sv->rundir, sv->name);
	sprintf(sv->svfile, "%s/%s", sv->sysdir, sv_name);
	sprintf(sv->svrundir, "%s/%s", sv->rundir, sv_name);

	return sv;
}
