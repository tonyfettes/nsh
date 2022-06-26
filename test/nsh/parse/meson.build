nsh_parse_test = [
  'parameter',
  'word',
  'redirect',
  'component',
  'simple',
  'command',
  'keyword',
]

foreach name : nsh_parse_test
  test('parse / ' + name,
    executable(name,
      files(['common.c', name + '.c']),
      dependencies: nsh_dep),
    suite: ['unit test'])
endforeach
