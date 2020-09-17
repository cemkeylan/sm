/* See LICENSE for copyright information */

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
  char svfile[PATH_MAX];
} service;

/*
 * Get information about the service.
 */
service *sv_init(service *sv, char *sv_name);

/* proc.c */
int getsvpid(service *sv);
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
