nsh_parse += {
  'posix': declare_dependency(
    link_with: library('nsh_parse_posix',
    files([
      'common.c',
      'name.c',
      'operator.c',
      'delimiter.c',
      'parameter.c',
      'segment.c',
      'word.c',
      'redirect.c',
      'component.c',
      'simple.c',
      'pipeline.c',
      'statement.c',
      'block.c',
      'compound.c',
      'loop.c',
      'command.c',
    ]),
    dependencies: nsh['core'],
    include_directories: include),
  include_directories: include),
}
