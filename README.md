# IceCream-Cpp

IceCream-Cpp is a little library to help with the print debugging on C++11 and forward.

With IceCream-Cpp, an execution inspection:

```c++
auto my_function(int i, double d) -> void
{
    std::cout << "1" << std::endl;
    if (condition)
        std::cout << "2" << std::endl;
    else
        std::cout << "3" << std::endl;
}
```

can be coded instead:

```c++
auto my_function(int i, double d) -> void
{
    IC();
    if (condition)
        IC();
    else
        IC();
}
```

and will print something like:

    ic| test.cpp:34 in "void my_function(int, double)"
    ic| test.cpp:36 in "void my_function(int, double)"

And any variable inspection like:

```c++
std::cout << "a: " << a
          << ", b: " << b
          << ", sum(a, b): " << sum(a, b)
          << std::endl;
```

can be simplified to:

```c++
IC(a, b, (sum(a, b)));
```

that will print:

    ic| a: 7, b: 2, (sum(a, b)): 9

This library is inspired by and aims to behave the most identical as possible to the
original [Python IceCream](https://github.com/gruns/icecream) library.

## Install

The IceCream-Cpp is a one file, header only library, having the STL as its only
dependency. To install it just copy the `icecream.hpp` header anywhere the compiler can
find it.

## Usage

After including the `icecream.hpp` header on a source file, `test.cpp` for this example:

```C++
#include "icecream.hpp"
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

will print:

    ic| test.cpp:34 in "void my_function(int, double)"

If called with arguments, it will print the prefix, those arguments names, and its values.
The code:

```C++
auto v0 = std::vector<int> {1, 2, 3};
auto s0 = std::string {"bla"};
IC(v0, s0, 3.14);
```

will print:

    ic| v0: [1, 2, 3], s0: bla, 3.14: 3.14

### Configuration

#### show_c_string

Controls if a `char*` variable should be interpreted as a null-terminated C string or a
pointer to `char`. The default value is `true`.

- get:
    ```C++
    ic.show_c_string();
    ```
- set:
    ```C++
    ic.show_c_string(false);
    ```

the code:

```C++
char const* flavor = "mango";

ic.show_c_string(true);
IC(flavor);

ic.show_c_string(false);
IC(flavor);
```

will print:

```
ic| flavor: "mango";
ic| flavor: 0x55587b6f5410
```

#### lineWrapWidth

fds

- get:
    ```C++
    ic.lineWrapWidth();
    ```
- set:
    ```C++
    ic.lineWrapWidth(35);
    ```

### Printing logic

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

To disable this functionality and print only the pointer value, just set the
`show_pointed_value` option to `false`:

```C++
icecream::ic.show_pointed_value(false)
```

Having set the option to `false`, the same code above would print:

    ic| v0: 0x55587b6f5410, v1: 0


#### C strings

C strings are ambiguous. Should a `char* foo` variable be interpreted as a pointer to a
single `char` or as a null-terminated string? Likewise, is the `char bar[]` variable an
array of single characters or a null-terminated string? Is `char baz[3]` an array with
three single characters or a string of size three?

Each one of those interpretations of `foo`, `bar`, and `baz` would be printed in a
distinct way. To the code:

```C++
char flavor[] = "pistachio";
IC(flavor);
```

all three outputs below are correct, each one having a distinct interpretation of what is
the `flavor` variable.

```
ic| flavor: 0x55587b6f5410
ic| flavor: ['p', 'i', 's', 't', 'a', 'c', 'h', 'i', 'o', '\0']
ic| flavor: "pistachio"
```

The IceCream-Cpp adopted policy is to handle any bounded `char` array (i.e.: array with a
known size) as an array of single characters. So the code:

```C++
char flavor[] = "chocolate";
IC(flavor);
```

will print:

```
ic| flavor: ['c', 'h', 'o', 'c', 'o', 'l', 'a', 't', 'e', '\0']
```

unbounded `char[]` arrays (i.e.: array with an unknown size) will decay to `char*`
pointers, and will be printed either as a string or a pointer as configured by the
[show_c_string](#show_c_string) option.


#### Optional type

A `std::optional<T>` typed variable will print its value, if it has one, or *nullopt*
otherwise.

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
        os << "Has string " << *value;
    else
        os << "No string";
    return os;
}

// ...

auto v2 = std::optional<std::string> {"bla"};
IC(v2);
```

will print:

    ic| v2: Has string bla

#### Pair type

A `std::pair<T1, T2>` typed variable will print both its values.

The code:

```C++
auto v0 = std::make_pair(10, 3.14);
IC(v0);
```

will print:

    ic| v0: (10, 3.14)

As with other default print methods, any possible overload resolution of
`operator<<(ostream&, T)` will take precedence and will be used instead.

#### Iterable types

If for a type `A` with an instance `a`, all operations below are valid:

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

If there is any suitable overload resolution to `operator<<(ostream&, A)`, it will take
precedence and used instead. The code:

```C++
template <typename T>
auto operator<<(std::ostream& os, std::vector<T> const& value) -> std::ostream&
{
    os << "Vector - { ";
    for (auto& i : value)
    {
        os << "(" << i << ") ";
    }
    os << "}";
    return os;
}

auto v1 = std::vector<char> {'a', 'b', 'c'};
IC(v1);
```

will print:

    ic| v1: Vector - { (a) (b) (c) }

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
```

will work and print something like:

    ic| (sum(40, 2)): 42

Also, since `IC(...)` is a preprocessor macro, it can cause conflicts if there is some
other `IC` identifier on code. To change the `IC(...)` macro to a longer `ICECREAM(...)`
one, just define `ICECREAM_LONG_NAME` before the inclusion of `icecream.hpp` header:

```C++
#define ICECREAM_LONG_NAME
#include "icecream.hpp"
```

## Similar projects

The [CleanType](https://github.com/pthom/cleantype) library has a focus on printing
readable types names, but there is support to print variables names and values alongside
its types. An optional integration of CleanType with IceCream-Cpp, if the first is present
on system, is being planed. With that would be possible to show the types of values within
`IC(...)` macro.

## Ongoing work

This library is at an early version, and it was tested on GCC 8.2 compiling
with C++11, C++14 and C++17 flags. Some minor code changes could be needed to make it work with other
compilers and older versions of GCC. Please report any success or problems.

Work must be done adding customization options to the Icecream class, and mimicking the
relevant functionalities of the original Python IceCream library.

Any help is welcome :-)
