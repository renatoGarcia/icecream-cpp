# IceCream-Cpp example project

This is a "Hello World" project, showing how to integrate Icecream-cpp using CMake, and
optionally also [Nix](https://nixos.org) or [Conan](https://conan.io).

Note that in this examples Icecream-cpp will be installed in the system. If you chose to
include the header file on your project, all you will need is `#include` it on your source
and compile the project as usual.

All the next instructions consider that you are with an open shell inside the
example_project directory.

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

## Building on CMake only

Before entering the next instructions, a working system wide Icecream-cpp installation is
presumed. To get that, either install it by a distribution package, or follow the
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
