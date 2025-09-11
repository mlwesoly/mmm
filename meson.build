project(
  'mmm',
  ['c'],
  version: '0.0.1',
  meson_version : '>= 0.50.0',
  default_options: [
    'warning_level=2',
    ]
  )

# Setup compiler.
cc = meson.get_compiler('c')
extra_flags = [
  '-fno-common',
  '-fstack-protector-strong',
  '-Wformat-security',
  '-Wformat-overflow=3',
  '-Wshadow',
  '-Wstrict-overflow=5'
# '-Wconversion'
  ]

public_headers = include_directories('src','/usr/local/dislin')

#host_sys = host_machine.system()
#platform_is_windows = (host_sys == 'windows')

add_project_arguments(cc.get_supported_arguments(extra_flags), language: 'c')

conf = configuration_data()
conf.set_quoted('MMM_VERSION', meson.project_version())
config_h = configure_file(output : 'config.h', configuration : conf)

geany_dep = dependency('geany')
# dislin_dep = dependency('dislin')


dislin_dep = declare_dependency(
    link_args : ['-L' + '/usr/local/dislin', '-l' + 'dislin'],
    include_directories : include_directories('/usr/local/dislin'))

m_dep = cc.find_library('m', required : true)

shared_library(
  meson.project_name(),
  'src/mmm.c',
  'src/matrixreader.c',
  'src/print.c',
  'src/tinyexpr.c',
  'src/converter.c',
  include_directories : public_headers,
  dependencies: [geany_dep, m_dep, dislin_dep],
  install: true,
  name_prefix: '',
  #install_dir : join_paths(get_option('prefix'), get_option('libdir'), 'geany')
  install_dir : join_paths('tools/geanyplugins')
  )
