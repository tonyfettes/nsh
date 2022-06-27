subdir('execute')
subdir('parse')

foreach name : [
  'stack',
  'string',
  'hash',
  'alias',
]
  test(name,
    executable(name,
      files(name + '.c'),
      dependencies: nsh['core']),
    suite: ['unit test'])
endforeach
