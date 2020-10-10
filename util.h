/* See LICENSE for copyright information */

#ifndef _UTIL_H
#define _UTIL_H
#ifndef _LIMITS_H
#include <limits.h>
#endif

#ifndef _TYPES_H
#include <sys/types.h>
#endif

#ifndef VERSION
#define VERSION "git"
#endif

typedef struct service {
	char name[NAME_MAX];
	char sysdir[PATH_MAX];
	char rundir[PATH_MAX];
	char pidfile[PATH_MAX];
	char syspidfile[PATH_MAX];
	char svfile[PATH_MAX];
	char svrundir[PATH_MAX];
	char lockfile[PATH_MAX];
} service;

/* service.c */
service *sv_init(service *sv, char *sv_name);
void sv_start(service *sv);
int  sv_check(service *sv, int force);
int  sv_writelock(char *file, int sig);

/* proc.c */
int getsvpid(service *sv);
int getsyspid(service *sv);
int checkprocess(int pid);
int writesvpid(char *file, pid_t pid);

/* env.c */
char *getenv_fallback(char *name, char *fallback);
void *setenv_fallback(char *name, char *fallback);

/* io.c */
void die(const char *msg, ...);

/* enprintf.c */
void enprintf(int, const char *, ...);
void eprintf(const char *, ...);
void weprintf(const char *, ...);

/* strtonum.c */
long long strtonum(const char *, long long, long long, const char **);
long long enstrtonum(int, const char *, long long, long long);
long long estrtonum(const char *, long long, long long);

/* mkdirp.c */
int mkdirp(const char *path);

/* estrlcpy.c */
size_t strlcpy(char *, const char *, size_t);
size_t estrlcpy(char *, const char *, size_t);

/* rm.c */
int rm_rf(char *path);

#endif
