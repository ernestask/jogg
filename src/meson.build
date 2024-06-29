gnome = import('gnome')

jogg_sources = [
  gnome.compile_resources(
    'jogg-resources',
    join_paths('res', 'jogg.gresource.xml'),
    source_dir: 'res',
    c_name: 'jogg'
  ),
  'jogg-application.c',
  'jogg-application.h',
  'jogg-application-window.c',
  'jogg-application-window.h',
  'jogg-result.c',
  'jogg-result.h',
  'jogg-types.h',
  'main.c',
]

executable('jogg',
  jogg_sources,
  dependencies: [
    gio_unix,
    glib,
    gtk4_layer_shell.get_variable('gtk_layer_shell'),
    gtk4,
  ],
  install : true,
)
