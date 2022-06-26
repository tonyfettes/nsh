subdir('execute')
subdir('parse')

nsh_test = [
  'stack',
  'string',
  'hash',
  'alias',
]

foreach name : nsh_test
  test(name,
    executable(name,
      files(name + '.c'),
      dependencies: nsh_dep),
    suite: ['unit test'])
endforeach
