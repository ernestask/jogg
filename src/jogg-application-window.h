/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (C) 2024 Ernestas Kulik <ernestas AT baltic DOT engineering>
 */

#pragma once

#include "jogg-types.h"

#include <gtk/gtk.h>

G_BEGIN_DECLS

G_DECLARE_FINAL_TYPE (JoggApplicationWindow, jogg_application_window,
                      JOGG, APPLICATION_WINDOW,
                      GtkApplicationWindow);

JoggApplicationWindow *jogg_application_window_new (JoggApplication *);

G_END_DECLS
