project(
    'IceCream-tests',
    'cpp',
    meson_version: '>= 0.40.0',
    license: 'MIT',
)


test_cpp11 = executable(
    'test_cpp11',
    'test.cpp',
    cpp_args: '-DCPP_11',
    override_options: ['cpp_std=c++11'],
)
test('test_cpp11', test_cpp11)


test_cpp14 = executable(
    'test_cpp14',
    'test.cpp',
    cpp_args: '-DCPP_14',
    override_options: ['cpp_std=c++14'],
)
test('test_cpp14', test_cpp14)


test_cpp17 = executable(
    'test_cpp17',
    'test.cpp',
    cpp_args: '-DCPP_17',
    override_options: ['cpp_std=c++17'],
)
test('test_cpp17', test_cpp17)
