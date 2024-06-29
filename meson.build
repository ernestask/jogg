project('jogg', 'c',
  license: 'GPL-2.0-only',
  version: '1.0',
)

gio_unix = dependency('gio-unix-2.0')
glib = dependency('glib-2.0')
gtk4 = dependency('gtk4')
gtk4_layer_shell = subproject('gtk4-layer-shell',
  default_options: [
    'smoke-tests=false',
    'introspection=false',
    'vapi=false',
  ],
)

subdir('src')
