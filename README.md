# IceCream-Cpp

[![CI.badge]][CI.page]

IceCream-Cpp is a little (single header) library to help with the print debugging on C++11 and forward.

**Contents**
* [Install](#install)
* [Usage](#usage)
  * [Return value](#return-value)
  * [Configuration](#configuration)
     * [enable/disable](#enabledisable)
     * [stream](#stream)
     * [prefix](#prefix)
     * [show_c_string](#show_c_string)
     * [line_wrap_width](#line_wrap_width)
     * [include_context](#include_context)
     * [context_delimiter](#context_delimiter)
  * [Printing logic](#printing-logic)
     * [C strings](#c-strings)
     * [Pointer like types](#pointer-like-types)
     * [Iterable types](#iterable-types)
     * [Tuple like types](#tuple-like-types)
     * [Optional types](#optional-types)
     * [Variant types](#variant-types)
     * [Exception types](#exception-types)
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

After including the `icecream.hpp` header on a source file, assumed `test.cpp` for this
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

If called with arguments it will print the prefix, those arguments names, and its values.
The code:

```C++
auto v0 = std::vector<int> {1, 2, 3};
auto s0 = std::string {"bla"};
IC(v0, s0, 3.14);
```

will print:

    ic| v0: [1, 2, 3], s0: "bla", 3.14: 3.14


### Return value

If called with no arguments the `IC(...)` macro will return `void`, if called with one argument it will return the argument itself, and if called with multiple arguments it will return a tuple with all of them:


```C++
auto a = int {7};
auto b = std::string {"bla"};
auto c = float {3.14};

IC();
int& d = IC(a);
std::tuple<std::string&, float&> e = IC(b, c);
```

### Configuration

The `Icecream` class is internally implemented as a singleton. All the configuration
changes will be done to an unique object, and shared across all the program and threads.

All configurations are done/viewed through accessor methods, using the `icecream::ic`
object. To allow the method chaining idiom all the set methods return a reference of the
`ic` object:

```C++
icecream::ic
    .prefix("ic: ")
    .show_c_string(false)
    .line_wrap_width(70);
```

To simplify the code, on examples below an `using icecream::ic;` statement will be
presumed.

#### enable/disable

Enable or disable the output of `IC(...)` macro, *enabled* default.

- set:
    ```C++
    auto enable() -> IcecreamAPI&;
    auto disable() -> IcecreamAPI&;
    ```

The code:

```C++
IC(1);
ic.disable();
IC(2);
ic.enable();
IC(3);
```

will print:

```
ic| 1: 1
ic| 3: 3
```

#### stream

Warning: this method will return a reference to the internal `std::ostream`. The
operations done on that reference will not be thread safe.

The `std::ostream` where the output will be streamed.

- get:
    ```C++
    auto stream() -> std::ostream&;
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
those elements.

- set:
    ```C++
    template <typename... Ts>
    auto prefix(Ts&& ...values) -> IcecreamAPI&;
    ```

The code:
```C++
ic.prefix("icecream| ");
IC(1);
ic.prefix([]{return 42;}, "- ");
IC(2);
ic.prefix("thread ", std::this_thread::get_id, " | ");
IC(3);
```

will print:

```
icecream| 1: 1
42- 2: 2
thread 1 | 3: 3
```

#### show_c_string

Controls if a `char*` variable should be interpreted as a null-terminated C string
(`true`) or a pointer to `char` (`false`). The default value is `true`.

- get:
    ```C++
    auto show_c_string() const -> bool;
    ```
- set:
    ```C++
    auto show_c_string(bool value) -> IcecreamAPI&;
    ```

The code:

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

#### line_wrap_width

The maximum number of characters before the output be broken on multiple lines. Default
value of `70`.

- get:
    ```C++
    auto line_wrap_width() const -> std::size_t;
    ```
- set:
    ```C++
    auto line_wrap_width(std::size_t value) -> IcecreamAPI&;
    ```

#### include_context

If the context (source name, line number, and function name) should be printed even when
printing variables. Default value is `false`.

- get:
    ```C++
    auto include_context() const -> bool;
    ```
- set:
    ```C++
    auto include_context(bool value) -> IcecreamAPI&;
    ```

#### context_delimiter

The string separating the context text from the variables values. Default value is `"- "`.

- get:
    ```C++
    auto context_delimiter() const -> std::string;
    ```
- set:
    ```C++
    auto context_delimiter(std::string const& value) -> IcecreamAPI&;
    ```

### Printing logic

The preference to print a type `T` is to use the overloaded function `operator<<(ostream&,
T)` always when it is available. The exceptions to that rule are strings (C strings,
`std::string`, and `std::string_view`), `char` and bounded arrays. Strings will be
enclosed by `"`, `char` will be enclosed by `'`, and arrays are considered iterables
rather than let decay to raw pointers.

In general, if an overload of `operator<<(ostream&, T)` is not available to a type `T`, a
call to `IC(t)` will result on a compiling error. All exceptions to that rule, when
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

#### Iterable types

If for a type `A` with an instance `a`, all operations below are valid:

```C++
auto it = begin(a);
it != end(a);
++it;
*it;
```

the type `A` is defined *iterable* and all of its items will be printed. The code:

```C++
auto v0 = std::list<int> {10, 20, 30};
IC(v0);
```

will print:

    ic| v0: [10, 20, 30]


#### Tuple like types

A `std::pair<T1, T2>` and `std::tuple<Ts...>` typed variables will print all of its values.

The code:

```C++
auto v0 = std::make_pair(10, 3.14);
auto v1 = std::make_tuple(7, 6.28, "bla");
IC(v0, v1);
```

will print:

```
ic| v0: (10, 3.14), v1: (7, 6.28, "bla")
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

```
ic| v0: 10, v1: nullopt
```


#### Variant types

A `std::variant<Ts...>` or `boost::variant2::variant<Ts...>` typed variable will print its
value.

The code:

```C++
auto v0 = std::variant<int, double, char> {4.2};
IC(v0);
```

will print:

```
ic| v0: 4.2
```


#### Exception types

Types inheriting from `std::exception` will print the response of `std::exception::what()`
method. If besides that it inherits from `boost::exception` the response of
`boost::diagnostic_information()` will be print too.

The code:

```C++
auto v0 = std::runtime_error("error description");
IC(v0);
```

will print:

```
ic| v0: error description
```

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

While most compilers will work just fine, until the C++20 the standard requires at least
one argument when calling a variadic macro. To handle this the nullary macros `IC0()` and
`ICECREAM0()` are defined alongside `IC(...)` and `ICECREAM(...)`.

## Similar projects

The [CleanType](https://github.com/pthom/cleantype) library has a focus on printing
readable types names, but there is support to print variables names and values alongside
its types. An optional integration of CleanType with IceCream-Cpp, if the first is present
on system, is being planed. With that would be possible to show the types of values within
`IC(...)` macro.

[CI.badge]: https://github.com/renatoGarcia/icecream-cpp/workflows/CI/badge.svg
[CI.page]: https://github.com/renatoGarcia/icecream-cpp/actions?query=workflow%3ACI
