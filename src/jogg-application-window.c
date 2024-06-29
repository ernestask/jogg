/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (C) 2024 Ernestas Kulik <ernestas AT baltic DOT engineering>
 */

#include "jogg-application.h"
#include "jogg-application-window.h"
#include "jogg-result.h"

#include <gio/gdesktopappinfo.h>

struct _JoggApplicationWindow
{
    GtkApplicationWindow parent_instance;

    GtkSingleSelection *model;
    GtkFilterListModel *filter_model;
    GListStore *applications;
    GtkCustomFilter *filter;
    GtkWidget *revealer;
    GtkWidget *revealer_box;
    GtkWidget *search_entry;
    GtkWidget *results_revealer;
    GtkWidget *results_scrolled_window;
    GtkWidget *results;
};

G_DEFINE_TYPE ( JoggApplicationWindow
              , jogg_application_window
              , GTK_TYPE_APPLICATION_WINDOW
              );

static gboolean
jogg_application_window_transform_n_items (GBinding     *binding,
                                           const GValue *from_value,
                                           GValue       *to_value,
                                           gpointer      user_data)
{
    guint n_items;

    n_items = g_value_get_uint (from_value);

    g_value_set_boolean (to_value, n_items > 0);

    return TRUE;
}

static void
jogg_application_window_results_revealer_on_notify_reveal_child (GObject    *self,
                                                                   GParamSpec *pspec,
                                                                   gpointer    user_data)
{
    JoggApplicationWindow *window;

    window = user_data;

    (void) pspec;

    gtk_widget_add_css_class (window->revealer_box, "linked");
    gtk_widget_add_css_class (window->results_scrolled_window, "jogg");
}

static void
jogg_application_window_results_revealer_on_notify_child_revealed (GObject    *self,
                                                                   GParamSpec *pspec,
                                                                   gpointer    user_data)
{
    JoggApplicationWindow *window;

    window = user_data;

    (void) pspec;

    if (!gtk_revealer_get_child_revealed (GTK_REVEALER (self)))
    {
        gtk_widget_remove_css_class (window->revealer_box, "linked");
        gtk_widget_remove_css_class (window->results_scrolled_window, "jogg");
    }
}

static void
jogg_application_window_search_entry_on_activate ( GtkSearchEntry *self
                                                 , gpointer user_data
                                                 )
{
    JoggApplicationWindow *window = NULL;
    GtkBitset *selection = NULL;
    guint position;

    window = user_data;
    selection = gtk_selection_model_get_selection (GTK_SELECTION_MODEL (window->model));
    position = gtk_bitset_get_minimum (selection);

    if (position == G_MAXUINT)
    {
        return;
    }

    gtk_widget_activate_action (window->results
                               , "list.activate-item"
                               , "u"
                               , position);
}

static void
jogg_application_window_search_entry_on_search_changed (GtkSearchEntry *self,
                                                        gpointer        user_data)
{
    JoggApplicationWindow *window = NULL;
    const char *text = NULL;
    g_autofree GStrv *app_infos = NULL;
    g_autoptr (GPtrArray) desktop_app_infos = NULL;
    size_t n;

    window = JOGG_APPLICATION_WINDOW (user_data);
    text = gtk_editable_get_text (GTK_EDITABLE (self));
    app_infos = g_desktop_app_info_search (text);
    desktop_app_infos = g_ptr_array_new_full (64, g_object_unref);
    n = g_list_model_get_n_items (G_LIST_MODEL (window->applications));

    g_list_store_splice (window->applications, 0, n, NULL, 0);

    for (size_t i = 0; app_infos[i] != NULL; i++)
    {
        for (size_t j = 0; app_infos[i][j] != NULL; j++)
        {
            GDesktopAppInfo *app_info = NULL;
            JoggResult *result = NULL;

            app_info = g_desktop_app_info_new (app_infos[i][j]);
            if (app_info == NULL)
            {
                continue;
            }
            result = jogg_result_new (app_info);

            g_ptr_array_add (desktop_app_infos, result);
        }

        g_strfreev (app_infos[i]);
    }

    g_list_store_splice (window->applications, 0, 0,
                         desktop_app_infos->pdata,
                         desktop_app_infos->len);
}

static void
jogg_application_window_revealer_on_child_revealed (GObject    *self,
                                                    GParamSpec *pspec,
                                                    gpointer    user_data)
{
    (void) pspec;

    if (!gtk_revealer_get_child_revealed (GTK_REVEALER (self)))
    {
        g_application_quit (G_APPLICATION (user_data));
    }
}

