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

G_BEGIN_DECLS

typedef struct _JoggApplication JoggApplication;
typedef struct _JoggApplicationWindow JoggApplicationWindow;
typedef struct _JoggResult JoggResult;

G_END_DECLS
