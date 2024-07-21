/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (C) 2024 Ernestas Kulik <ernestas AT baltic DOT engineering>
 */

#pragma once

#include "jogg-types.h"

#include <gtk/gtk.h>

G_BEGIN_DECLS

G_DECLARE_FINAL_TYPE (JoggApplication, jogg_application,
                      JOGG, APPLICATION,
                      GtkApplication);

GPtrArray *jogg_application_app_info_search ( JoggApplication *self
                                            , const char      *query);

JoggApplication *jogg_application_new (void);

G_END_DECLS
