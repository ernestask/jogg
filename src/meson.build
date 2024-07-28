gnome = import('gnome')

jogg_sources = [
  gnome.compile_resources(
    'jogg-resources',
    join_paths('res', 'jogg.gresource.xml'),
    source_dir: 'res',
    c_name: 'jogg'
  ),
  gnome.mkenums(
    'jogg-enum-types',
    c_template: 'jogg-enum-types.c.template',
    h_template: 'jogg-enum-types.h.template',
    sources: [
      'jogg-enums.h',
    ],
  ),
  'jogg-application.c',
  'jogg-application.h',
  'jogg-application-window.c',
  'jogg-application-window.h',
  'jogg-enums.h',
  'jogg-result.c',
  'jogg-result.h',
  'jogg-result-widget.c',
  'jogg-result-widget.h',
  'jogg-types.h',
  'jogg-utils.c',
  'jogg-utils.h',
  'main.c',
]

executable('jogg',
  jogg_sources,
  dependencies: [
    gio_unix,
    glib,
    gmodule_export,
    gtk4_layer_shell,
    gtk4,
  ],
  export_dynamic: true,
  install: true,
)
