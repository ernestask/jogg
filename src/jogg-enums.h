#pragma once

/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (C) 2024 Ernestas Kulik <ernestas AT baltic DOT engineering>
 */

#include <glib.h>

G_BEGIN_DECLS

typedef enum
{
    JOGG_MATCH_TYPE_INVALID,
    JOGG_MATCH_TYPE_NAME,
    JOGG_MATCH_TYPE_GENERIC_NAME,
    JOGG_MATCH_TYPE_ACTIONS,
    JOGG_MATCH_TYPE_KEYWORDS,
    JOGG_MATCH_TYPE_EXEC,
} JoggMatchType;

G_END_DECLS
