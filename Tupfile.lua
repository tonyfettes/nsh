local sources = tup.glob[[src/*.c]]
local compiler = 'gcc'
local flags = {
  '-std=c11',
  '-Wall',
  '-Wextra',
  '-g',
  '-O3',
}
local executable = 'build/nsh'

local function compile (args)
  -- args = {
  --   compiler = 'g++',
  --   flags = { '-g', ... },
  --   inputs = { 'example.cpp', ... },
  --   output = 'example.o'
  -- }
  local command = {
    args.compiler,
    table.concat(args.flags, ' '),
    '-c',
    table.concat(args.inputs, ' '),
    '-o',
    args.output,
  }
  tup.definerule {
    inputs = args.inputs,
    command = table.concat(command, ' ');
    outputs = {args.output},
  }
end

local function link (args)
  -- args = {
  --   linker = 'g++',
  --   flags = { '-pg', ... },
  --   inputs = { 'example.o', ... },
  --   output = 'example'
  -- }
  local command = {
    args.linker,
    table.concat(args.flags, ' '),
    table.concat(args.inputs, ' '),
    '-o',
    args.output,
  }
  tup.definerule {
    inputs = args.inputs,
    command = table.concat(command, ' ');
    outputs = {args.output},
  }
end

local objects = {}
for _, source in ipairs(sources) do
  local object = 'build/' .. tup.base(source) .. '.o'
  table.insert(objects, object)
  compile {
    compiler = compiler,
    flags = flags,
    inputs = {source},
    output = object
  }
end
link {
  linker = compiler,
  flags = flags,
  inputs = objects,
  output = executable
}
