subdir('execute')

nsh = {
  'core': declare_dependency(
    link_with: library('nsh_core',
      files([
        # libc alternative / wrapper
        'error.c',
        'memory.c',
        'assert.c',
        'log.c',
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
      ]) + nsh_execute_src,
      include_directories: include),
    include_directories: include),
}

subdir('parse')
