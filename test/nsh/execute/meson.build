nsh_execute_test = [
  'simple',
]

foreach name : nsh_execute_test
  test('execute / ' + name,
    executable(name,
      files(name + '.c'),
      dependencies: nsh_dep),
    suite: ['unit test'])
endforeach
