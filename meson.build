project('pktx', 'cpp')
cc = meson.get_compiler('cpp')
dependencies = [
    dependency('ezr'),
    cc.find_library('ktx')
]
incs = [
    'include/',
]
if get_option('use_custom_ktx')
    dependencies += cc.find_library('ktx', dirs: get_option('ktx_path') / 'lib')
    incs += get_option('ktx_path') / 'include'
else
    dependencies += cc.find_library('ktx')
endif


sources = [
    'src/texture.cpp'
]
lib = library('pktx', sources, include_directories: incs, dependencies: dependencies)
pktx_dep = declare_dependency(include_directories: incs, link_with: lib, dependencies:dependencies)