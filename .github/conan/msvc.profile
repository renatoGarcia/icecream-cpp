# Explicit MSVC profile. `conan profile detect` picks the gcc that is on the windows-2025
# runner's PATH instead of MSVC, so the compiler is spelled out here.
[settings]
os=Windows
arch=x86_64
build_type=Release
compiler=msvc
compiler.version=194
compiler.runtime=dynamic
