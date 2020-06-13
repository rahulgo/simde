simde_test_arm_neon_tests = [
  'abs',
  'add',
  'addl',
  'addl_high',
  'addw',
  'and',
  'bsl',
  'cagt',
  'ceq',
  'ceqz',
  'cltz',
  'dup_n',
  'eor',
  'get_high',
  'get_lane',
  'get_low',
  'max',
  'min',
  'mla',
  'mlal',
  'mlal_high',
  'movl',
  'movl_high',
  'mul',
  'mull',
  'orr',
  'padd',
  'paddl',
  'pmax',
  'pmin',
  'reinterpret',
  'shl',
  'shl_n',
  'shr_n',
  'sra_n',
  'sub',
  'trn',
  'trn1',
  'trn2',
  'uzp',
  'uzp1',
  'uzp2',
  'zip',
  'zip1',
  'zip2',
]

simde_test_arm_neon_sources = []
simde_test_arm_neon_libs = []
foreach name : simde_test_arm_neon_tests
  c_source = name + '.c'
  cpp_source = configure_file(input: name + '.c', output: name + '.cpp', copy: true)
  simde_test_arm_neon_sources += [c_source, cpp_source]

  test('arm/neon/' + name + '/emul/c', executable(name + '-emul-c', c_source,
      c_args: simde_c_args + simde_c_defs + simde_native_c_flags + ['-DSIMDE_NO_NATIVE', '-DSIMDE_TEST_BARE'],
      include_directories: simde_include_dir,
      dependencies: libm),
    protocol: 'tap')
  test('arm/neon/' + name + '/emul/cpp', executable(name + '-emul-cpp', cpp_source,
      cpp_args: simde_c_args + simde_c_defs + simde_native_c_flags + ['-DSIMDE_NO_NATIVE', '-DSIMDE_TEST_BARE'],
      include_directories: simde_include_dir,
      dependencies: libm),
    protocol: 'tap')
  test('arm/neon/' + name + '/native/c', executable(name + '-native-c', c_source,
      c_args: simde_c_args + simde_c_defs + simde_native_c_flags + ['-DSIMDE_TEST_BARE'],
      include_directories: simde_include_dir,
      dependencies: libm),
    protocol: 'tap')
  test('arm/neon/' + name + '/native/cpp', executable(name + '-native-cpp', cpp_source,
      cpp_args: simde_c_args + simde_c_defs + simde_native_c_flags + ['-DSIMDE_TEST_BARE'],
      include_directories: simde_include_dir,
      dependencies: libm),
    protocol: 'tap')
endforeach
