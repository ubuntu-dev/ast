/***********************************************************************
 *                                                                      *
 *               This software is part of the ast package               *
 *          Copyright (c) 1985-2011 AT&T Intellectual Property          *
 *                      and is licensed under the                       *
 *                 Eclipse Public License, Version 1.0                  *
 *                    by AT&T Intellectual Property                     *
 *                                                                      *
 *                A copy of the License is available at                 *
 *          http://www.eclipse.org/org/documents/epl-v10.html           *
 *         (with md5 checksum b35adb5213ca9657e911e9befb180842)         *
 *                                                                      *
 *              Information and Software Systems Research               *
 *                            AT&T Research                             *
 *                           Florham Park NJ                            *
 *                                                                      *
 *               Glenn Fowler <glenn.s.fowler@gmail.com>                *
 *                    David Korn <dgkorn@gmail.com>                     *
 *                     Phong Vo <phongvo@gmail.com>                     *
 *                                                                      *
 ***********************************************************************/
/*
 * Glenn Fowler
 * AT&T Research
 */
#include "config_ast.h"  // IWYU pragma: keep

#include <sys/types.h>

#include "ast.h"

/*
 * return a pointer to the element matching
 * name in the (*comparf*)() sorted tab of num elements of
 * size siz where the first member of each
 * element is a char*
 *
 * 0 returned if name not found
 */

void *strsearch(const void *tab, size_t num, size_t siz, Strcmp_context_f comparf, const char *name,
                void *context) {
    char *lo = (char *)tab;
    char *hi = lo + (num - 1) * siz;
    char *mid;
    int v;

    while (lo <= hi) {
        mid = lo + (((hi - lo) / siz) / 2) * siz;
        v = (*comparf)(name, *((char **)mid), context);
        if (v == 0) {
            return (void *)mid;
        } else if (v > 0) {
            lo = mid + siz;
        } else {
            hi = mid - siz;
        }
    }
    return 0;
}
