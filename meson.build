project(
  'openvk-purple',
  'c',
  version: '0.1.0',
  license: 'GPL-2.0-or-later',
  default_options: [
    'c_std=gnu11',
    'warning_level=2',
  ],
)

glib_dep = dependency('glib-2.0', version: '>=2.28')
purple_dep = dependency('purple', version: '>=2.14', required: false)
if not purple_dep.found()
  purple_dep = dependency('libpurple', version: '>=2.14')
endif

purple_plugindir = purple_dep.get_variable(
  pkgconfig: 'plugindir',
  default: get_option('libdir') / 'purple-2',
)

plugin_dir = get_option('plugin_dir')
if plugin_dir == ''
  plugin_dir = purple_plugindir
endif

sources = files(
  'src/plugin.c',
  'src/openvk.c',
  'src/prpl.c',
)

openvk = shared_module(
  'openvk',
  sources,
  name_prefix: '',
  dependencies: [glib_dep, purple_dep],
  c_args: [
    '-DPURPLE_PLUGINS',
    '-DGLIB_VERSION_MAX_ALLOWED=GLIB_VERSION_2_28',
    '-DGLIB_VERSION_MIN_REQUIRED=GLIB_VERSION_2_28',
  ],
  install: true,
  install_dir: plugin_dir,
)

protocol_icon_dir = get_option('datadir') / 'pixmaps/pidgin/protocols'
foreach size : ['16', '22', '48']
  install_data(
    'data/openvk.png',
    install_dir: protocol_icon_dir / size,
  )
endforeach
install_data(
  'data/openvk.svg',
  install_dir: protocol_icon_dir / '16/scalable',
)

summary({
  'Plugin directory': plugin_dir,
  'libpurple': purple_dep.version(),
  'GLib': glib_dep.version(),
}, section: 'OpenVK')