static void
jogg_application_window_quit (JoggApplicationWindow *self)
{
    GtkApplication *application;

    application = gtk_window_get_application (GTK_WINDOW (self));

    /* Immediately hide results to avoid the search bar revealer making things awkward. */
    gtk_revealer_set_transition_type (GTK_REVEALER (self->results_revealer),
                                      GTK_REVEALER_TRANSITION_TYPE_NONE);
    gtk_revealer_set_reveal_child (GTK_REVEALER (self->results_revealer), FALSE);

    /* Quit application only after the pretty revealer animation finishes. */
    g_signal_connect (self->revealer,
                      "notify::child-revealed",
                      G_CALLBACK (jogg_application_window_revealer_on_child_revealed),
                      application);
    gtk_revealer_set_reveal_child (GTK_REVEALER (self->revealer), FALSE);
}

static void
jogg_application_window_search_entry_on_stop_search ( GtkSearchEntry *self
                                                    , gpointer        user_data)
{
    jogg_application_window_quit (JOGG_APPLICATION_WINDOW (user_data));
}

static void
jogg_application_window_results_on_activate ( GtkListView *self
                                            , guint        position
                                            , gpointer     user_data)
{
    GtkSelectionModel *model = NULL;
    JoggResult *item = NULL;
    g_autoptr (GDesktopAppInfo) app_info = NULL;

    model = gtk_list_view_get_model (self);
    item = g_list_model_get_item (G_LIST_MODEL (model), position);
    app_info = jogg_result_get_app_info (JOGG_RESULT (item));

    g_app_info_launch (G_APP_INFO (app_info), NULL, NULL, NULL);
    jogg_application_window_quit (user_data);
}

static gboolean
jogg_application_window_app_info_filter_func ( gpointer item
                                             , gpointer user_data)
{
    g_autoptr (GDesktopAppInfo) app_info = NULL;

    app_info = jogg_result_get_app_info (item);

    return !g_desktop_app_info_get_nodisplay (app_info);
}

static gboolean
jogg_application_window_search_entry_on_key_pressed ( GtkEventControllerKey *self
                                                    , guint                  keyval
                                                    , guint                  keycode
                                                    , GdkModifierType        state
                                                    , gpointer               user_data
                                                    )
{
    JoggApplicationWindow *window = NULL;
    GtkBitset *selection = NULL;
    guint position;

    switch (keyval)
    {
    case GDK_KEY_Up:
    case GDK_KEY_Down:
    case GDK_KEY_KP_Up:
    case GDK_KEY_KP_Down:
        break;
    default:
        return GDK_EVENT_PROPAGATE;
    }

    window = user_data;
    selection = gtk_selection_model_get_selection (GTK_SELECTION_MODEL (window->model));
    position = gtk_bitset_get_minimum (selection);

    if (position == G_MAXUINT)
    {
        return GDK_EVENT_STOP;
    }

    switch (keyval)
    {
    case GDK_KEY_Up:
    case GDK_KEY_KP_Up:
        if (position > 0)
        {
            position--;
        }

        break;
    case GDK_KEY_Down:
    case GDK_KEY_KP_Down:
        if (position < G_MAXUINT)
        {
            position++;
        }

        break;
    default:
        g_assert_not_reached ();
    }

    gtk_list_view_scroll_to ( GTK_LIST_VIEW (window->results)
                            , position
                            , GTK_LIST_SCROLL_SELECT
                            , NULL
                            );

    return GDK_EVENT_STOP;
}

