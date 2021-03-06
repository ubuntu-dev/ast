/***********************************************************************
 *                                                                      *
 *               This software is part of the ast package               *
 *          Copyright (c) 1992-2012 AT&T Intellectual Property          *
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
 *                                                                      *
 ***********************************************************************/
/*
 * AT&T Research
 *
 * builtin cmd definitions
 */
#ifndef _CMD_H
#define _CMD_H 1

#include "ast.h"
#include "error.h"
#include "shcmd.h"
#include "stak.h"

extern int b_stty(int argc, char **argv, Shbltin_t *context);

#define cmdinit _cmd_init

#define ERROR_CALLBACK ERROR_SET

#include "cmdext.h"

#if defined(CMD_BUILTIN) && !defined(CMD_STANDALONE)
#define CMD_STANDALONE CMD_BUILTIN
#endif

#ifdef CMD_STANDALONE

#define CMD_CONTEXT(c) ((Shbltin_t *)0)

#if CMD_DYNAMIC

#include "dlldefs.h"

#else

extern int CMD_STANDALONE(int, char **, Shbltin_t *);

#endif

#ifndef CMD_BUILTIN

/*
 * command initialization
 */

static int cmdinit(int argc, char **argv, Shbltin_t *context, const char *catalog, int flags) {
    UNUSED(argc);
    char *cp;
    char *pp;

    cp = strrchr(argv[0], '/');
    if (cp) {
        cp++;
    } else {
        cp = argv[0];
    }
    pp = strrchr(cp, '_');
    if (pp) cp = pp + 1;
    error_info.id = cp;
    if (!error_info.catalog) error_info.catalog = (char *)catalog;
    opt_info.index = 0;
    if (context) error_info.flags |= flags & ~(ERROR_CALLBACK | ERROR_NOTIFY);
    return 0;
}

#endif

int main(int argc, char **argv) {
#if CMD_DYNAMIC
    char *s;
    char *t;
    void *dll;
    Shbltin_f fun;
    char buf[64];

    if (s = strrchr(argv[0], '/'))
        s++;
    else if (!(s = argv[0]))
        return 127;
    if ((t = strrchr(s, '_')) && *++t) s = t;
    buf[0] = '_';
    buf[1] = 'b';
    buf[2] = '_';
    // Is it okay if the string is truncated?
    strlcpy(buf + 3, s, sizeof(buf) - 3);
    t = strchr(buf, '.');
    if (t) *t = 0;
    for (;;) {
        dll = dlopen(NULL, RTLD_LAZY);
        if (dll) {
            fun = (Shbltin_f)dlsym(dll, buf + 1);
            if (fun) break;
            fun = (Shbltin_f)dlsym(dll, buf);
            if (fun) break;
        }
        dll = dllplug(NULL, "cmd", NULL, RTLD_LAZY, NULL, 0);
        if (dll) {
            fun = (Shbltin_f)dlsym(dll, buf + 1);
            if (fun) break;
            fun = (Shbltin_f)dlsym(dll, buf);
            if (fun) break;
        }
        return 127;
    }
    return (*fun)(argc, argv, NULL);
#else
    return CMD_STANDALONE(argc, argv, NULL);
#endif
}

#else

#undef cmdinit
#ifdef _MSC_VER
#define CMD_CONTEXT(p) ((Shbltin_t *)(p))
#define cmdinit(a, b, c, d, e)                   \
    do {                                         \
        if (_cmd_init(a, b, c, d, e)) return -1; \
    } while (0)
#else
#define CMD_CONTEXT(p)                                                                          \
    (((p) && ((Shbltin_t *)(p))->version >= 20071012 && ((Shbltin_t *)(p))->version < 20350101) \
         ? ((Shbltin_t *)(p))                                                                   \
         : 0)
#define cmdinit(a, b, c, d, e)                   \
    do {                                         \
        if ((c) && !CMD_CONTEXT(c)) c = 0;       \
        if (_cmd_init(a, b, c, d, e)) return -1; \
    } while (0)
#endif

extern int _cmd_init(int, char **, Shbltin_t *, const char *, int);

#endif

#endif  // _CMD_H
