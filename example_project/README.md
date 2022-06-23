# IceCream-Cpp example project

This is a "Hello World" project, showing how to integrate IceCream-Cpp using CMake and
optionally also [Nix](https://nixos.org).

Note that this example applies if IceCream-Cpp is installed system wide. If you chose to
include the header file on your project, you will need only `#include` it on your source
and compile the project as usual.

From now, a working system wide IceCream-Cpp installation is assumed. To achieve that,
follow the instructions at
[here](https://github.com/renatoGarcia/icecream-cpp/tree/master#install).

## Building on CMake only

With an open shell inside the example_project directory, run the commands:

```shell
mkdir build
cd build
cmake ..
cmake --build .
```


## Building with Nix flake

With an open shell inside the example_project directory, run the commands:

```shell
nix build
```