static void
jogg_application_window_init (JoggApplicationWindow *self)
{
    GtkEventController *controller = NULL;

    gtk_widget_init_template (GTK_WIDGET (self));

    g_object_bind_property_full ( self->model
                                , "n-items"
                                , self->results_revealer
                                , "reveal-child"
                                , G_BINDING_DEFAULT
                                , jogg_application_window_transform_n_items
                                , NULL
                                , NULL
                                , NULL
                                );

    g_signal_connect ( self->search_entry
                     , "activate"
                     , G_CALLBACK (jogg_application_window_search_entry_on_activate)
                     , self
                     );
#if 0
    g_signal_connect ( self->search_entry
                     , "next-match"
                     , G_CALLBACK (jogg_application_window_search_entry_on_next_match)
                     , self
                     );
#endif
    g_signal_connect ( self->search_entry
                     , "search-changed"
                     , G_CALLBACK (jogg_application_window_search_entry_on_search_changed)
                     , self
                     );
    g_signal_connect ( self->search_entry
                     , "stop-search"
                     , G_CALLBACK (jogg_application_window_search_entry_on_stop_search)
                     , self
                     );
    g_signal_connect ( self->results_revealer
                     , "notify::reveal-child"
                     , G_CALLBACK (jogg_application_window_results_revealer_on_notify_reveal_child)
                     , self
                     );
    g_signal_connect ( self->results_revealer
                     , "notify::child-revealed"
                     , G_CALLBACK (jogg_application_window_results_revealer_on_notify_child_revealed)
                     , self
                     );
    g_signal_connect ( self->results
                     , "activate"
                     , G_CALLBACK (jogg_application_window_results_on_activate)
                     , self
                     );

    gtk_search_entry_set_key_capture_widget ( GTK_SEARCH_ENTRY (self->search_entry)
                                            , self->results
                                            );
    gtk_custom_filter_set_filter_func ( self->filter
                                      , jogg_application_window_app_info_filter_func
                                      , NULL
                                      , NULL
                                      );

    controller = gtk_event_controller_key_new ();

    g_signal_connect ( controller
                     , "key-pressed"
                     , G_CALLBACK (jogg_application_window_search_entry_on_key_pressed)
                     , self
                     );
    gtk_widget_add_controller ( self->search_entry
                              , controller
                              );
}

static void
jogg_application_window_map (GtkWidget *self)
{
    JoggApplicationWindow *window;

    window = JOGG_APPLICATION_WINDOW (self);

    GTK_WIDGET_CLASS (jogg_application_window_parent_class)->map (self);
    gtk_revealer_set_reveal_child (GTK_REVEALER (window->revealer), TRUE);
}

static gboolean
jogg_application_window_on_escape_pressed (GtkWidget *widget,
                                           GVariant  *args,
                                           gpointer   user_data)
{
    jogg_application_window_quit (JOGG_APPLICATION_WINDOW (widget));

    return TRUE;
}

static void
jogg_application_window_class_init (JoggApplicationWindowClass *klass)
{
    GtkWidgetClass *widget_class = NULL;

    widget_class = GTK_WIDGET_CLASS (klass);

    widget_class->map = jogg_application_window_map;

    g_type_ensure (JOGG_TYPE_RESULT);
    gtk_widget_class_set_template_from_resource ( widget_class
                                                , "/baltic/engineering/jogg/ui/window.ui"
                                                );

    gtk_widget_class_add_binding ( widget_class
                                 , GDK_KEY_Escape
                                 , GDK_NO_MODIFIER_MASK
                                 , jogg_application_window_on_escape_pressed
                                 , NULL
                                 );

    gtk_widget_class_bind_template_child ( widget_class
                                         , JoggApplicationWindow
                                         , model
                                         );
    gtk_widget_class_bind_template_child (widget_class,
                                          JoggApplicationWindow,
                                          filter_model);
    gtk_widget_class_bind_template_child (widget_class,
                                          JoggApplicationWindow,
                                          applications);
    gtk_widget_class_bind_template_child (widget_class,
                                          JoggApplicationWindow,
                                          filter);
    gtk_widget_class_bind_template_child (widget_class,
                                          JoggApplicationWindow,
                                          revealer);
    gtk_widget_class_bind_template_child (widget_class,
                                          JoggApplicationWindow,
                                          revealer_box);
    gtk_widget_class_bind_template_child (widget_class,
                                          JoggApplicationWindow,
                                          search_entry);
    gtk_widget_class_bind_template_child (widget_class,
                                          JoggApplicationWindow,
                                          results_revealer);
    gtk_widget_class_bind_template_child (widget_class,
                                          JoggApplicationWindow,
                                          results_scrolled_window);
    gtk_widget_class_bind_template_child (widget_class,
                                          JoggApplicationWindow,
                                          results);

    gtk_widget_class_bind_template_callback (widget_class,
                                             jogg_application_window_search_entry_on_search_changed);
    gtk_widget_class_bind_template_callback (widget_class,
                                             jogg_application_window_search_entry_on_stop_search);
}

JoggApplicationWindow *
jogg_application_window_new (JoggApplication *app)
{
      return g_object_new (JOGG_TYPE_APPLICATION_WINDOW,
                           "application", app,
                           NULL);
}
