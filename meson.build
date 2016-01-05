project('cpp-bitstring', 'cpp')
add_global_arguments('-std=c++11', language : 'cpp')
add_global_arguments('-Wall', language : 'cpp')
add_global_arguments('-Wextra', language : 'cpp')

#subdir('tests')

cppunittest_dep = dependency('cppunit')

src_dir = include_directories('src')

cppbitstring = shared_library(
    'cpp-bitstring', 'src/Bits.cpp', 'src/Utils.cpp',
    include_directories: src_dir,
    install: false,
    version: '0.0.1',
    soversion: '0'
)

pkgconfig_gen(
    libraries : cppbitstring,
    version : '0.1',
    name : 'libcpp-bitstring',
    filebase : 'cpp-bitstring',
    description : ''
)

unittests = executable(
    'unittests', 'tests/main.cpp', 'tests/BitsTests.cpp',
    dependencies: cppunittest_dep,
    link_with : cppbitstring
)
test('cpp-bitstring test', unittests)
