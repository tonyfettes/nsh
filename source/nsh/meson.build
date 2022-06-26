subdir('parse')
subdir('execute')

nsh_src = files([
  # libc alternative / wrapper
  'error.c',
  'memory.c',
  'assert.c',
  # containers
  'stack.c',
  'string.c',
  'hash.c',
  # core type definition
  'alias.c',
  'block.c',
  'command.c',
  'compound.c',
  'context.c',
  'environment.c',
  'parameter.c',
  'segment.c',
  'word.c',
  'alias.c',
  'function.c',
  'keyword.c',
  'redirect.c',
  'simple.c',
  'pipeline.c',
  'statement.c',
  'loop.c',
  # parse
  'parse.c',
]) + nsh_parse_src + nsh_execute_src

nsh_dep = declare_dependency(
  link_with: library('nsh',
    nsh_src,
    include_directories: include),
  include_directories: include)
