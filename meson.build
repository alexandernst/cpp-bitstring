project('cpp-bitstring', 'cpp')
add_global_arguments('-std=c++11', language : 'cpp')

#subdir('tests')

crypto_dep = dependency('libcrypto')
cppunittest_dep = dependency('cppunit')

cppbitstring = shared_library(
    'cpp-bitstring', 'Bits.cpp', 'Utils.cpp',
    dependencies: crypto_dep,
    install: false
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
