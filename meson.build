project('pktx', 'cpp')
cc = meson.get_compiler('cpp')
dependencies = [
    dependency('ezr'),
    cc.find_library('ktx')
]
incs = include_directories([
    'include/'
])
sources = [
    'src/texture.cpp'
]
lib = library('pktx', sources, include_directories: incs, dependencies: dependencies)
pktx_dep = declare_dependency(include_directories: incs, link_with: lib, dependencies:dependencies)