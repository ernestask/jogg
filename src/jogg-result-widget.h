/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (C) 2024 Ernestas Kulik <ernestas AT baltic DOT engineering>
 */

#pragma once

#include "jogg-types.h"

#include <gtk/gtk.h>

G_BEGIN_DECLS

G_DECLARE_FINAL_TYPE ( JoggResultWidget
                     , jogg_result_widget
                     , JOGG
                     , RESULT_WIDGET
                     , GtkBox
                     );

void jogg_result_widget_set_result     ( JoggResultWidget *self
                                       , JoggResult       *result
                                       );

GtkWidget *jogg_result_widget_new (void);

G_END_DECLS
