/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (C) 2024 Ernestas Kulik <ernestas AT baltic DOT engineering>
 */

#include "jogg-result.h"
#include "jogg-result-widget.h"

struct _JoggResultWidget
{
    GtkBox parent_instance;

    JoggResult *result;

    GtkWidget *icon;
    GtkWidget *name_description_box;
    GtkWidget *name;
    GtkWidget *action_box;
    GtkWidget *action;
    GtkWidget *description;
};

G_DEFINE_TYPE (JoggResultWidget, jogg_result_widget, GTK_TYPE_BOX);

static void
jogg_result_widget_init (JoggResultWidget *self)
{
    gtk_widget_init_template (GTK_WIDGET (self));
}

static void
jogg_result_widget_class_init (JoggResultWidgetClass *klass)
{
    GtkWidgetClass *widget_class = NULL;

    widget_class = GTK_WIDGET_CLASS (klass);

    gtk_widget_class_set_template_from_resource ( widget_class
                                                , "/baltic/engineering/jogg/ui/result.ui"
                                                );

    gtk_widget_class_bind_template_child ( widget_class
                                         , JoggResultWidget
                                         , icon
                                         );
    gtk_widget_class_bind_template_child ( widget_class
                                         , JoggResultWidget
                                         , name_description_box
                                         );
    gtk_widget_class_bind_template_child ( widget_class
                                         , JoggResultWidget
                                         , name
                                         );
    gtk_widget_class_bind_template_child ( widget_class
                                         , JoggResultWidget
                                         , action_box
                                         );
    gtk_widget_class_bind_template_child ( widget_class
                                         , JoggResultWidget
                                         , action
                                         );
    gtk_widget_class_bind_template_child ( widget_class
                                         , JoggResultWidget
                                         , description
                                         );
}

void
jogg_result_widget_set_result ( JoggResultWidget *self
                              , JoggResult       *result
                              )
{
    GDesktopAppInfo *app_info = NULL;
    GIcon *icon = NULL;
    const char *name = NULL;
    g_autofree char *action = NULL;
    g_autofree char *action_name = NULL;
    const char *description = NULL;

    g_return_if_fail (JOGG_IS_RESULT_WIDGET (self));
    g_return_if_fail (JOGG_IS_RESULT (result));

    app_info = jogg_result_get_app_info (result);
    icon = g_app_info_get_icon (G_APP_INFO (app_info));
    if (NULL == icon)
    {
        icon = g_icon_new_for_string ("application-x-executable", NULL);
    }
    else
    {
        icon = g_object_ref (icon);
    }
    name = g_app_info_get_name (G_APP_INFO (app_info));
    action = jogg_result_get_action (result);
    if (action != NULL)
    {
        action_name = g_desktop_app_info_get_action_name (app_info, action);
    }
    description = g_app_info_get_description (G_APP_INFO (app_info));

    gtk_image_set_from_gicon (GTK_IMAGE (self->icon), icon);
    gtk_label_set_label (GTK_LABEL (self->name), name);
    gtk_label_set_label (GTK_LABEL (self->action), action_name);
    gtk_label_set_label (GTK_LABEL (self->description), description);

    gtk_widget_set_visible (self->action_box, action != NULL);
    gtk_widget_set_visible (self->description, description != NULL);
}

GtkWidget *
jogg_result_widget_new (void)
{
    return g_object_new (JOGG_TYPE_RESULT_WIDGET, NULL);
}
