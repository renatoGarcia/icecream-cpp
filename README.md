# IceCream-Cpp

IceCream-Cpp is a little library to help with the print debugging on C++11 and forward.

**Contents**
* [Install](#install)
* [Usage](#usage)
  * [Configuration](#configuration)
     * [stream](#stream)
     * [prefix](#prefix)
     * [show_c_string](#show_c_string)
     * [lineWrapWidth](#linewrapwidth)
  * [Printing logic](#printing-logic)
     * [C strings](#c-strings)
     * [Pointer like types](#pointer-like-types)
     * [Optional types](#optional-types)
     * [Pair types](#pair-types)
     * [Iterable types](#iterable-types)
* [Pitfalls](#pitfalls)
* [Similar projects](#similar-projects)

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

Also, any variable inspection like:

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

After including the `icecream.hpp` header on a source file, assuming `test.cpp` for this
example:

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

    ic| v0: [1, 2, 3], s0: "bla", 3.14: 3.14

### Configuration

The `Icecream` class is internally implemented as a singleton. All the configuration
changes will be done on to unique object, and shared across all the program and threads.

All configurations are done/viewed through accessor methods, using the `icecream::ic`
object, and to enable the method chaining idiom all the set methods return a reference of
the `ic` object:

```C++
icecream::ic
    .prefix("ic: ")
    .show_c_string(false)
    .lineWrapWidth(70);
```

To simplify the code, on examples below a `using icecream::ic;` statement will be
presumed.

#### stream

The `std::ostream` where the output will be streamed.

- get:
    ```C++
    ic.stream()
    ```

The default stream buffer associated is the same as `std::cerr`, but that can be changed.
For instance, to stream the output to a string:

```C++
auto sstr = std::stringstream {};
ic.stream().rdbuf(sstr.rdbuf());
```

#### prefix

The prefix is the text that is printed before each output. It can be set to a string, a
nullary callable that returns an object that has an overload of `operator<<(ostream&, T)`,
or any number of instances of those two. The printed prefix will be a concatenation of all
elements.

- set:
    ```C++
    ic.prefix("icecream| ");
    ic.prefix([]{return 42;}, ": ");
    ic.prefix("thread ", std::this_thread::get_id, " | ");
    ```

With those prefixes the code:
```C++
IC();
```

will print respectively:

```
icecream| test.cpp:34 in "void my_function(int, double)"

42: test.cpp:34 in "void my_function(int, double)"

thread 1 | test.cpp:34 in "void my_function(int, double)"
```

#### show_c_string

Controls if a `char*` variable should be interpreted as a null-terminated C string
(`true`) or a pointer to `char` (`false`). The default value is `true`.

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

The maximum number of characters before the output be broken on multiple lines. Default
value of `70`.

- get:
    ```C++
    ic.lineWrapWidth();
    ```
- set:
    ```C++
    ic.lineWrapWidth(35);
    ```

### Printing logic

The guiding principle to print a type `T` is to use the function `operator<<(ostream&, T)`
always when an overload is available. The exceptions to that rule are strings (C and
`std::string`), `char` and bounded arrays. Strings are enclosed by `"`, `char` are
enclosed by `'`, and arrays are considered iterables rather than decay to raw pointers.

In general if an overload of `operator<<(ostream&, T)` is not available to a type `T` a
call to `IC(t)` will result on a compilation error. Exceptions to that rule, when
IceCream-Cpp will print a type `T` even without a `operator<<` overload are discussed
below. Note however that if a user implements a custom `operator<<(ostream&, T)` that will
take precedence and used instead.

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

#### Pointer like types

The `std::unique_ptr<T>` (before C++20) and `boost::scoped_ptr<T>` types will be printed
like usual raw pointers.

The `std::weak_ptr<T>` and `boost::weak_ptr<T>` types will print their address if they are
valid or "expired" otherwise. The code:

```C++
auto v0 = std::make_shared<int>(7);
auto v1 = std::weak_ptr<int> {v0};

IC(v1);
v0.reset();
IC(v1);
```

will print:

```
ic| v1: 0x55bcbd840ec0
ic| v1: expired
```

#### Optional types

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

#### Pair types

A `std::pair<T1, T2>` typed variable will print both its values.

The code:

```C++
auto v0 = std::make_pair(10, 3.14);
IC(v0);
```

will print:

    ic| v0: (10, 3.14)

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
