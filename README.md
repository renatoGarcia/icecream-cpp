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

will print something like:

    ic| i0: 7, s0: bla, 3.14: 3.14

### Printing function

Except by some especial types described bellow, an overload of the `operator<<(ostream&,
T)` function must exist to all printed types `T`. If an overload of `operator<<` could not
be resolved at `IC(...)` usage, the compilation will fail.

#### Pointer like types

When printing a pointer or pointer like type (`std::unique_ptr`, `boost::shared_ptr`,
etc), the pointed value will be printed alongside the pointer value (the memory address).
A null pointer won't be dereferenced, however a dangling or invalid pointer will
potentially to cause a segmentation fault.

A code like:

```C++
auto v0 = std::make_shared<int>(7);
float* v1 = nullptr;
IC(v0, v1);
```

will print something like:

    ic| v0: 7 at 0x55587b6f5410, v1: nullptr

To disable this functionality and to print only the pointer value, set the
`show_pointed_value` option to `false`:

```C++
icecream::ic.show_pointed_value(false)
```

Having set the option to `false`, the same code above would print:

    ic| v0: 0x55587b6f5410, v1: 0

#### Optional type

A `std::optional` variable will print its value, if it has one, or *nullopt* otherwise.

The code:

```C++
auto v0 = std::optional<int> {10};
auto v1 = std::optional<int> {};
IC(v0, v1);
```

will print:

    ic| v0: 10, v1: nullopt

Any possible overload resolution of `operator<<(ostream&, T)` will take precedence and
will be used instead. The code:

```C++
auto operator<<(std::ostream& os, std::optional<std::string> const& value) -> std::ostream&
{
    if (value.has_value())
        std::cout << "Has string " << *value;
    else
        std::cout << "No string";

    return os;
}
auto v2 = std::optional<std::string> {"bla"};
IC(v2);
```

will print:

    ic| v2: Has string bla


#### Iterable types

If for an instance `a` of a type `A`, all operations below are valid:

```C++
auto it = begin(a);
it != end(a);
++it;
*it;
```

the type `A` is defined *iterable* and if no suitable overload resolution to
`operator<<(ostream&, A)` is found, all of its items will be printed instead. The code:

```C++
auto v0 = std::list<int> {10, 20, 30};
IC(v0);
```

will print:

    ic| v0: [10, 20, 30]

If there are any suitable overload resolution to `operator<<(ostream&, A)`, it will take
precedence and used instead. The code:

```C++
template<typename T>
auto operator<<(std::ostream& os, std::vector<T> const& value) -> std::ostream&
{
    std::cout << "Vector - { ";
    auto c = int {0};
    for (auto& i : value)
    {
        std::cout << "(" << c << ":" << i << ") ";
        ++c;
    }
    std::cout << "}";
    return os;
}

auto v1 = std::vector<char> {'a', 'b', 'c'};
IC(v1);
```

will print:

    ic| v1: Vector - { (0:a) (1:b) (2:c) }

## Pitfalls

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

### Similar projects

The [CleanType](https://github.com/pthom/cleantype) library has a focus on printing
readable types names, but there is support to print variables names and values alongside
its types. An optional integration of CleanType with IceCream-Cpp, if the first is present
on system, is being planed. With that would be possible to show the types of values within
`IC(...)` macro.

### Ongoing work

This library is at an early version, and it was tested on GCC 8.2 compiling
with C++11, C++14 and C++17 flags. Some minor code changes could be needed to make it work with other
compilers and older versions of GCC. Please report any success or problems.

Work must be done adding customization options to the Icecream class, and mimicking the
relevant functionalities of the original Python IceCream library.

Any help is welcome :-)
