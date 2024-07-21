/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (C) 2024 Ernestas Kulik <ernestas AT baltic DOT engineering>
 */

#include "jogg-application.h"
#include "jogg-application-window.h"
#include "jogg-result.h"
#include "jogg-utils.h"

#include <gio/gdesktopappinfo.h>
#include <gtk4-layer-shell.h>

struct _JoggApplication
{
    GtkApplication parent_instance;

    GList *applications;
};

G_DEFINE_TYPE (JoggApplication, jogg_application, GTK_TYPE_APPLICATION);

static void
jogg_application_init (JoggApplication *self)
{
    self->applications = g_app_info_get_all ();
}

static void
jogg_application_window_on_realize (GtkWidget *self,
                                    gpointer   user_data)
{
    GdkDisplay *display = NULL;
    GdkSurface *surface = NULL;
    GdkMonitor *monitor = NULL;
    GdkRectangle geometry = { 0 };

    display = gtk_widget_get_display (self);
    surface = gtk_native_get_surface (GTK_NATIVE (self));
    monitor = gdk_display_get_monitor_at_surface (display, surface);

    gdk_monitor_get_geometry (monitor, &geometry);

    gtk_layer_set_margin (GTK_WINDOW (self),
                          GTK_LAYER_SHELL_EDGE_TOP,
                          geometry.height / 3);
}

static void
jogg_application_finalize (GObject *object)
{
    JoggApplication *self = NULL;

    self = JOGG_APPLICATION (object);

    g_list_free_full (g_steal_pointer (&self->applications), g_object_unref);
}

static void
jogg_application_activate (GApplication *app)
{
    JoggApplicationWindow *window = NULL;
    GtkWindow *gtk_window = NULL;
    GtkCssProvider *provider = NULL;

    window = jogg_application_window_new (JOGG_APPLICATION (app));
    gtk_window = GTK_WINDOW (window);
    provider = gtk_css_provider_new ();

    g_signal_connect (window,
                      "realize",
                      G_CALLBACK (jogg_application_window_on_realize),
                      NULL);

    gtk_layer_init_for_window (gtk_window);

    gtk_layer_set_anchor (gtk_window, GTK_LAYER_SHELL_EDGE_TOP, TRUE);
    gtk_layer_set_anchor (gtk_window, GTK_LAYER_SHELL_EDGE_BOTTOM, TRUE);
    gtk_layer_set_keyboard_mode (gtk_window, GTK_LAYER_SHELL_KEYBOARD_MODE_EXCLUSIVE);
    gtk_layer_set_layer (gtk_window, GTK_LAYER_SHELL_LAYER_TOP);

    gtk_css_provider_load_from_resource (provider,
                                         "/baltic/engineering/jogg/styles.css");
    gtk_style_context_add_provider_for_display (gdk_display_get_default (),
                                                GTK_STYLE_PROVIDER (provider),
                                                GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    gtk_window_present (GTK_WINDOW (window));
}

static void
jogg_application_class_init (JoggApplicationClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    GApplicationClass *app_class = G_APPLICATION_CLASS (klass);

    object_class->finalize = jogg_application_finalize;
    app_class->activate = jogg_application_activate;
}

GPtrArray *
jogg_application_app_info_search ( JoggApplication *self
                                 , const char      *query)
{
    GPtrArray *results = NULL;

    g_return_val_if_fail (JOGG_IS_APPLICATION (self), NULL);
    g_return_val_if_fail (query != NULL, NULL);

    results = g_ptr_array_new_full (64, g_object_unref);

    if (*query == '\0')
    {
        return results;
    }

    for (GList *l = self->applications; l != NULL; l = g_list_next (l))
    {
        GDesktopAppInfo *app_info = NULL;
        const char *haystack = NULL;
        bool prefix = false;
        const char *const *haystacks = NULL;
        JoggMatchType match_type = JOGG_MATCH_TYPE_INVALID;
        JoggResult *result = NULL;

        app_info = G_DESKTOP_APP_INFO (l->data);
        haystack = g_app_info_get_name (G_APP_INFO (app_info));
        if (jogg_has_substring (haystack, query, &prefix))
        {
            match_type = JOGG_MATCH_TYPE_NAME;

            goto match_found;
        }
        haystack = g_app_info_get_display_name (G_APP_INFO (app_info));
        if (jogg_has_substring (haystack, query, &prefix))
        {
            match_type = JOGG_MATCH_TYPE_GENERIC_NAME;

            goto match_found;
        }
        haystacks = g_desktop_app_info_get_keywords (app_info);
        for (; haystacks != NULL && *haystacks != NULL; haystacks++)
        {
            haystack = *haystacks;

            if (jogg_has_substring (haystack, query, &prefix))
            {
                match_type = JOGG_MATCH_TYPE_KEYWORDS;

                goto match_found;
            }
        }
        haystack = g_app_info_get_executable (G_APP_INFO (app_info));
        if (jogg_has_substring (haystack, query, &prefix))
        {
            match_type = JOGG_MATCH_TYPE_EXEC;
        }

match_found:
        if (match_type != JOGG_MATCH_TYPE_INVALID)
        {
            result = jogg_result_new (app_info, NULL, match_type, prefix);

            g_ptr_array_add (results, result);

            continue;
        }

        haystacks = g_desktop_app_info_list_actions (app_info);
        for (; haystacks != NULL && *haystacks != NULL; haystacks++)
        {
            g_autofree char *action_name = NULL;

            haystack = *haystacks;
            action_name = g_desktop_app_info_get_action_name (app_info, haystack);

            if (jogg_has_substring (action_name, query, &prefix))
            {
                result = jogg_result_new ( app_info
                                         , haystack
                                         , JOGG_MATCH_TYPE_ACTIONS
                                         , prefix
                                         );

                g_ptr_array_add (results, result);
            }
        }
    }

    return results;
}

JoggApplication *
jogg_application_new (void)
{
      return g_object_new (JOGG_TYPE_APPLICATION,
                           "application-id", "engineering.baltic.jogg",
                           "flags", G_APPLICATION_DEFAULT_FLAGS,
                           NULL);
}
