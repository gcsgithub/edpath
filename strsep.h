#ifndef __STRSEP_H__
#define __STRSEP_H__
/*
 * @(#) $Id: strsep.h,v 1.1 2010/12/03 21:29:45 mark Exp $
 * $Log: strsep.h,v $
 * Revision 1.1  2010/12/03 21:29:45  mark
 * Initial revision
 *
 * Revision 1.3  2008/10/20  13:01:38  mark
 * checkpoint
 *
 * Revision 1.2  2008/09/27 13:11:22  mark
 * initial checkin
 *
 *
 *  strsep.h
 *  blib
 *
 *  Created by mark on 26/09/2008.
 *  Copyright 2008 Garetech Computer Solutions. All rights reserved.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#ifdef NEED_STRSEP
char *strsep(char **stringp, const char *delim);
#endif

#endif /*  __STRSEP_H__ */
