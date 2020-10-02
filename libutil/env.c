#include <stdio.h>
#include <stdlib.h>

#include "../util.h"

char*
getenv_fallback(char *name, char *fallback)
{
	if (setenv(name, fallback, 0) == -1) {
		perror(name);
		return NULL;
	}
	return getenv(name);
}
