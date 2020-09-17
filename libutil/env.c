#include <stdlib.h>

#include "../util.h"

char*
getenv_fallback(char *name, char *fallback)
{
	char *value = getenv(name);
	if (!value)
		value = fallback;
	return value;
}
