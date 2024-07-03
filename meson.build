project('jogg', 'c',
  default_options: [
    'gtk4-layer-shell:smoke-tests=false',
    'gtk4-layer-shell:introspection=false',
    'gtk4-layer-shell:vapi=false',
  ],
  license: 'GPL-2.0-only',
  meson_version: '>= 0.55.0',
  version: '1.0',
)

gio_unix = dependency('gio-unix-2.0')
glib = dependency('glib-2.0')
gtk4 = dependency('gtk4')
gtk4_layer_shell = dependency('gtk4-layer-shell',
  version: '>= 1.0.0',
)

subdir('src')
