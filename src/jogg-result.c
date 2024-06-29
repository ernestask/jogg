/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (C) 2024 Ernestas Kulik <ernestas AT baltic DOT engineering>
 */

#include "jogg-result.h"

struct _JoggResult
{
    GObject parent_instance;

    GDesktopAppInfo *app_info;
};

enum
{
    PROP_APP_INFO = 1,
    PROP_ICON,
    PROP_LABEL,
    N_PROPERTIES,
};

static GParamSpec *properties[N_PROPERTIES];

G_DEFINE_TYPE (JoggResult, jogg_result, G_TYPE_OBJECT);

static void
jogg_result_init (JoggResult *self)
{
    (void) self;
}

static void
jogg_result_get_property ( GObject    *object
                         , guint       property_id
                         , GValue     *value
                         , GParamSpec *pspec)
{
    JoggResult *self = NULL;
    GIcon *icon = NULL;
    const char *name = NULL;

    self = JOGG_RESULT (object);

    switch (property_id)
    {
    case PROP_APP_INFO:
        g_value_set_object (value, self->app_info);
        break;
    case PROP_ICON:
        icon = g_app_info_get_icon (G_APP_INFO (self->app_info));

        g_value_set_object (value, icon);
        break;
    case PROP_LABEL:
        name = g_app_info_get_name (G_APP_INFO (self->app_info));

        g_value_set_string (value, name);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
jogg_result_set_property ( GObject      *object
                         , guint         property_id
                         , const GValue *value
                         , GParamSpec   *pspec)
{
    JoggResult *self = NULL;

    self = JOGG_RESULT (object);

    switch (property_id)
    {
    case PROP_APP_INFO:
        self->app_info = g_value_get_object (value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
jogg_result_class_init (JoggResultClass *klass)
{
    GObjectClass *object_class = NULL;

    object_class = G_OBJECT_CLASS (klass);

    properties[PROP_APP_INFO] = g_param_spec_object ( "app-info"
                                                    , NULL
                                                    , NULL
                                                    , G_TYPE_DESKTOP_APP_INFO
                                                    , ( G_PARAM_READWRITE
                                                      | G_PARAM_CONSTRUCT_ONLY
                                                      | G_PARAM_STATIC_STRINGS
                                                      )
                                                    );
    properties[PROP_ICON] = g_param_spec_object ( "icon"
                                                , NULL
                                                , NULL
                                                , G_TYPE_ICON
                                                , ( G_PARAM_READABLE
                                                  | G_PARAM_STATIC_STRINGS
                                                  )
                                                );
    properties[PROP_LABEL] = g_param_spec_string ( "label"
                                                 , NULL
                                                 , NULL
                                                 , NULL
                                                 , ( G_PARAM_READABLE
                                                   | G_PARAM_STATIC_STRINGS
                                                   )
                                                 );

    object_class->get_property = jogg_result_get_property;
    object_class->set_property = jogg_result_set_property;

    g_object_class_install_properties ( object_class
                                      , G_N_ELEMENTS (properties)
                                      , properties
                                      );
}

GDesktopAppInfo *
jogg_result_get_app_info (JoggResult *self)
{
    g_return_val_if_fail (JOGG_IS_RESULT (self), NULL);

    return g_object_ref (self->app_info);
}

JoggResult *
jogg_result_new (GDesktopAppInfo *app_info)
{
    g_return_val_if_fail (app_info != NULL, NULL);

    return g_object_new ( JOGG_TYPE_RESULT
                        , "app-info", app_info
                        , NULL
                        );
}
