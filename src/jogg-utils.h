/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (C) 2024 Ernestas Kulik <ernestas AT baltic DOT engineering>
 */

#pragma once

#include <stdbool.h>

bool jogg_has_substring ( const char *haystack
                        , const char *needle
                        , bool       *prefix);
