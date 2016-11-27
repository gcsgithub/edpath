static const char *rcsid="@(#) $Id: edpath.c,v 1.3 2010/12/06 23:27:24 mark Exp mark $";
#define COPYRIGHT "Copyright (c) 2008-2010  Garetech Computer Soltions, Mark Garrett "
/*
 *  edpath.c
 *  edpath
 *
 *  Created by mark on 02/12/2010.
 *  Copyright 2010 Garetech Computer Solutions. All rights reserved.
 * edpath <path1> <path2> ..
 * merge path2 into path1 returning via stdout newpath
 * -v or -d to display to stderr path merg opertaions eg path dropped.
 * $Log: edpath.c,v $
 * Revision 1.3  2010/12/06 23:27:24  mark
 * add copyright notice for strsep (BSD)
 *
 * Revision 1.2  2010/12/06 22:03:56  mark
 * add back -V
 * setup progid from argv[0]
 *
 * Revision 1.1  2010/12/03 21:29:35  mark
 * Initial revision
 *
 *
 *
 */

#include "edpath.h"


edpath_global_t EDPATH;

void do_cmd_help(FILE *fd)
{
    fprintf(fd, "\tmerge and dedup <pathX> into <path1> returning via stdout newpath\n"
            " there maybe one ore many paths to merge over <path1>"
            "\t or even just %s <path> to remove duplicates from a single path\n"
            "\t-v or -d to display to stderr path merg opertaions eg path dropped.\n"
            "\t-r <path>  remove the specified path from the merged list\n", EDPATH.progid);
}

void display_copyright()
{
    fprintf(stderr, "# Version: %s\n", rcsid+9);
    fprintf(stderr, "# %s\n", COPYRIGHT);
#ifdef NEED_STRSEP
    fprintf(stderr,
            "# strsep has been included which is :-\n"
            "# Copyright (c) 1990, 1993\n"
            "#      The Regents of the University of California.  All rights reserved.\n"
            "#\n"
            "# Redistribution and use in source and binary forms, with or without\n"
            "# modification, are permitted provided that the following conditions\n"
            "# are met:\n"
            "# 1. Redistributions of source code must retain the above copyright\n"
            "#    notice, this list of conditions and the following disclaimer.\n"
            "# 2. Redistributions in binary form must reproduce the above copyright\n"
            "#    notice, this list of conditions and the following disclaimer in the\n"
            "#    documentation and/or other materials provided with the distribution.\n"
            "# 3. All advertising materials mentioning features or use of this software\n"
            "#    must display the following acknowledgement:\n"
            "#      This product includes software developed by the University of\n"
            "#      California, Berkeley and its contributors.\n"
            "# 4. Neither the name of the University nor the names of its contributors\n"
            "#    may be used to endorse or promote products derived from this software\n"
            "#    without specific prior written permission.\n"
            "#\n"
            "# THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND\n"
            "# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE\n"
            "# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE\n"
            "# ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE\n"
            "# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL\n"
            "# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS\n"
            "# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)\n"
            "# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT\n"
            "# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY\n"
            "# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF\n"
            "# SUCH DAMAGE.\n"
            "\n");
#endif /* NEED_STRSEP */
}

/****************************/
void    usage(const char *prg)
/****************************/
{
    fprintf(stderr,"# Usage: %s\t[-d] [-v] [-r <path> ] <path1> <path2> \n", prg);
    do_cmd_help(stderr);
    exit(EINVAL);
}

