foreach name : ['simple']
  test('execute / ' + name,
    executable(name,
      files(name + '.c'),
      dependencies: nsh['core']),
    suite: ['unit test'])
endforeach
