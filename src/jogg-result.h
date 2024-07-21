/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (C) 2024 Ernestas Kulik <ernestas AT baltic DOT engineering>
 */

#pragma once

#include "jogg-enums.h"
#include "jogg-types.h"

#include <gio/gio.h>
#include <gio/gdesktopappinfo.h>
#include <gtk/gtk.h>
#include <stdbool.h>

G_BEGIN_DECLS

G_DECLARE_FINAL_TYPE ( JoggResult
                     , jogg_result
                     , JOGG
                     , RESULT
                     , GObject
                     );

GDesktopAppInfo *jogg_result_get_app_info    (JoggResult *self);
bool             jogg_result_is_prefix_match (JoggResult *self);
JoggMatchType    jogg_result_get_match_type  (JoggResult *self);

GtkOrdering jogg_result_compare ( const JoggResult *lhs
                                , const JoggResult *rhs);

JoggResult *jogg_result_new ( GDesktopAppInfo *app_info
                            , const char      *action
                            , JoggMatchType    match_type
                            , bool             prefix_match);

G_END_DECLS
