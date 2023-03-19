/**
 * @file tdestroy.c
 * @brief replacement for GNU tdestroy()
 */
// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * Copyright (C) 2009, 2018 Rémi Denis-Courmont
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <assert.h>
#include <stdlib.h>
#ifdef HAVE_SEARCH_H
# include <search.h>
#endif

#ifdef HAVE_TFIND
static __thread struct
{
    const void **tab;
    size_t count;
} list = { NULL, 0 };

static void list_nodes(const void *node, const VISIT which, const int depth)
{
    (void) depth;

    if (which != postorder && which != leaf)
        return;

    const void **tab = realloc(list.tab, sizeof (*tab) * (list.count + 1));
    if (tab == NULL)
        abort();

    tab[list.count] = *(const void **)node;
    list.tab = tab;
    list.count++;
}

static __thread const void *smallest;

static int cmp_smallest(const void *a, const void *b)
{
    if (a == b)
        return 0;
    if (a == smallest)
        return -1;
    if (b == smallest)
        return +1;
    abort();
}

void tdestroy(void *root, void (*freenode)(void *))
{
    const void **tab;
    size_t count;

    assert(freenode != NULL);

    /* Enumerate nodes in order */
    assert(list.count == 0);
    twalk(root, list_nodes);
    tab = list.tab;
    count = list.count;
    list.tab = NULL;
    list.count = 0;

    /* Destroy the tree */
    for (size_t i = 0; i < count; i++)
    {
         void *node = (void *)(tab[i]);

         smallest = node;
         node = tdelete(node, &root, cmp_smallest);
         assert(node != NULL);
    }
    assert (root == NULL);

    /* Destroy the nodes */
    for (size_t i = 0; i < count; i++)
         freenode((void *)(tab[i]));
    free(tab);
}
#endif /* HAVE_TFIND */
