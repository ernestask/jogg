/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (C) 2024 Ernestas Kulik <ernestas AT baltic DOT engineering>
 */

#pragma once

#include <glib.h>

#define JOGG_TYPE_APPLICATION jogg_application_get_type ()
#define JOGG_TYPE_APPLICATION_WINDOW jogg_application_window_get_type ()
#define JOGG_TYPE_RESULT jogg_result_get_type ()
#define JOGG_TYPE_RESULT_WIDGET jogg_result_widget_get_type ()

G_BEGIN_DECLS

typedef struct _JoggApplication JoggApplication;
typedef struct _JoggApplicationWindow JoggApplicationWindow;
typedef struct _JoggResult JoggResult;
typedef struct _JoggResultWidget JoggResultWidget;

G_END_DECLS
