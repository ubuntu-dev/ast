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
 * AT&T Bell Laboratories
 *
 * hash table library
 */
#include "config_ast.h"  // IWYU pragma: keep

#include <stddef.h>

#include "hashlib.h"

/*
 * push/pop/query hash table scope
 *
 *	bot==0		pop top scope
 *	bot==top	query
 *	bot!=0		push top on bot
 *
 * scope table pointer returned
 */

Hash_table_t *hashview(Hash_table_t *top, Hash_table_t *bot) {
    Hash_bucket_t *b;
    Hash_bucket_t *p;
    Hash_bucket_t **sp;
    Hash_bucket_t **sx;

    if (!top || top->frozen)
        bot = 0;
    else if (top == bot)
        bot = top->scope;
    else if (bot) {
        if (top->scope)
            bot = 0;
        else {
            sx = &top->table[top->size];
            sp = &top->table[0];
            while (sp < sx) {
                for (b = *sp++; b; b = b->next) {
                    p = (Hash_bucket_t *)hashlook(bot, b->name, HASH_LOOKUP, NULL);
                    if (p) {
                        b->name = (p->hash & HASH_HIDES) ? p->name : (char *)b;
                        b->hash |= HASH_HIDES;
                    }
                }
            }
            top->scope = bot;
            bot->frozen++;
        }
    } else {
        bot = top->scope;
        if (bot) {
            sx = &top->table[top->size];
            sp = &top->table[0];
            while (sp < sx)
                for (b = *sp++; b; b = b->next)
                    if (b->hash & HASH_HIDES) {
                        b->hash &= ~HASH_HIDES;
                        b->name = ((Hash_bucket_t *)b->name)->name;
                    }
            top->scope = 0;
            bot->frozen--;
        }
    }
    return (bot);
}
