foreach name : [
  'parameter',
  'word',
  'redirect',
  'component',
  'simple',
  'command',
  'keyword',
]
  test('parse / posix / ' + name,
    executable(name,
      files(['common.c', name + '.c']),
      dependencies: [nsh['core'], nsh['parse']['posix']]),
    suite: ['unit test'])
endforeach
