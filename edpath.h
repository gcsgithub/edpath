#ifndef __EDPATH_H__
#define __EDPATH_H__
/*
 * @(#) $Id: edpath.h,v 1.2 2010/12/06 23:28:03 mark Exp mark $
 *
 *  edpath.h
 *  edpath
 *
 *  Created by mark on 26/10/2010.
 *  Copyright 2010 Garetech Computer Solutions. All rights reserved.
 * $Log: edpath.h,v $
 * Revision 1.2  2010/12/06 23:28:03  mark
 * include strings.h for hpux
 *
 * Revision 1.1  2010/12/03 21:29:38  mark
 * Initial revision
 *
 *
 */
#include    <arpa/inet.h>
#include    <ctype.h>
#include    <errno.h>
#include    <math.h>
#include    <netdb.h>
#include    <netinet/in.h>
#include    <regex.h>
#include    <setjmp.h>
#include    <signal.h>
#include    <stdarg.h>
#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#ifndef _AIX
#include    <sys/fcntl.h>
#endif
#include    <sys/param.h>
#include    <sys/socket.h>
#include    <sys/types.h>
#include    <sys/uio.h>
#include    <sys/wait.h>
#include    <syslog.h>
#include    <time.h>
typedef struct tm       tm_t;
#include    <unistd.h>
#ifndef __hpux
#ifndef _AIX
#include    <getopt.h>
#endif
#include    <strings.h>
#endif
#include    <locale.h>
#include    <sys/stat.h>
#include    <fcntl.h>
#include    <limits.h>
#include    <libgen.h>
#include    <sys/types.h>

#ifndef __PRETTY_FUNCTION__
#define __PRETTY_FUNCTION__ __FILE__
#endif
#define cpp_str(x)  #x
#define __ATLINE__ __PRETTY_FUNCTION__ ":"  cpp_str(__LINE__)


typedef		unsigned long long llu_t;

typedef struct {
    char 	*progid;
    int		quiet;
    int		verbose;
    int		debug;
    int     remove;
    //char	*result_path;
} edpath_global_t;

typedef struct _path path_t;
struct _path {
    path_t  *prev, *next;
    char    *path;
};


path_t  *path_new(char *path);
void    path_free(path_t *path);

path_t *path_find(path_t **pathhead, char *pe);
void    path_merge(path_t **pathhead, char *path);
void    path_remove(path_t **pathhead, char *path);
void    path_insert_tail(path_t **pathhead, char *path);
int     path_print(FILE *fd, path_t *path_head);


#endif /* __EDPATH_H__ */
