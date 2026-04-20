# Icecream-cpp example project

This is a "Hello World" project showing how to integrate Icecream-cpp using CMake, and
optionally [Nix](https://nixos.org) or [Conan](https://conan.io).

Note that in these examples Icecream-cpp will be installed in the system. If you choose to
copy the header file to your project instead, all you need to do is `#include` it in your
source and compile the project as usual.

All instructions below assume you are in a shell opened inside the "example_project"
directory.

## Building it with Conan

This will use the Icecream-cpp recipe in [Conan
Center](https://conan.io/center/recipes/icecream-cpp), at the same version as stated in
[conanfile.txt](conanfile.txt)

```shell
conan profile detect --force
conan install . --output-folder=build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug
cmake --build .
```

## Building with CMake only

Before following these instructions, a working system-wide Icecream-cpp installation is
required. To do so, either install it from a distribution package or follow the
instructions [here](https://github.com/renatoGarcia/icecream-cpp/tree/master#install).


```shell
mkdir build
cd build
cmake ..
cmake --build .
```

## Building with Nix flake

```shell
nix build
```
