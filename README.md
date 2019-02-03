# IceCream-Cpp

IceCream-Cpp is a little library to help with the print debugging on C++11 and forward.

This library is inspired by and aims to behave the most identical as possible to the
original Python [IceCream](https://github.com/gruns/icecream) library.

## Install

The IceCream-Cpp is a header only library, having the STL as its only dependency. To
install it just copy the `icecream.hpp` header anywhere the compiler can find it.

## Usage

After including the header on source `test.cpp`:

```C++
#include <icecream.hpp>
```

A macro `IC(...)` will be defined. If called with no arguments it will print the prefix
(default `ic| `), the source file name, the current line number, and the current function
signature. The code:

```C++
auto my_function(int foo, double bar) -> void
{
    // ...
    IC();
    // ...
}
```

Will print something like:

    ic| test.cpp:34 in "void my_function(int, double)"

If called with arguments, it will print the prefix, those arguments names, and its values.
The code:

```C++
auto i0 = int {7};
auto s0 = std::string {"bla"};
IC(i0, s0, 3.14);
```

Will print something like:

    ic| i0: 7, s0: bla, 3.14: 3.14

### Pitfalls

The `IC(...)` is a preprocessor macro, then care must be taken when using arguments with
commas. Any argument having commas must be enclosed by parenthesis. The code:

```C++
auto sum(int i0, int i1) -> int
{
    return i0 + i1;
}

// ...
IC((sum(40, 2)));
// ...
```

will work and print something like:

    ic| (sum(40, 2)): 42

As `IC(...)` is a preprocessor macro, it can cause conflicts if there is some other `IC`
identifier on code. To change the `IC(...)` macro to a longer `ICECREAM(...)` one, just
define `ICECREAM_LONG_NAME` before the inclusion of `icecream.hpp` header:

```C++
#define ICECREAM_LONG_NAME
#include "icecream.hpp"
```

Also, because of the preprocessor metaprogramming machinery, there are a maximum limit of
16 arguments on `IC(...)` macro. That limit can be extended, but it will always be a
finite integer number.

### Ongoind work

This library is at a very early version, and I have only tested it on GCC 8.2 compiling
with C++11, C++14 and C++17 flags. Some code could be needed to make it work with other
compilers and older versions of GCC.

Work must be done adding customization options to the Icecream class, and mimicking the
relevant functionalities of the original Python IceCream library.

Any help is welcome :-)
