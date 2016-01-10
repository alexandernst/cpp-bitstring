project('cpp-bitstring', 'cpp', default_options: ['warnlevel=2', 'cpp_std=c++11'])

#subdir('tests')

cppunittest_dep = dependency('cppunit')

src_dir = include_directories('src')

cppbitstring = shared_library(
	'cpp-bitstring', 'src/Bits.cpp', 'src/Utils.cpp',
	include_directories: src_dir,
	version: '0.0.1',
	soversion: '0'
)

cppbitstringstatic = static_library(
	'cpp-bitstring-static', 'src/Bits.cpp', 'src/Utils.cpp',
	include_directories: src_dir,
	cpp_args : '-fPIC'
)

unittests = executable(
	'unittests', 'tests/main.cpp', 'tests/BitsTests.cpp',
	dependencies: cppunittest_dep,
	link_with : cppbitstringstatic
)
test('cpp-bitstring test', unittests)
