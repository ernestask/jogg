/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (C) 2024 Ernestas Kulik <ernestas AT baltic DOT engineering>
 */

#include "jogg-application.h"
#include "jogg-application-window.h"

#include <gtk4-layer-shell.h>

struct _JoggApplication
{
    GtkApplication parent_instance;
};

G_DEFINE_TYPE (JoggApplication, jogg_application, GTK_TYPE_APPLICATION);

static void
jogg_application_init (JoggApplication *self)
{
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
    GApplicationClass *app_class = G_APPLICATION_CLASS (klass);

    app_class->activate = jogg_application_activate;
}

JoggApplication *
jogg_application_new (void)
{
      return g_object_new (JOGG_TYPE_APPLICATION,
                           "application-id", "engineering.baltic.jogg",
                           "flags", G_APPLICATION_DEFAULT_FLAGS,
                           NULL);
}
