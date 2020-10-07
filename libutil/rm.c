/* Remove directories
 *
 * This file is part of sysmgr.
 *
 * Function mostly taken from:
 * https://stackoverflow.com/a/5467788
 * Licensed under CC BY-SA 2.5
 */
#include <stdio.h>
#include <stdlib.h>
#include <ftw.h>

#define UNUSED(x) (void)(x)

#include "../util.h"

static int
rm_files(const char *pathname, const struct stat *sbuf, int type, struct FTW *ftwb)
{
	UNUSED(sbuf);
	UNUSED(ftwb);
	UNUSED(type);

	if (remove(pathname) < 0) {
		perror("remove");
		return -1;
	}
	return 0;
}

int
rm_rf(char *path)
{
	if (nftw(path, rm_files, 64, FTW_DEPTH|FTW_PHYS) < 0) {
		perror("nftw");
		return -1;
	}
	return 0;
}
