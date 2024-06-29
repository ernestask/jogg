/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (C) 2024 Ernestas Kulik <ernestas AT baltic DOT engineering>
 */

#include "jogg-application.h"

int
main (int    argc,
      char **argv)
{
    g_autoptr (JoggApplication) application = NULL;

    application = jogg_application_new ();

    return g_application_run (G_APPLICATION (application), argc, argv);
}
