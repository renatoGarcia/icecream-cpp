# IceCream-Cpp

[![CI.badge]][CI.link]
[![LICENSE.badge]][LICENSE.link]

IceCream-Cpp is a little (single header) library to help with the print debugging on C++11 and forward.

[Try it at godbolt!](https://godbolt.org/z/4133d1Kxq)

**Contents**
* [Install](#install)
  * [Nix](#nix)
  * [Conan](#conan)
* [Usage](#usage)
  * [Return value and IceCream apply macro](#return-value-and-icecream-apply-macro)
  * [Output formatting](#output-formatting)
     * [Format string syntax](#format-string-syntax)
  * [Character Encoding](#character-encoding)
  * [Configuration](#configuration)
     * [enable/disable](#enabledisable)
     * [output](#output)
     * [prefix](#prefix)
     * [show_c_string](#show_c_string)
     * [wide_string_transcoder](#wide_string_transcoder)
     * [unicode_transcoder](#unicode_transcoder)
     * [output_transcoder](#output_transcoder)
     * [line_wrap_width](#line_wrap_width)
     * [include_context](#include_context)
     * [context_delimiter](#context_delimiter)
  * [Printing logic](#printing-logic)
     * [C strings](#c-strings)
     * [Ordinary narrow strings](#ordinary-narrow-strings)
     * [Wide strings](#wide-strings)
     * [Unicode strings](#unicode-strings)
     * [Pointer like types](#pointer-like-types)
     * [Iterable types](#iterable-types)
     * [Tuple like types](#tuple-like-types)
     * [Optional types](#optional-types)
     * [Variant types](#variant-types)
     * [Exception types](#exception-types)
     * [Standard layout types](#standard-layout-types-clang-only)
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
IC(a, b, sum(a, b));
```

and will print:

    ic| a: 7, b: 2, sum(a, b): 9

This library is inspired by and aims to behave the most identical as possible to the
original [Python IceCream](https://github.com/gruns/icecream) library.

## Install

The IceCream-Cpp is a one file, header only library, having the STL as its only
dependency. The most immediate way to use it, is just copy the `icecream.hpp` header into
your project.

To properly install it system wide, together with the CMake project files, run these
commands in IceCream-Cpp project root directory:
```Shell
mkdir build
cd build
cmake ..
cmake --install .
```

### Nix

If using [Nix](https://nixos.org), IceCream-Cpp can be included as a flakes input as
```Nix
inputs.icecream-cpp.url = "github:renatoGarcia/icecream-cpp";
```

The IceCream-Cpp flake defines an overlay so that it can be used when importing `nixpkgs`:
```Nix
import nixpkgs {
  system = "x86_64-linux";
  overlays = [
    icecream-cpp.overlays.default
  ];
}
```
Doing this, an `icecream-cpp` derivation will be added to the `nixpkgs` attribute set.

A working example of how to use IceCream-Cpp in a flake project is [here](example_project/flake.nix).


### Conan

The released versions are available on [Conan](https://conan.io) too:
```Shell
conan install icecream-cpp/0.3.1@
```

## Usage

If using CMake:
```CMake
find_package(IcecreamCpp)
include_directories(${IcecreamCpp_INCLUDE_DIRS})
```
will add the installed directory on include paths list.

After including the `icecream.hpp` header on a source file, here named `test.cpp`:

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

All the functionalities of IceCream-Cpp library are implemented by the macros `IC`,
[`IC_`](#output-formatting), [`IC_A`](#return-value-and-icecream-apply-macro), and
[`IC_A_`](#output-formatting).

### Return value and IceCream apply macro

If called with no arguments the `IC(...)` macro will return `void`, if called with one
argument it will return the argument itself, and if called with multiple arguments it will
return a tuple with all of them.

This is done in this way so that you can use `IC` to inspect a function argument at
calling point, with no further code change. On the code:

```C++
my_function(IC(MyClass{}));
```
the created `MyClass` instance will be passed to `my_function` exactly the same as if the
`IC` macro was not there. The `my_function` will keep receiving a rvalue reference of a
`MyClass` object.

This approach however is not so practical when the function has many arguments. On the code:
```C++
my_function(IC(a), IC(b), IC(c), IC(d));
```
besides writing four times the `IC` macro, the printed output will be split on four
distinct lines. Something like:

    ic| a: 1
    ic| b: 2
    ic| c: 3
    ic| d: 4

Unfortunately, just wrapping all the four arguments in a single `IC` call will not work
too. The returned value will be a `std:::tuple` with `(a, b, c, d)` and the `my_function`
expects four arguments.

To work around that, there are the `IC_A` macro. `IC_A` behaves exactly as the `IC` macro,
but receives a function (any callable actually) as its first argument, and will call that
function with all the following arguments, printing all of them before. That last code can
be rewritten as:
```C++
IC_A(my_function, a, b, c, d);
```
and this time will print:

    ic| a: 1, b: 2, c: 3, d: 4

`IC_A` will return the same value returned by the callable. The code:
```C++
auto mc = std::make_unique<MyClass>();
auto r = IC_A(mc->my_function, a, b);
```
behaves exactly the same as:
```C++
auto mc = std::make_unique<MyClass>();
auto r = mc->my_function(a, b);
```
but will print the values of `a` and `b`.


### Output formatting

It is possible to configure how the value must be formatted while printing. The following code:

```C++
auto a = int{42};
auto b = int{20};
IC_("#X", a, b);

```

will print:

    ic| a: 0X2A, b: 0X14

The same formatting string will be applied to all the values on an `IC` macro call.

To configure the formating of [`IC_A`](#return-value-and-icecream-apply-macro) macro,
there are the macro `IC_A_`. It is just like `IC_A` but receiving a formating string as
its first argument. The code:
```C++
IC_A_("#x", my_function, 10, 20);
```
will print:

    ic| 10: 0xa, 20: 0x14


#### Format string syntax

The adopted formatting string is strongly based on
[{fmt}](https://fmt.dev/latest/syntax.html#format-specification-mini-language) and [STL
Formatting](https://en.cppreference.com/w/cpp/utility/format/formatter#Standard_format_specification)
has the following syntax:

```
format_spec ::=  [[fill]align][sign]["#"][width]["." precision][type]
fill        ::=  <a character>
align       ::=  "<" | ">" | "v"
sign        ::=  "+" | "-"
width       ::=  integer
precision   ::=  integer
type        ::=  "a" | "A" | "d" | "e" | "E" | "f" | "F" | "g" | "G" | "o" | "x" | "X"
integer     ::=  digit+
digit       ::=  "0"..."9"
```

##### [[fill]align]

The fill character can be any char. The presence of a fill character is signaled by the
character following it, which must be one of the alignment options. The meaning of the
alignment options is as follows:

| Symbol | Meaning                                                                                                                                                |
|--------|--------------------------------------------------------------------------------------------------------------------------------------------------------|
| `'<'`  | Left align within the available space.                                                                                                                 |
| `'>'`  | Right align within the available space. This is the default.                                                                                           |
| `'v'`  | Internally align the data, with the fill character being placed between the digits and either the base or sign. Applies to integer and floating-point. |

Note that unless a minimum field width is defined, the field width will always be the same
size as the data to fill it, so that the alignment option has no meaning in this case.

##### [sign]

The sign option is only valid for number types, and can be one of the following:

| Symbol | Meaning                                                                 |
|--------|-------------------------------------------------------------------------|
| `'+'`  | A sign will be used for both nonnegative as well as negative numbers. |
| `'-'`  | A sign will be used only for negative numbers. This is the default.   |

##### ["#"]

Causes the “alternate form” to be used for the conversion. The alternate form is defined
differently for different types. This option is only valid for integer and floating-point
types. For integers, when binary, octal, or hexadecimal output is used, this option adds
the prefix respective "0b" ("0B"), "0", or "0x" ("0X") to the output value. Whether the
prefix is lower-case or upper-case is determined by the case of the type specifier, for
example, the prefix "0x" is used for the type 'x' and "0X" is used for 'X'. For
floating-point numbers the alternate form causes the result of the conversion to always
contain a decimal-point character, even if no digits follow it. Normally, a decimal-point
character appears in the result of these conversions only if a digit follows it. In
addition, for 'g' and 'G' conversions, trailing zeros are not removed from the result.

##### [width]

A decimal integer defining the minimum field width. If not specified, then the field width
will be determined by the content.

##### ["." precision]

The precision is a decimal number indicating how many digits should be displayed after the
decimal point for a floating-point value formatted with 'f' and 'F', or before and after
the decimal point for a floating-point value formatted with 'g' or 'G'. For non-number
types the field indicates the maximum field size - in other words, how many characters
will be used from the field content. The precision is not allowed for integer, character,
Boolean, and pointer values. Note that a C string must be null-terminated even if
precision is specified.

##### [type]

Determines how the data should be presented.

The available integer presentation types are:

| Symbol | Meaning                                                                                                                                                                   |
|--------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `'d'`  | Decimal integer. Outputs the number in base 10.                                                                                                                           |
| `'o'`  | Octal format. Outputs the number in base 8.                                                                                                                               |
| `'x'`  | Hex format. Outputs the number in base 16, using lower-case letters for the digits above 9. Using the '#' option with this type adds the prefix "0x" to the output value. |
| `'X'`  | Hex format. Outputs the number in base 16, using upper-case letters for the digits above 9. Using the '#' option with this type adds the prefix "0X" to the output value. |

The available presentation types for floating-point values are:

| Symbol | Meaning                                                                                                                                                                                                                                                                     |
|--------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `'a'`  | Hexadecimal floating point format. Prints the number in base 16 with prefix "0x" and lower-case letters for digits above 9. Uses 'p' to indicate the exponent.                                                                                                              |
| `'A'`  | Same as 'a' except it uses upper-case letters for the prefix, digits above 9 and to indicate the exponent.                                                                                                                                                                  |
| `'e'`  | Exponent notation. Prints the number in scientific notation using the letter ‘e’ to indicate the exponent.                                                                                                                                                                  |
| `'E'`  | Exponent notation. Same as 'e' except it uses an upper-case 'E' as the separator character.                                                                                                                                                                                 |
| `'f'`  | Fixed point. Displays the number as a fixed-point number.                                                                                                                                                                                                                   |
| `'F'`  | Fixed point. Same as 'f', but converts nan to NAN and inf to INF.                                                                                                                                                                                                           |
| `'g'`  | General format. For a given precision p >= 1, this rounds the number to p significant digits and then formats the result in either fixed-point format or in scientific notation, depending on its magnitude. A precision of 0 is treated as equivalent to a precision of 1. |
| `'G'`  | General format. Same as 'g' except switches to 'E' if the number gets too large. The representations of infinity and NaN are uppercased, too.                                                                                                                               |


### Character Encoding

Character encoding in C++ is messy.

The `char8_t`, `char16_t`, and `char32_t` strings are well defined. They are capable, and
do hold Unicode code units of 8, 16, and 32 bits respectively, and they are encoded in
UTF-8, UTF-16, and UTF-32 also respectively.

The `char` strings have a well defined code unit bit size (given by
[`CHAR_BIT`](https://en.cppreference.com/w/cpp/types/climits), usually 8 bits), but there
are no requirements about its encoding.

The `wchar_t` strings have [neither a well defined code unit
size](https://en.cppreference.com/w/cpp/language/types#Character_types), nor any
requirements about its encoding.

In a code like this:

```C++
auto const str = std::string{"foo"};
std::cout << str;
```

We will have three character encoding points of interest. In the first one, before
compiling, that code will be in a source file in an unspecified "source encoding". In the
second interest point, the compiled binary will have the "foo" string saved in an
unspecified "execution encoding". Finally on the third point, the "foo" byte stream
received by `std::cout` will be ultimately forwarded to the system, that expects the
stream being encoded in an also unspecified "output encoding".

From that three interest points of character encoding, both "execution encoding", and
"output encoding" have impact in the working flow of Icecream-cpp, and there is no way to
know for sure what is the used encoding in both of them. In face of this uncertainty, the
adopted strategy is offer a reasonable default transcoding function, that will try convert
the data to the right encoding, and allow the user to use its own implementation when
needed.

Except for wide and Unicode string types (discussed below), when printing any other type
we will have its serialized textual data in "execution encoding". That "execution
encoding" may or may not be the same as the "output encoding", this one being the encoded
expected by the configured [output](#output). Because of that, before we send that data to
the output, we must transcode it to make sure that we have it in "output encoding". To
that end, before delivering the text data to the [output](#output), we send it to the
configured [output_transcoder](#output_transcoder) function, that must ensure it is
encoded in the correct "output encoding".

When printing the wide and Unicode string types, we need to have one more transcoding
level, because it is possible that the text data is in a distinct character encoding from
the expected "execution encoding".  Because of that, additional logic is applied to make
sure that the strings are in "execution encoding" before we send them to output. This is
further discussed in [wide strings](#wide-strings), and [unicode
strings](#unicode-strings) sections.


### Configuration

The `Icecream` class is internally implemented as a singleton. All the configuration
changes will be done to a unique object, and shared across all the program and threads.

All configurations are done/viewed through accessor methods, using the `icecream::ic`
object. To allow the method chaining idiom all the set methods return a reference of the
`ic` object:

```C++
icecream::ic
    .prefix("ic: ")
    .show_c_string(false)
    .line_wrap_width(70);
```

For simplification purposes, on the following examples a `using icecream::ic` statement
will be presumed.

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

#### output

Sets where the serialized textual data will be printed. By default that data will be
printed on the standard error output, the same as `std::cerr`.

- set:
    ```C++
    template <typename T>
    auto output(T&& t) -> IcecreamAPI&;
    ```

The type `T` can be any of:
- A class inheriting from `std::ostream`.
- A class having a method `push_back(char)`.
- An output iterator that accepts the operation `*it = 'c'`

For instance, the code:
```C++
auto str = std::string{};
icecream::ic.output(str);
IC(1, 2);
```
Will print the output `"ic| 1: 1, 2: 2\n"` on the `str` string.

*Warning:* The `Icecream` class won't take property of the argument `t`, so care must be
taken by the user to keep it alive.

#### prefix

The text that will be printed before each output. It can be set to a string, a nullary
callable that returns an object having an overload of `operator<<(ostream&, T)`, or any
number of instances of those two. The printed prefix will be a concatenation of all those
elements.

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
(`true`) or a pointer to a `char` (`false`). The default value is `true`.

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

#### wide_string_transcoder

Function that transcodes a `wchar_t` string, from a system defined encoding to a `char`
string in the system "execution encoding".

- set:
    ```C++
    auto wide_string_transcoder(std::function<std::string(wchar_t const*, std::size_t)> transcoder) -> IcecreamAPI&;
    auto wide_string_transcoder(std::function<std::string(std::wstring_view)> transcoder) -> IcecreamAPI&;
    ```

There is no guarantee that the input string will end on a null terminator (this is the
actual semantic of string_view), so the user must observe the input string size value.

The default implementation will check if the C locale is set to other value than "C" or
"POSIX". If yes, it will forward the input to the
[std::wcrtomb](https://en.cppreference.com/w/cpp/string/multibyte/wcrtomb) function.
Otherwise, it will assume that the input is Unicode encoded (UTF-16 or UTF-32, accordingly
to the byte size of `wchar_t`), and transcoded it to UTF-8.

#### unicode_transcoder

Function that transcodes a `char32_t` string, from a UTF-32 encoding to a `char` string in
the system "execution encoding".

- set:
    ```C++
    auto unicode_transcoder(std::function<std::string(char32_t const*, std::size_t)> transcoder) -> IcecreamAPI&;
    auto unicode_transcoder(std::function<std::string(std::u32string_view)> transcoder) -> IcecreamAPI&;
    ```

There is no guarantee that the input string will end on a null terminator (this is the
actual semantic of string_view), so the user must observe the input string size value.

The default implementation will check the C locale is set to other value than "C" or
"POSIX". If yes, it will forward the input to the
[std::c32rtomb](https://en.cppreference.com/w/cpp/string/multibyte/c32rtomb) function.
Otherwise, it will just transcoded it to UTF-8.

This function will be used to transcode all the `char8_t`, `char16_t`, and `char32_t`
strings.  When transcoding `char8_t` and `char16_t` strings, they will be first converted
to a `char32_t` string, before being sent as input to this function.

#### output_transcoder

Function that transcodes a `char` string, from in the system "execution encoding" to a
`char` string in the system "output encoding", as expected by the configured
[output](#output).

- set:
    ```C++
    auto output_transcoder(std::function<std::string(char const*, std::size_t)> transcoder) -> IcecreamAPI&;
    auto output_transcoder(std::function<std::string(std::string_view)> transcoder) -> IcecreamAPI&;
    ```

There is no guarantee that the input string will end on a null terminator (this is the
actual semantic of string_view), so the user must observe the input string size value.

The default implementation assumes that the "execution encoding" is the same as the
"output encoding", and will just return an unchanged input.

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

When printing a type `T`, the precedence is use an overloaded function
`operator<<(ostream&, T)` always when it is available. The exceptions to that rule are
strings (C strings, `std::string`, and `std::string_view`), `char` and bounded arrays.
Strings will be enclosed by `"`, `char` will be enclosed by `'`, and arrays are considered
iterables rather than let decay to raw pointers.

In general, if an `operator<<(ostream&, T)` overload is not available to a type `T`, a
call to `IC(t)` will result on a compiling error. All exceptions to that rule, when
IceCream-Cpp will print a type `T` even without an `operator<<(ostream&, T)` overload are
discussed below. Note however that even to those, if a user implements a custom
`operator<<(ostream&, T)` it will take precedence and be used instead.

#### C strings

C strings are ambiguous. Should a `char* foo` variable be interpreted as a pointer to a
single `char` or as a null-terminated string? Likewise, is the `char bar[]` variable an
array of single characters or a null-terminated string? Is `char baz[3]` an array with
three single characters or is it a string of size two plus a `'\0'`?

Each one of those interpretations of `foo`, `bar`, and `baz` would be printed in a
distinct way. To the code:

```C++
char flavor[] = "pistachio";
IC(flavor);
```

all three outputs below are correct, each one having a distinct interpretation of what
should be the `flavor` variable.

```
ic| flavor: 0x55587b6f5410
ic| flavor: ['p', 'i', 's', 't', 'a', 'c', 'h', 'i', 'o', '\0']
ic| flavor: "pistachio"
```

The IceCream-Cpp policy is handle any bounded `char` array (i.e.: array with a known size)
as an array of single characters. So the code:

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


#### Wide strings

Any realization of `wchar_t` strings, like `wchar_t*`, `std::wstring`, and `std::string_view`

Since the [output](#output) expects a `char` string, we must convert the text data to that
format, making sure that it is in "execution encoding". Icecream-cpp implements a default
transcoder function for doing that, but is possible to customize it by setting the
[wide_string_transcoder](#wide_string_transcoder) option.

#### Unicode strings

Any realization of `char8_t`, `char16_t`, and `char32_t` strings, like `char32_t*`,
`std::u8string`, and `std::u16string_view`

Since the [output](#output) expects a `char` string, we must convert the data to that
format, making sure that it is in "execution encoding". Icecream-cpp implements a default
transcoder function for doing that, but is possible to customize it by setting the
[unicode_transcoder](#unicode_transcoder) option.


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

If for a type `A` with an instance `a`, all following operations are valid:

```C++
auto it = begin(a);
it != end(a);
++it;
*it;
```

the type `A` is defined *iterable*, and if `A` has no overload of `operator<<(ostream&,
A)`, all of its items will be printed instead. The code:

```C++
auto v0 = std::list<int> {10, 20, 30};
IC(v0);
```

will print:

    ic| v0: [10, 20, 30]


#### Tuple like types

A `std::pair<T1, T2>` or `std::tuple<Ts...>` typed variables will print all of its
elements.

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

Types inheriting from `std::exception` will print the return of `std::exception::what()`
method. If beyond that it inherits from `boost::exception` too, the response of
`boost::diagnostic_information()` will be also printed.

The code:

```C++
auto v0 = std::runtime_error("error description");
IC(v0);
```

will print:

```
ic| v0: error description
```

#### Standard layout types (Clang only)

If using Clang >= 15, a class will be printable even without an `operator<<(ostream&, T)` overload.

The code:
```C++
class S
{
public:
    float f;
    int ii[3];
};

S s = {3.14, {1,2,3}};
IC(s);
```
will print:
```
ic| s: {f: 3.14, ii: [1, 2, 3]}
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
its types.


[CI.badge]: https://github.com/renatoGarcia/icecream-cpp/workflows/CI/badge.svg
[CI.link]: https://github.com/renatoGarcia/icecream-cpp/actions?query=workflow%3ACI

[LICENSE.badge]: https://img.shields.io/badge/licence-MIT-blue
[LICENSE.link]: https://raw.githubusercontent.com/renatoGarcia/icecream-cpp/master/LICENSE.txt
