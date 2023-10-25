project('finder', 'cpp', version: '0.0.1')

inc = 'include'
src = meson.source_root() / 'src'
include = include_directories(inc)

p_name = meson.project_name()
p_version = meson.project_version()

headers = [inc / 'finder.hpp']
sources = [src / 'finder.cpp']

shared = shared_library(
  p_name, 
  version: p_version,
  include_directories: include,
  install: true,
  dependencies: [dependency('wildest-card')],
  sources: sources
  )

static = static_library(
  p_name, 
  include_directories: include,
  install: true,
  dependencies: [dependency('wildest-card', static: true)],
  sources: sources
  )

install_headers(headers, subdir: p_name)

pkg = import('pkgconfig')
pkg.generate(
  name: p_name,
  version: p_version,
  description: 'An easy-to-use c++ lib for finding files in the system',
  subdirs: p_name,
  libraries: [shared, static]
  )

subdir('test')
subdir('example')
