/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (C) 2024 Ernestas Kulik <ernestas AT baltic DOT engineering>
 */

#include "jogg-utils.h"

#include <glib.h>

bool
jogg_has_substring ( const char *haystack
                   , const char *needle
                   , bool       *prefix_match)
{
    g_autofree char *haystack_folded = NULL;
    g_autofree char *needle_folded = NULL;
    const char *p = NULL;

    if (NULL == haystack)
    {
        return false;
    }
    if (NULL == needle)
    {
        return false;
    }

    haystack_folded = g_utf8_casefold (haystack, -1);
    needle_folded = g_utf8_casefold (needle, -1);
    p = g_strstr_len (haystack_folded, -1, needle_folded);
    if (NULL == p)
    {
        return false;
    }

    if (prefix_match != NULL)
    {
        *prefix_match = (p == haystack_folded);
    }

    return true;
}
