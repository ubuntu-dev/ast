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
 * ccmapchr(ccmap(CC_NATIVE,CC_ASCII),c) and strcmp
 */
#include "config_ast.h"  // IWYU pragma: keep

#include <string.h>

#include "ccode.h"
#include "ast_ccode.h"

#if !_lib_stracmp

int stracmp(const char *aa, const char *ab) {
    unsigned char *a;
    unsigned char *b;
    unsigned char *m;
    int c;
    int d;

    if (!(m = ccmap(CC_NATIVE, CC_ASCII))) return strcmp(aa, ab);
    a = (unsigned char *)aa;
    b = (unsigned char *)ab;
    for (;;) {
        c = m[*a++];
        d = c - m[*b++];
        if (d) return d;
        if (!c) return 0;
    }
}

#endif  // !_lib_stracmp
