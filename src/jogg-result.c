/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (C) 2024 Ernestas Kulik <ernestas AT baltic DOT engineering>
 */

#include "jogg-enum-types.h"
#include "jogg-result.h"

struct _JoggResult
{
    GObject parent_instance;

    GDesktopAppInfo *app_info;
    char *action;
    JoggMatchType match_type;
    bool prefix_match;
};

enum { PROP_APP_INFO = 1
     , PROP_ACTION
     , PROP_MATCH_TYPE
     , PROP_PREFIX_MATCH
     , PROP_HIDDEN
     , PROP_LABEL
     , N_PROPERTIES
     };

static GParamSpec *properties[N_PROPERTIES];

G_DEFINE_TYPE (JoggResult, jogg_result, G_TYPE_OBJECT);

char *
jogg_result_get_action_name ( GObject    *object
                            , JoggResult *self)
{
    if (NULL == self)
    {
        return NULL;
    }
    if (NULL == self->action)
    {
        return NULL;
    }

    return g_desktop_app_info_get_action_name (self->app_info, self->action);
}

GIcon *
jogg_result_get_icon ( GObject    *object
                     , JoggResult *self)
{
    GIcon *icon = NULL;

    if (NULL == self)
    {
        return NULL;
    }

    icon = g_app_info_get_icon (G_APP_INFO (self->app_info));
    if (NULL == icon)
    {
        icon = g_icon_new_for_string ("application-x-executable", NULL);
    }
    else
    {
        icon = g_object_ref (icon);
    }

    return icon;
}

gboolean
jogg_result_is_action_visible ( GObject    *object
                              , JoggResult *self)
{
    JoggMatchType match_type;

    if (NULL == self)
    {
        return FALSE;
    }

    match_type = jogg_result_get_match_type (self);

    return match_type == JOGG_MATCH_TYPE_ACTIONS;
}

static void
jogg_result_init (JoggResult *self)
{
    (void) self;
}

static void
jogg_result_finalize (GObject *object)
{
    JoggResult *self = NULL;

    self = JOGG_RESULT (object);

    g_free (g_steal_pointer (&self->action));
}

static void
jogg_result_get_property ( GObject    *object
                         , guint       property_id
                         , GValue     *value
                         , GParamSpec *pspec)
{
    JoggResult *self = NULL;
    gboolean nodisplay = false;
    const char *name = NULL;

    self = JOGG_RESULT (object);

    switch (property_id)
    {
    case PROP_APP_INFO:
        g_value_set_object (value, self->app_info);
        break;
    case PROP_ACTION:
        g_value_set_string (value, self->action);
        break;
    case PROP_MATCH_TYPE:
        g_value_set_enum (value, self->match_type);
        break;
    case PROP_PREFIX_MATCH:
        g_value_set_boolean (value, self->prefix_match);
        break;
    case PROP_HIDDEN:
        nodisplay = g_desktop_app_info_get_nodisplay (self->app_info);

        g_value_set_boolean (value, nodisplay);
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
    case PROP_ACTION:
        self->action = g_value_dup_string (value);
        break;
    case PROP_MATCH_TYPE:
        self->match_type = g_value_get_enum (value);
        break;
    case PROP_PREFIX_MATCH:
        self->prefix_match = g_value_get_boolean (value);
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
    properties[PROP_ACTION] = g_param_spec_string ( "action"
                                                  , NULL
                                                  , NULL
                                                  , NULL
                                                  , ( G_PARAM_READWRITE
                                                    | G_PARAM_CONSTRUCT_ONLY
                                                    | G_PARAM_STATIC_STRINGS
                                                    )
                                                  );
    properties[PROP_MATCH_TYPE] = g_param_spec_enum ( "match-type"
                                                    , NULL
                                                    , NULL
                                                    , JOGG_TYPE_MATCH_TYPE
                                                    , JOGG_MATCH_TYPE_INVALID
                                                    , ( G_PARAM_READWRITE
                                                      | G_PARAM_CONSTRUCT_ONLY
                                                      | G_PARAM_STATIC_STRINGS
                                                      )
                                                    );
    properties[PROP_PREFIX_MATCH] = g_param_spec_boolean ( "prefix-match"
                                                         , NULL
                                                         , NULL
                                                         , FALSE
                                                         , ( G_PARAM_READWRITE
                                                           | G_PARAM_CONSTRUCT_ONLY
                                                           | G_PARAM_STATIC_STRINGS
                                                           )
                                                         );
    properties[PROP_HIDDEN] = g_param_spec_boolean ( "hidden"
                                                   , NULL
                                                   , NULL
                                                   , FALSE
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

    object_class->finalize = jogg_result_finalize;
    object_class->get_property = jogg_result_get_property;
    object_class->set_property = jogg_result_set_property;

    g_object_class_install_properties ( object_class
                                      , G_N_ELEMENTS (properties)
                                      , properties
                                      );
}

char *
jogg_result_get_action (JoggResult *self)
{
    return g_strdup (self->action);
}

GDesktopAppInfo *
jogg_result_get_app_info (JoggResult *self)
{
    g_return_val_if_fail (JOGG_IS_RESULT (self), NULL);

    return g_object_ref (self->app_info);
}

bool
jogg_result_is_prefix_match (JoggResult *self)
{
    g_return_val_if_fail (JOGG_IS_RESULT (self), false);

    return self->prefix_match;
}

JoggMatchType
jogg_result_get_match_type (JoggResult *self)
{
    g_return_val_if_fail (JOGG_IS_RESULT (self), JOGG_MATCH_TYPE_INVALID);

    return self->match_type;
}

GtkOrdering
jogg_result_compare ( const JoggResult *lhs
                    , const JoggResult *rhs)
{
    if (lhs->match_type < rhs->match_type)
    {
        return GTK_ORDERING_SMALLER;
    }
    else if (lhs->match_type > rhs->match_type)
    {
        return GTK_ORDERING_LARGER;
    }

    return GTK_ORDERING_EQUAL;
}

JoggResult *
jogg_result_new ( GDesktopAppInfo *app_info
                , const char      *action
                , JoggMatchType    match_type
                , bool             prefix_match)
{
    g_return_val_if_fail (app_info != NULL, NULL);

    return g_object_new ( JOGG_TYPE_RESULT
                        , "action", action
                        , "app-info", app_info
                        , "match-type", match_type
                        , "prefix-match", prefix_match
                        , NULL
                        );
}