/****************************************************/
int main(int argc,  char *argv[] /* , char *envp[] */)
/****************************************************/
{
    int             dousage=0;
    signed char     c;
    int             pidx;
    extern char     *optarg;
    extern int      optind;
    
    path_t          *path_head;
    path_t          *path_remove_head;
    path_t          *rpath;
    
    
    (void)setlocale(LC_ALL, "");
    bzero(&EDPATH, sizeof(EDPATH));
    EDPATH.progid = strdup(basename(argv[0]));
    
    path_head = NULL;
    path_remove_head = NULL;
    
    optarg = NULL;
    while (!dousage && ((c = getopt(argc, argv, "r:dvVh?")) != -1)) {
        switch (c) {
            case 'r':
                EDPATH.remove++;
                path_merge(&path_remove_head, optarg);
                break;
            case 'd':
                EDPATH.debug++;
                break;
            case 'v':
                EDPATH.verbose++;
                break;
            case 'V':
                display_copyright();
                exit(0);
                break;
            case 'h':
                usage(EDPATH.progid);
                exit(0);
                break;
            case -1:
                break;
            default:
                dousage++;
                break;
        }
    }
    
    argc -= optind;
    argv += optind;
    if (argc<1) {
        dousage++;
    }
    
    if (dousage) usage(EDPATH.progid); // will exit via usage
    
    for (pidx=0;pidx<=argc;pidx++) {
        path_merge(&path_head, argv[pidx]);
    }
    
    if (path_remove_head) {
        rpath = path_remove_head;
        while (rpath) {
            path_remove(&path_head, rpath->path);
            rpath = rpath->next;
        }
    }

    path_print(stdout, path_head);

}
/**********************************************/
/**********************************************/
/**********************************************/
/**********************************************/


/**********************************************/
int path_print(FILE *fd, path_t *path_head)
/**********************************************/
{
    int outlen;
    outlen = 0;
    
    if (path_head) {
        outlen += fprintf(fd, "%s", path_head->path);
        while(path_head->next) {
            path_head = path_head->next;
            outlen += fprintf(fd, ":%s", path_head->path);
        }
        fputc('\n', fd);
        outlen++;
    }
    return(outlen);
}

/*********************************************/
void path_remove(path_t **pathhead, char *path)
/*********************************************/
{
    path_t *pathe;
    pathe = path_find(pathhead, path);
    if (pathe) {
        if (EDPATH.verbose) {
            fprintf(stderr, "# removing \"%s\"\n", path);
        }
        if (pathe->prev) {
            pathe->prev->next = pathe->next;
        }
        else {
            *pathhead = pathe->next;
        }
        
        if (pathe->next) {
            pathe->next->prev = pathe->prev;
        }
    }
}

/**************************************************/
void path_insert_tail(path_t **pathhead, char *path)
/**************************************************/
{
    path_t  *pathe, *pcur;
    
    pathe = path_new(path); // always wins or exits
    
    if (pathhead) {
        pcur = *pathhead;
        if (!pcur) {        // no list create one
            *pathhead = pathe;
            pathe->next = pathe->prev = NULL;
        }
        else {
            while(pcur->next)
                pcur = pcur->next;
            // pcur is now the last entry of the current list
            pcur->next = pathe;
            pathe->prev = pcur;
            pathe->next = NULL;
        }
    }
}


/*********************************************/
void path_merge(path_t **pathhead, char *path)
/*********************************************/
{
    char   *pe;
    path_t  *fnd;
    
    while((pe = strsep(&path,":")) ) {
        fnd = path_find(pathhead, pe);
        if (fnd) {
            if (EDPATH.verbose)
                fprintf(stderr, "# dropping duplicate path name: \"%s\"\n", pe);
        }
        else {
            path_insert_tail(pathhead, pe);
        }
    }
}


/*********************************************/
path_t *path_find(path_t **pathhead, char *pe)
/*********************************************/
{
    // return either a pointer to the found matching element
    // or the insert point at the end of the list
    // or NULL for nolist
    path_t  *path;
    
    if (pathhead) {
        path = *pathhead;
        while(path) {
            if (strcmp(path->path, pe) == 0 ) { // found it
                return(path);
            }
            path=path->next;
        };
    }
    return(NULL); // nothing found
}

/****************************/
path_t  *path_new(char *path)
/****************************/
{
    path_t *rval;
    
    rval = malloc(sizeof(path_t));
    if (rval == NULL) {
        fprintf(stderr, "#error allocating memory for path_new()\n");
        exit(ENOMEM);
    }
    
    bzero(rval, sizeof(path_t));
    if (path) {
        rval->path = malloc(strlen(path)+1);
        if (rval->path == NULL) {
            fprintf(stderr, "#error allocting memory for path element\n");
            exit(ENOMEM);
        }
        strcpy(rval->path, path);
    }
    return(rval);
}

/**************************/
void path_free(path_t *path)
/**************************/
{
    if (path) {
        if (path->path) {
            free(path->path);
        }
        free(path);
    }
}




