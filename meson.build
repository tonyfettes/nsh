project('nsh', 'c',
  default_options: [
    'warning_level=3',
    'c_std=c11',
    'b_sanitize=address',
  ])

add_project_arguments('-D_POSIX_C_SOURCE=200809L', language: 'c')

include = include_directories('include')

subdir('source')
subdir('test')
