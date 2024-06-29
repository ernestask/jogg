/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (C) 2024 Ernestas Kulik <ernestas AT baltic DOT engineering>
 */

#pragma once

#include "jogg-types.h"

#include <gio/gio.h>
#include <gio/gdesktopappinfo.h>

G_BEGIN_DECLS

G_DECLARE_FINAL_TYPE ( JoggResult
                     , jogg_result
                     , JOGG
                     , RESULT
                     , GObject
                     );

GDesktopAppInfo *jogg_result_get_app_info (JoggResult *);

JoggResult *jogg_result_new (GDesktopAppInfo *);

G_END_DECLS
