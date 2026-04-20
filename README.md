# Icecream-cpp

[![CI.badge]][CI.link]
[![LICENSE.badge]][LICENSE.link]

Icecream-cpp is a little (single header) library to help with print debugging in C++11 and
later.

[Try it at Compiler Explorer!](https://godbolt.org/z/WKbxejjaa)


Tracking execution flow:

```c++
auto roll_attack(int roll) -> void
{
    std::cout << "entered roll_attack" << std::endl;
    if (roll == 20)
        std::cout << "critical hit branch" << std::endl;
    else
        std::cout << "normal hit branch" << std::endl;
}
```

can be [coded instead](#direct-printing):

```c++
auto roll_attack(int roll) -> void
{
    IC();
    if (roll == 20)
        IC();
    else
        IC();
}
```

and will print something like:

    ic| combat.cpp:12 in "void roll_attack(int)"
    ic| combat.cpp:14 in "void roll_attack(int)"

Inspecting variables:

```c++
std::cout << "a: " << a
          << ", b: " << b
          << ", sum(a, b): " << sum(a, b)
          << std::endl;
```

can be [simplified to](#direct-printing):

```c++
IC(a, b, sum(a, b));
```

and will print:

    ic| a: 7, b: 2, sum(a, b): 9

[Formatting output](#output-formatting). Instead of:

```c++
auto answer = int{42};
auto num = int{10};
std::cout << "answer: 0x" << std::hex << answer << ", num: 0x" << num << std::endl;
```

just write:

```c++
IC_F("#x", answer, num);
```

and get:

    ic| answer: 0x2a, num: 0xa

Slicing ranges [like in Python](#range-format-string):

```c++
auto v = std::vector<char32_t>{U'α', U'β', U'γ', U'δ', U'ε', U'ζ'};
IC_F("[1:-2]", v);
```

will print:

    ic| v: [1:-2]->['β', 'γ', 'δ']

Debugging range pipelines. Insert [`IC_V()`](#range-views-pipeline) to inspect data
flowing through [STL ranges](https://en.cppreference.com/w/cpp/header/ranges) or
[Range-v3](https://ericniebler.github.io/range-v3/) pipelines:

```c++
auto rv = std::vector<int>{1, 0, 2, 3, 0, 4, 5}
    | vws::split(0)
    | IC_V()
    | vws::enumerate;
```

When iterating on `rv`, we will see:

    ic| range_view_63:16[0]: [1]
    ic| range_view_63:16[1]: [2, 3]
    ic| range_view_63:16[2]: [4, 5]

This library was inspired by the original [Python
IceCream](https://github.com/gruns/icecream) library.


**Contents**
* [Rationale](#rationale)
* [Install](#install)
  * [Quick Start](#quick-start)
* [Basic Usage](#basic-usage)
  * [Direct printing](#direct-printing)
  * [Range views pipeline](#range-views-pipeline)
  * [Return value and Icecream apply macro](#return-value-and-icecream-apply-macro)
  * [Output formatting](#output-formatting)
* [Configuration](#configuration)
* [Printing Strategies](#printing-strategies)
* [Third-party Libraries](#third-party-libraries)
* [Compatibility](#compatibility)
* [Reference](#reference)
  * [C strings](#c-strings)
  * [Character Encoding](#character-encoding)
  * [Macro disabling](#macro-disabling)
* [Pitfalls](#pitfalls)


## Rationale

Icecream-cpp is a throwaway library. It is designed for use during development and
debugging, not for shipping with the released product. It complements debuggers rather
than replacing them, sometimes print debugging is simply faster or more practical.

Because of this, Icecream-cpp is one of the few libraries where you will spend more time
writing code with it than reading code with it. Our design goals are expressivity and
conciseness: print any variable, format it as desired, with as little code as possible.


## Install

Icecream-cpp is a single-file, header-only library with only the STL as dependency. The
simplest way to use it is to copy `icecream.hpp` into your project.

To install system-wide with CMake support, run from the project root:

```Shell
mkdir build
cd build
cmake ..
cmake --install .
```

### Conan

Released versions are available on
[Conan](https://conan.io/center/recipes/icecream-cpp). See
[example_project/conanfile.txt](example_project/conanfile.txt) for a usage example.

### Nix

Icecream-cpp can be included as a flakes input:
```Nix
inputs.icecream-cpp.url = "github:renatoGarcia/icecream-cpp";
```

The flake defines an overlay for use when importing `nixpkgs`:
```Nix
import nixpkgs {
  system = "x86_64-linux";
  overlays = [
    icecream-cpp.overlays.default
  ];
}
```
This adds an `icecream-cpp` derivation to the `nixpkgs` attribute set.

See [example_project/flake.nix](example_project/flake.nix) for a working example.

### Quick Start

With CMake:
```CMake
find_package(icecream-cpp)
target_link_libraries(<target> PRIVATE icecream-cpp::icecream-cpp)
```
Where `<target>` is the executable or library being compiled.

Then include the header:

```C++
#include <icecream.hpp>

int main(int, char*[])
{
    auto x = 42;
    auto name = std::string{"foo"};
    IC(x, name);  // prints: ic| x: 42, name: "foo"
    return 0;
}
```


## Basic Usage

The main Icecream-cpp functions are [`IC`](#direct-printing),
[`IC_A`](#return-value-and-icecream-apply-macro), and [`IC_V`](#range-views-pipeline);
together with their respective counterparts `IC_F`, `IC_FA`, and `IC_FV`; that behave the
same but accept an [output formatting string](#output-formatting) as their first argument.


### Direct printing

`IC` is the simplest of the Icecream-cpp functions. If called with no arguments it will
print the [prefix](#prefix), the source file name, the current line number, and the
current function signature. The code:

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

If called with arguments it will print the [prefix](#prefix), those argument names, and
their values.  The code:

```C++
auto v0 = std::vector<int>{1, 2, 3};
auto s0 = std::string{"bla"};
IC(v0, s0, 3.14);
```

will print:

    ic| v0: [1, 2, 3], s0: "bla", 3.14: 3.14

The variant `IC_F` behaves the same as the `IC` function, but accepts an [output
formatting string](#output-formatting) as its first argument.


### Range views pipeline

To print the data flowing through a range views pipeline (both with [STL
ranges](https://en.cppreference.com/w/cpp/header/ranges) and
[Range-v3](https://ericniebler.github.io/range-v3/)), we can use either the `IC_V` or
`IC_FV` functions, which will lazily print any input coming from the previous view. `IC_FV` behaves the same as `IC_V`, but accepts a [format string](#output-formatting)
(same syntax as the [*range format string*](#range-format-string)) as its first argument. Since these functions will be placed within a range views pipeline, the
printing will be done lazily, while each element is generated. For instance:

```C++
namespace vws = std::views;
auto v0 = vws::iota('a') | vws::enumerate | IC_V() | vws::take(3);
for (auto e : v0)
{
    //...
}
```

In this code nothing will be immediately printed when `v0` is created, just when iterating
over it. At each `for` loop iteration one line will be printed, until we have the output:

    ic| range_view_61:53[0]: (0, 'a')
    ic| range_view_61:53[1]: (1, 'b')
    ic| range_view_61:53[2]: (2, 'c')

> [!NOTE]
> The Icecream-cpp will enable its support to Range-v3 types either if the "icecream.hpp"
> header is included some lines after any Range-v3 header, or if the `ICECREAM_RANGE_V3`
> macro was declared before the "icecream.hpp" header inclusion. This is discussed in
> details at the [*third-party libraries*](#third-party-libraries) section.

`IC_V` has the signature `IC_V(name, projection)`, and `IC_FV` has
`IC_FV(fmt, name, projection)`. In both, the `name` and `projection` parameters are
optional.

#### fmt

The same syntax as described at [*range types format string*](#range-format-string).

#### name

The variable name used by the view when printing. The printing layout is: `<name>[<idx>]:
<value>`. If the name parameter is not used, the default value to `<name>` is
`range_view_<source_location>`.

The code:

```C++
vws::iota('a') | vws::enumerate | IC_V("foo") | vws::take(2);
```

when iterated over will print:

    ic| foo[0]: (0, 'a')
    ic| foo[1]: (1, 'b')


#### projection

A [callable](https://en.cppreference.com/w/cpp/named_req/Callable) that will receive as
input a *const reference* to the current element at the *range views pipeline*, and must
return the actual value to be printed.

The code:

```C++
vws::iota('a') | vws::enumerate | IC_V([](auto e){return std::get<1>(e);}) | vws::take(2);
```

when iterated over will print:

    ic| range_view_61:53[0]: 'a'
    ic| range_view_61:53[1]: 'b'

> [!NOTE]
> The `IC_V` function will forward to the next view the exact same input element, just as
> it was received from the previous view. No action done by the `projection` function will
> have any effect on that.


### Return value and Icecream apply macro

[`IC`](#direct-printing) (and [`IC_F`](#output-formatting)) returns no value when called
with zero or multiple arguments. When called with exactly one argument, it returns that
argument.

This allows using `IC` to inspect a function argument at the call site without changing
the code structure. In the code:

```C++
my_function(IC(MyClass{}));
```
the `MyClass` object will be forwarded to `my_function` as if the `IC` function wasn't
there. The `my_function` will continue receiving a rvalue reference to a `MyClass` object.

This approach, however, is not practical when the function has multiple arguments. In the code:
```C++
another_function(IC(a), IC(b), IC(c), IC(d));
```
besides writing four times the `IC` function, the printed output will be split in four
lines. Something like:

    ic| a: 1
    ic| b: 2
    ic| c: 3
    ic| d: 4

To work around that, there is the `IC_A` function. `IC_A` behaves exactly like the `IC`
function, but receives a [callable](https://en.cppreference.com/w/cpp/named_req/Callable)
as its first argument, and will call it using the remaining arguments, printing all of
them before that. That previous example code could be rewritten as:

```C++
IC_A(another_function, a, b, c, d);
```

and this time it will print:

    ic| a: 1, b: 2, c: 3, d: 4

The `IC_A` function will return the same value returned by the callable. The code:

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

The `IC_FA` variant does the same as the `IC_A` function, but accepts an [output
formatting string](#output-formatting) as its first argument, just before the callable
argument.


### Output formatting

It is possible to configure how a value must be formatted while printing. The following
code:

```C++
auto a = int{42};
auto b = int{20};
IC_F("#X", a, b);
```

will print:

    ic| a: 0X2A, b: 0X14

if using the `IC_F` variant instead of the plain [`IC`](#direct-printing) function. A
similar result would be achieved if using `IC_FA` and `IC_FV` in place of
[`IC_A`](#return-value-and-icecream-apply-macro) and [`IC_V`](#range-views-pipeline)
respectively.

When using the formatting function variants (`IC_F` and `IC_FA`), the same formatting
string will be applied by default to all of its arguments. That could be a problem if we
need to have distinct formatting to each argument, or if the arguments have multiple types
with mutually incompatible format syntaxes. Therefore, to set a distinct formatting string to a
specific argument we can wrap it with the `IC_` function. The code:

```C++
auto a = int{42};
auto b = int{20};
IC_F("#X", a, IC_("d", b));
```

will print:

    ic| a: 0X2A, b: 20

The `IC_` function can also be used within the plain `IC` (or `IC_A`) function:

```C++
auto a = int{42};
auto b = int{20};
IC(IC_("#x", a), b);
```

will print:

    ic| a: 0x2a, b: 20

The last argument in an `IC_` function call is the one that will be printed, all other
arguments coming before the last will be converted to a string using the
[`to_string`](https://en.cppreference.com/w/cpp/string/basic_string/to_string) function
and concatenated to the resulting formatting string.

```C++
auto a = float{1.234};
auto width = int{7};
IC(IC_("*<",width,".3", a));
```

This produces a formatting string `"*<7.3"`, and will print:

    ic| a: 1.23***

For completeness, here is an example using `IC_FA` and `IC_FV`:

```C++
IC_FA("#x", my_function, 10, 20);
auto rv0 = vws::iota(0) | IC_FV("[::2]:#x", "bar") | vws::take(5);
```

This will at first print:

    ic| 10: 0xa, 20: 0x14

and when iterating on `rv0`:

    ic| bar[0]: 0
    ic| bar[2]: 0x2
    ic| bar[4]: 0x4

For `IC_F` and `IC_FA`, the format string syntax depends on the type being printed and its
[printing strategy](#printing-strategies).

For `IC_FV`, the format syntax is the same as the [Range format
string](#range-format-string).


## Configuration

The Icecream-cpp configuration system works *layered by scope*. At the base level we have
the global `IC_CONFIG` object, shared by the whole program. It is created with all config
options at their default values, and any change is immediately visible program-wide.

At any point, we can create a new config layer in the current scope by calling the
`IC_CONFIG_SCOPE()` macro, which creates a new `IC_CONFIG` variable. All config options of
this new instance are *unset* by default, and any request for an unset option is delegated
to the parent object. The request goes up the parent chain until the first one with that
option set responds.

All config options are set by using accessor methods of the `IC_CONFIG` object, and they
can be chained:

```C++
IC_CONFIG
    .prefix("ic: ")
    .show_c_string(false)
    .line_wrap_width(70);
```

`IC_CONFIG` is just a regular variable with a funny name to make a collision extremely
unlikely. When calling any `IC*(...)` macro, it will pick the `IC_CONFIG` instance at
current scope by doing an [unqualified name
lookup](https://en.cppreference.com/w/cpp/language/unqualified_lookup), using the same
rules applied to any other regular variable.

To summarize all the above, in the code:

```C++
auto my_function() -> void
{
    IC_CONFIG.line_wrap_width(20);

    IC_CONFIG_SCOPE();
    IC_CONFIG.context_delimiter("|");
    IC_CONFIG.show_c_string(true);

    {
        IC_CONFIG_SCOPE();
        IC_CONFIG.show_c_string(false);
        // A
    }
    // B
}
```

At line `A`, the value of `IC_CONFIG`'s `line_wrap_width`, `context_delimiter`, and
`show_c_string` will be respectively: `20`, `"|"`, and `false`.

After the closing of the innermost scope block, at line `B`, the value of `IC_CONFIG`'s
`line_wrap_width`, `context_delimiter`, and `show_c_string` will be respectively: `20`,
`"|"`, and `true`.

The reading and writing operations on `IC_CONFIG` objects are thread safe.

> [!NOTE]
> Any modification in an `IC_CONFIG`, other than to the global instance, will be seen only
> within the current scope. As a consequence, those modifications won't propagate to the
> scope of any called function.

**Options:**
[enable/disable](#enabledisable) |
[output](#output) |
[prefix](#prefix) |
[show_c_string](#show_c_string) |
[decay_char_array](#decay_char_array) |
[force_range_strategy](#force_range_strategy) |
[force_tuple_strategy](#force_tuple_strategy) |
[force_variant_strategy](#force_variant_strategy) |
[wide_string_transcoder](#wide_string_transcoder) |
[unicode_transcoder](#unicode_transcoder) |
[output_transcoder](#output_transcoder) |
[line_wrap_width](#line_wrap_width) |
[include_context](#include_context) |
[context_delimiter](#context_delimiter)


### enable/disable

Enable or disable the output of `IC(...)` macro. The default value is *enabled*.

- get:
    ```C++
    auto is_enabled() const -> bool;
    ```
- set:
    ```C++
    auto enable() -> Config&;
    auto disable() -> Config&;
    ```

The code:

```C++
IC(1);
IC_CONFIG.disable();
IC(2);
IC_CONFIG.enable();
IC(3);
```

will print:

```
ic| 1: 1
ic| 3: 3
```

### output

Sets where the serialized textual data will be printed. By default that data will be
printed in the *standard error output*, the same as `std::cerr`.

- set:
    ```C++
    template <typename T>
    auto output(T&& t) -> Config&;
    ```

Where the type `T` must be one of:
- A class inheriting from `std::ostream`,
- A class having a method `push_back(char)`,
- An output iterator that accepts the operation `*it = 'c'`.

For instance, the code:
```C++
auto str = std::string{};
IC_CONFIG.output(str);
IC(1, 2);
```
will print the output `"ic| 1: 1, 2: 2\n"` to the `str` string.

> [!WARNING]
> Icecream-cpp won't take ownership of the `t` argument, so care must be taken by the user
> to ensure that it stays alive.

### prefix

A function that generates the text that will be printed before each output.

- set:
    ```C++
    template <typename... Ts>
    auto prefix(Ts&& ...values) -> Config&;
    ```

Where each one of the types `Ts` must be one of:
- A string,
- A callable `T() -> U`, where `U` has an overload of `operator<<(ostream&, U)`.

The printed prefix will be a concatenation of all those elements.

The code:
```C++
IC_CONFIG.prefix("icecream| ");
IC(1);
IC_CONFIG.prefix([]{return 42;}, "- ");
IC(2);
IC_CONFIG.prefix("thread ", std::this_thread::get_id, " | ");
IC(3);
```

will print:

```
icecream| 1: 1
42- 2: 2
thread 1 | 3: 3
```

### show_c_string

Controls if a character pointer variable (`char*`, `wchar_t*`, `char8_t*`, `char16_t*`, or
`char32_t*`) should be interpreted as a null-terminated C string (`true`) or a pointer to
a `char` (`false`). The default value is `true`.

- get:
    ```C++
    auto show_c_string() const -> bool;
    ```
- set:
    ```C++
    auto show_c_string(bool value) -> Config&;
    ```

The code:

```C++
char const* flavor = "mango";

IC_CONFIG.show_c_string(true);
IC(flavor);

IC_CONFIG.show_c_string(false);
IC(flavor);
```

will print:

```
ic| flavor: "mango";
ic| flavor: 0x55587b6f5410
```

### decay_char_array

Controls if a character array variable (`char[N]`, `wchar_t[N]`, `char8_t[N]`,
`char16_t[N]`, or `char32_t[N]`) should decay to a character pointer (when `true`) to be
printed by the [*strings strategy*](#strings) (subject to the
[`show_c_string`](#show_c_string) configuration), or remain as an array (when `false`) to
be printed by the [*range types*](#range-types) strategy.

The default value is `false`.

- get:
    ```C++
    auto decay_char_array() const -> bool;
    ```
- set:
    ```C++
    auto decay_char_array(bool value) -> Config&;
    ```

The code:

```C++
char flavor[] = "caju";

IC_CONFIG.decay_char_array(true);
IC(flavor);

IC_CONFIG.decay_char_array(false);
IC(flavor);
```

will print:

```
ic| flavor: "caju";
ic| flavor: ['c', 'a', 'j', 'u', '\u{0}']
```

### force_range_strategy

Controls if a range type `T` will be printed using the [range type](#range-types) strategy
even when the [Formatting](https://en.cppreference.com/w/cpp/utility/format) or
[{fmt}](https://fmt.dev) libraries would be able to print it. The *range type* strategy
supports more useful formatting options than the *baseline strategies*.

This option has a default value of `true`.

- get:
    ```C++
    auto force_range_strategy() const -> bool;
    ```
- set:
    ```C++
    auto force_range_strategy(bool value) -> Config&;
    ```

### force_tuple_strategy

Controls if a tuple-like type `T` will be printed using the [tuple-like
types](#tuple-like-types) strategy even when the
[Formatting](https://en.cppreference.com/w/cpp/utility/format) or [{fmt}](https://fmt.dev)
libraries would be able to print it. The *tuple-like types* strategy supports more useful
formatting options than the *baseline strategies*.

This option has a default value of `true`.

- get:
    ```C++
    auto force_tuple_strategy() const -> bool;
    ```
- set:
    ```C++
    auto force_tuple_strategy(bool value) -> Config&;
    ```

### force_variant_strategy

Controls if a variant type `T`
([`std::variant`](https://en.cppreference.com/w/cpp/utility/variant) or
[`boost::variant2::variant`](https://www.boost.org/doc/libs/release/libs/variant2/doc/html/variant2.html))
will be printed using the [variant types](#variant-types) strategy even when some of the
*baseline strategies* would be able to print it. The *variant types* strategy supports
more useful formatting options than the *baseline strategies*.

This option has a default value of `true`.

- get:
    ```C++
    auto force_variant_strategy() const -> bool;
    ```
- set:
    ```C++
    auto force_variant_strategy(bool value) -> Config&;
    ```

### wide_string_transcoder

Function that transcodes a `wchar_t` string, from a system defined encoding to a `char`
string in the system *execution encoding*.

- set:
    ```C++
    auto wide_string_transcoder(std::function<std::string(wchar_t const*, std::size_t)> transcoder) -> Config&;
    auto wide_string_transcoder(std::function<std::string(std::wstring_view)> transcoder) -> Config&;
    ```

There is no guarantee that the input string will end in a null terminator (this is the
actual semantic of a
[`string_view`](https://en.cppreference.com/w/cpp/string/basic_string_view)), so the user
must observe the input string size value.

The default implementation checks if the C locale is set to a value other than "C" or
"POSIX". If so, it forwards the input to
[std::wcrtomb](https://en.cppreference.com/w/cpp/string/multibyte/wcrtomb). Otherwise, it
assumes the input is Unicode encoded (UTF-16 or UTF-32, depending on `wchar_t` size) and
transcodes it to UTF-8.

### unicode_transcoder

Function that transcodes a `char32_t` string, from a UTF-32 encoding to a `char` string in
the system *execution encoding*.

- set:
    ```C++
    auto unicode_transcoder(std::function<std::string(char32_t const*, std::size_t)> transcoder) -> Config&;
    auto unicode_transcoder(std::function<std::string(std::u32string_view)> transcoder) -> Config&;
    ```

There is no guarantee that the input string will end on a null terminator (this is the
actual semantic of a
[`string_view`](https://en.cppreference.com/w/cpp/string/basic_string_view)), so the user
must observe the input string size value.

The default implementation checks if the C locale is set to a value other than "C" or
"POSIX". If so, it forwards the input to
[std::c32rtomb](https://en.cppreference.com/w/cpp/string/multibyte/c32rtomb). Otherwise,
it just transcodes to UTF-8.

This function will be used to transcode all the `char8_t`, `char16_t`, and `char32_t`
strings.  When transcoding `char8_t` and `char16_t` strings, they will be first converted
to a `char32_t` string, before being sent as input to this function.

### output_transcoder

Function that transcodes a `char` string, from the system *execution encoding* to a `char`
string in the system *output encoding*, as expected by the configured [`output`](#output).

- set:
    ```C++
    auto output_transcoder(std::function<std::string(char const*, std::size_t)> transcoder) -> Config&;
    auto output_transcoder(std::function<std::string(std::string_view)> transcoder) -> Config&;
    ```

There is no guarantee that the input string will end on a null terminator (this is the
actual semantic of a
[`string_view`](https://en.cppreference.com/w/cpp/string/basic_string_view)), so the user
must observe the input string size value.

The default implementation assumes that the *execution encoding* is the same as the
*output encoding*, and will return an unchanged input.

### line_wrap_width

The maximum number of characters before the output is broken into multiple lines. Default
value of `70`.

- get:
    ```C++
    auto line_wrap_width() const -> std::size_t;
    ```
- set:
    ```C++
    auto line_wrap_width(std::size_t value) -> Config&;
    ```

### include_context

If the context (source name, line number, and function name) should be printed even when
printing variables. Default value is `false`.

- get:
    ```C++
    auto include_context() const -> bool;
    ```
- set:
    ```C++
    auto include_context(bool value) -> Config&;
    ```

### context_delimiter

The string separating the context text from the variables values. Default value is `"- "`.

- get:
    ```C++
    auto context_delimiter() const -> std::string;
    ```
- set:
    ```C++
    auto context_delimiter(std::string const& value) -> Config&;
    ```


## Printing Strategies

In order to be printable, a type `T` must satisfy at least one of the strategies described
in the next sections. If a type `T` is not printable, it can be mended by adding support
to one of the *baseline strategies*: [IOStreams](#iostreams), [Formatting
library](#formatting-library), and [{fmt}](#fmt-1).

If it happens that multiple strategies are simultaneously satisfied, the one with the
higher precedence will be chosen. Within the *baseline strategies*, the precedence order
is: *{fmt}*, *Formatting library*, and *IOStreams*. The precedence criteria other than
these are discussed in each strategy section.

**Strategies:**
[IOStreams](#iostreams) |
[Formatting library](#formatting-library) |
[{fmt}](#fmt-1) |
[Characters](#characters) |
[Strings](#strings) |
[Pointer-like types](#pointer-like-types) |
[Range types](#range-types) |
[Tuple-like types](#tuple-like-types) |
[Optional types](#optional-types) |
[Variant types](#variant-types) |
[Exception types](#exception-types) |
[Clang dump struct](#clang-dump-struct)


### IOStreams

Uses the [STL IOStream](https://en.cppreference.com/w/cpp/io) library to print values. A
type `T` is eligible for this strategy if there exists a function overload
[`operator<<(ostream&,
<SOME_TYPE>)`](https://en.cppreference.com/w/cpp/io/basic_ostream/operator_ltlt), where a
value of type `T` is accepted as `<SOME_TYPE>`.

Within the *baseline strategies*, IOStreams has the lowest precedence. So if a type is
supported either by [{fmt}](#fmt-1) or [Formatting library](#formatting-library) they will
be used instead.

#### IOStreams format string

The format string of IOStreams strategy is strongly based on the ones of [STL
Formatting](https://en.cppreference.com/w/cpp/utility/format/spec) and
[{fmt}](https://fmt.dev/11.0/syntax/#format-specification-mini-language).

It has the following specification:

```
format_spec ::=  [[fill]align][sign]["#"][width]["." precision][type]
fill        ::=  <a character>
align       ::=  "<" | ">" | "^"
sign        ::=  "+" | "-"
width       ::=  integer
precision   ::=  integer
type        ::=  "a" | "A" | "b" | "B" | "c" | "d" | "e" | "E" | "f" | "F" | "g" | "G" | "o" | "s" | "x" | "X" | "?"
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
| `'^'`  | Internally align the data, with the fill character being placed between the digits and either the base or sign. Applies to integer and floating-point. |

Note that unless a minimum field width is defined, the field width will always be the same
size as the data to fill it, so that the alignment option has no meaning in this case.

##### [sign]

The sign option is only valid for number types, and can be one of the following:

| Symbol | Meaning                                                                 |
|--------|-------------------------------------------------------------------------|
| `'+'`  | A sign will be used for both nonnegative as well as negative numbers. |
| `'-'`  | A sign will be used only for negative numbers. This is the default.   |

##### ["#"]

Causes the "alternate form" to be used for the conversion. The alternate form is defined
differently for different types. This option is only valid for integer and floating-point
types. For integers, when binary, octal, or hexadecimal output is used, this option adds
the respective prefix "0b" ("0B"), "0", or "0x" ("0X") to the output value. Whether the
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

The available string presentation types are:

| Symbol | Meaning                                                            |
|--------|--------------------------------------------------------------------|
| `'s'`  | String format.                                                     |
| `'?'`  | Debug format. The string is quoted and special characters escaped. |
| none   | The same as `'?'`.                                                 |

The available character presentation types are:

| Symbol | Meaning                                                               |
|--------|-----------------------------------------------------------------------|
| `'c'`  | Character format.                                                     |
| `'?'`  | Debug format. The character is quoted and special characters escaped. |
| none   | The same as `'?'`.                                                    |

The available integer presentation types are:

| Symbol | Meaning                                                                                                                                                                   |
|--------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `'b'`  | Binary format. Outputs the number in base 2. Using the '#' option with this type adds the prefix "0b" to the output value.                                                |
| `'B'`  | Binary format. Outputs the number in base 2. Using the '#' option with this type adds the prefix "0B" to the output value.                                                |
| `'c'`  | Character format. Outputs the number as a character.                                                                                                                      |
| `'d'`  | Decimal integer. Outputs the number in base 10.                                                                                                                           |
| `'o'`  | Octal format. Outputs the number in base 8.                                                                                                                               |
| `'x'`  | Hex format. Outputs the number in base 16, using lower-case letters for the digits above 9. Using the '#' option with this type adds the prefix "0x" to the output value. |
| `'X'`  | Hex format. Outputs the number in base 16, using upper-case letters for the digits above 9. Using the '#' option with this type adds the prefix "0X" to the output value. |

Integer presentation types can also be used with character and boolean values with the
only exception that 'c' cannot be used with bool. Boolean values are formatted using
textual representation, either true or false, if the presentation type is not specified.

The available presentation types for floating-point values are:

| Symbol | Meaning                                                                                                                                                                                                                                                                     |
|--------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `'a'`  | Hexadecimal floating point format. Prints the number in base 16 with prefix "0x" and lower-case letters for digits above 9. Uses 'p' to indicate the exponent.                                                                                                              |
| `'A'`  | Same as 'a' except it uses upper-case letters for the prefix, digits above 9 and to indicate the exponent.                                                                                                                                                                  |
| `'e'`  | Exponent notation. Prints the number in scientific notation using the letter 'e' to indicate the exponent.                                                                                                                                                                  |
| `'E'`  | Exponent notation. Same as 'e' except it uses an upper-case 'E' as the separator character.                                                                                                                                                                                 |
| `'f'`  | Fixed point. Displays the number as a fixed-point number.                                                                                                                                                                                                                   |
| `'F'`  | Fixed point. Same as 'f', but converts nan to NAN and inf to INF.                                                                                                                                                                                                           |
| `'g'`  | General format. For a given precision p >= 1, this rounds the number to p significant digits and then formats the result in either fixed-point format or in scientific notation, depending on its magnitude. A precision of 0 is treated as equivalent to a precision of 1. |
| `'G'`  | General format. Same as 'g' except switches to 'E' if the number gets too large. The representations of infinity and NaN are uppercased, too.                                                                                                                               |


### Formatting library

Uses the [STL formatting](https://en.cppreference.com/w/cpp/utility/format) library to
print values. A type `T` is eligible for this strategy if there exists a struct
specialization
[`std::formatter<T>`](https://en.cppreference.com/w/cpp/utility/format/formatter).

Within the *baseline strategies* the [{fmt}](#fmt-1) has precedence over [Formatting
library](#formatting-library), so if a type is supported by both, the *{fmt}* will be used
instead.

#### Formatting library format string

The format string is forwarded to the *STL Formatting* library. Its syntax can be checked
[here](https://en.cppreference.com/w/cpp/utility/format/spec).


### {fmt}

Uses the [{fmt}](https://fmt.dev) library to print values. A type `T` is eligible for this
strategy if there exists either a struct specialization
[`fmt::formatter<T>`](https://fmt.dev/latest/api/#formatting-user-defined-types) or a
function overload [`auto
format_as(T)`](https://fmt.dev/latest/api/#formatting-user-defined-types).

{fmt} is a third-party library, so it needs to be available on the system and enabled to
be supported by Icecream-cpp. An explanation of this is in the [*third-party
libraries*](#third-party-libraries) section.

#### {fmt} format string

The format string is forwarded to the *{fmt}* library. Its syntax can be checked
[here](https://fmt.dev/11.0/syntax/#format-specification-mini-language).


### Characters

All character types: `char`, `wchar_t`, `char8_t`, `char16_t`, and `char32_t`. This
strategy [transcodes](#character-encoding) non-`char` character types to `char`, using
either [`wide_string_transcoder`](#wide_string_transcoder) or
[`unicode_transcoder`](#unicode_transcoder) as appropriate, then delegates the actual
printing to the [IOStreams](#iostreams) strategy.

This strategy has higher precedence than all the *baseline strategies*.

#### Characters format string

The same as the [IOStreams format string](#iostreams-format-string).

### Strings

C strings ([with some subtleties](#c-strings)), [STL's
strings](https://en.cppreference.com/w/cpp/string/basic_string), and [STL's
string_views](https://en.cppreference.com/w/cpp/string/basic_string_view). These three
classes instantiated to all character types: `char`, `wchar_t`, `char8_t`, `char16_t`, and
`char32_t`.

This strategy [transcodes](#character-encoding) non-`char` character types to `char`,
using either [`wide_string_transcoder`](#wide_string_transcoder) or
[`unicode_transcoder`](#unicode_transcoder) as appropriate, then delegates the actual
printing to the [IOStreams](#iostreams) strategy.

This strategy has higher precedence than all the *baseline strategies*.

#### Strings format string

The same as the [IOStreams format string](#iostreams-format-string).

### Pointer-like types

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

This strategy has lower precedence than the *baseline strategies*. If the type is
supported by any of them, that strategy will be used instead.

### Range types

Concisely, a range is any object of a type `R` that holds a collection of elements and is
able to provide a [`begin`, `end`) pair, where `begin` is an iterator of type `I` and
`end` is a sentinel of type `S`. The iterator `I` is used to traverse the elements of `R`,
and the sentinel `S` is used to signal the end of the range interval, it may or may not be
the same type as `I`. In precise terms, the Icecream-cpp library is able to format a range
type `R` if it fulfills the
[`forward_range`](https://en.cppreference.com/w/cpp/ranges/forward_range) concept.

If a type `R` fulfills the range requirements and its elements are *formattable* by
IceCream, the type `R` is formattable by the range types strategy.

The code:

```C++
auto v0 = std::list<int>{10, 20, 30};
IC(v0);
```

will print:

    ic| v0: [10, 20, 30]

A `view` is a closely related concept. Refer to the [*range views
pipeline*](#range-views-pipeline) section to see how to print them.

This strategy has a higher precedence than the *baseline strategies*, so if the printing
of a type is supported by both, this strategy will be used instead. This precedence can be
disabled by the [force_range_strategy](#force_range_strategy) configuration.

#### Range format string

The accepted formatting string to a range type is a combination of both a range formatting
and its elements formatting. The range formatting is syntactically and semantically almost
identical to the [Python
slicing](https://docs.python.org/3/reference/expressions.html#slicings).

Formally, the accepted range types formatting string is:

```
format_spec  ::=  [range_fmt][":"elements_fmt]
range_fmt    ::=  "[" slicing | index "]"
slicing      ::=  [lower_bound] ":" [upper_bound] [ ":" [stride] ]
lower_bound  ::=  integer
upper_bound  ::=  integer
stride       ::=  integer
index        ::=  integer
integer      ::=  ["-"]digit+
digit        ::=  "0"..."9"
```

The same `elements_fmt` string will be used by all the printing elements, so it will have
the same syntax as the formatting string of the range elements.

The code:

```C++
auto arr = std::vector<int>{10, 11, 12, 13, 14, 15};
IC_F("[:2:-1]:#x", arr);
```
will print:

    ic| arr: [:2:-1]->[0xf, 0xe, 0xd]

Even though the specification says that `lower_bound`, `upper_bound`, `stride`, and
`index`, can have any integer value, some `range` capabilities can restrict them to just
positive values.

If a `range` is not [`sized`](https://en.cppreference.com/w/cpp/ranges/sized_range), the
`lower_bound`, `upper_bound`, and `index` values must be positive. Similarly, if a `range`
is not [`bidirectional`](https://en.cppreference.com/w/cpp/ranges/bidirectional_range) the
`stride` value must be positive too.

When printing within a [range views pipeline](#range-views-pipeline) using the `IC_FV`
function, all the `lower_bound`, `upper_bound`, and `index` values must be positive.


### Tuple-like types

`std::pair<T1, T2>` and `std::tuple<Ts...>` variables will print all of their elements.

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

This strategy has higher precedence than the *baseline strategies*, so if a type is
supported by both, this strategy will be used instead. This precedence can be disabled by
the [`force_tuple_strategy`](#force_tuple_strategy) configuration.

#### Tuple-like format string

The tuple-like formatting specification is based on the syntax suggested in the
[Formatting
Ranges](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2022/p2286r8.html#pair-and-tuple-specifiers)
paper. Since that part hasn't made it into the standard, we may revisit it with any future
changes.

```
tuple_spec  ::= [casing][content]
casing      ::= "n" | "m"
content     ::= (delimiter element_fmt){N}
delimiter   ::= <a character, the same to all N expansions>
element_fmt ::= <format specification of the element type>
```

Where the number `N` of repetitions in `content` rule is the tuple size.

The code:

```C++
auto v0 = std::make_tuple(20, "foo", 0.0123);
IC_F("n|#x||.3e", v0);
```

will print:

```
ic| v0: 0x14, "foo", 1.230e-02
```

##### casing

Controls the tuple enclosing characters and separator. If not used, the default behavior
is to enclose the values between `"("` and `")"`, and separated by `", "`.

If `n` is used, the tuple won't be enclosed by parentheses. If `m` is used the tuple will
be printed "map value like", i.e.: not enclosed by parentheses and using `": "` as
separator. The `m` specifier is valid only for a *pair* or *2-tuple*.

##### content

The formatting string of each tuple element, separated by a `delimiter` character. This
can be any character, whose value will be defined by the first `char` read when parsing
this rule.

Each `element_fmt` string will be forwarded to the respective tuple element when printing
it, so it must follow the formatting specification of that particular element type.


### Optional types

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

This strategy has lower precedence than the *baseline strategies*. If the type is
supported by any of them, that strategy will be used instead.

#### Optional types format string

```
optional_spec ::= [":"element_fmt]
element_fmt   ::= <format specification of the element type>
```

```C++
auto v0 = std::optional<int>{50};
IC_F(":#x", v0);
```

will print:

```
ic| v0: 0x32
```

### Variant types

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

This strategy has higher precedence than the *baseline strategies*, so if a type is
supported by both, this strategy will be used instead. This precedence can be disabled by
the [force_variant_strategy](#force_variant_strategy) configuration.

#### Variant types format string

```
variant_spec ::= [content]
content      ::= (delimiter element_fmt){N}
delimiter    ::= <a character, the same to all N expansions>
element_fmt  ::= <format specification of the element type>
```

Where the number `N` of repetitions in `content` rule is the number of types in the
variant.

The code:

```C++
auto v0 = std::variant<int, char const*>{50};
IC_F("|b|s", v0);
```

will print:

```
ic| v0: 110010
```


### Exception types

Types inheriting from `std::exception` will print the return of `std::exception::what()`.
If the type also inherits from `boost::exception`, the output of
`boost::diagnostic_information()` will also be printed.

The code:

```C++
auto v0 = std::runtime_error("error description");
IC(v0);
```

will print:

```
ic| v0: error description
```

This strategy has lower precedence than the *baseline strategies*. If the type is
supported by any of them, that strategy will be used instead.

### Clang dump struct

With Clang >= 15, a class will be printable even without support from any of the
*baseline strategies*.

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

This strategy has the lowest precedence of all printing strategies. If the type is
supported by any other strategy, that strategy will be used instead.


## Third-party Libraries

Icecream-cpp has no required dependencies beyond the C++ standard library. However, it
optionally supports printing some types from
[Boost](https://www.boost.org/) and [Range-v3](https://ericniebler.github.io/range-v3/),
as well as using the [{fmt}](https://fmt.dev) library alongside the STL's
[IOStreams](https://en.cppreference.com/w/cpp/io#Stream-based_I.2FO) and
[formatting](https://en.cppreference.com/w/cpp/utility/format) libraries.

None of these external libraries are necessary for Icecream-cpp to work properly, and no
action is required if any of them are not available.

### Boost

All supported Boost types are forward declared in the Icecream-cpp header, so you can
print them with no further work, just like STL types.


### Range-v3

Icecream-cpp can optionally support printing Range-v3 views at any point in a pipeline.
This functionality is fully described in the [Range views pipeline](#range-views-pipeline)
section.

Support for printing Range-v3 types can be explicitly enabled by defining the
`ICECREAM_RANGE_V3` macro before including `icecream.hpp`:

```C++
#define ICECREAM_RANGE_V3
#include "icecream.hpp"
```

or by using a compiler command line argument if available. In GCC for example:

```Shell
gcc -DICECREAM_RANGE_V3 source.cpp
```

Even without explicitly defining `ICECREAM_RANGE_V3`, if `icecream.hpp` is included after
any Range-v3 header, support will be automatically enabled:

```C++
#include <range/v3/view/transform.hpp>
#include "icecream.hpp"
```

### {fmt}

Icecream-cpp can optionally use the {fmt} library to get the string representation of a
type. When available, {fmt} takes precedence over the STL's formatting and IOStreams
libraries. See the [Printing Strategies](#printing-strategies) section for details.

Support for the {fmt} library can be explicitly enabled by defining the `ICECREAM_FMT`
macro before including `icecream.hpp`:

```C++
#define ICECREAM_FMT
#include "icecream.hpp"
```

or by using a compiler command line argument if available. In GCC for example:

```Shell
gcc -DICECREAM_FMT source.cpp
```

Even without explicitly defining `ICECREAM_FMT`, if `icecream.hpp` is included after any
{fmt} header, support will be automatically enabled:

```C++
#include <fmt/format.h>
#include "icecream.hpp"
```


## Compatibility

Icecream-cpp is tested via CI on the following platforms and compilers:

| Platform | Compilers                                  | C++ Standards       |
|----------|--------------------------------------------|---------------------|
| Linux    | GCC 5.4, 9, 14; Clang 8, 19                | 11, 14, 17, 20, 23  |
| Windows  | MSVC 17.14                                 | 14, 17, 20, 23      |
| Windows  | MinGW GCC and Clang                        | 11, 14, 17, 20, 23  |
| macOS    | Apple Clang (Xcode)                        | 11, 14, 17, 20, 23  |

Older compilers (GCC 5.4, GCC 9, Clang 8) are excluded from C++20 and C++23 tests since they
predate those standards.


## Reference

This section covers special topics and edge cases.

### C strings

C strings are ambiguous. Should a `char* foo` variable be interpreted as a pointer to a
single `char` or as a null-terminated string? Likewise, is the `char bar[]` variable an
array of single characters or a null-terminated string? Is `char baz[3]` an array with
three single characters or is it a string of size two plus a `'\0'`?

Each interpretation of `foo`, `bar`, and `baz` would be printed differently. For the
code:

```C++
char flavor[] = "pistachio";
IC(flavor);
```

all three outputs below are valid, each reflecting a different interpretation of
`flavor`.

```
ic| flavor: 0x55587b6f5410
ic| flavor: ['p', 'i', 's', 't', 'a', 'c', 'h', 'i', 'o', '\u{0}']
ic| flavor: "pistachio"
```

A bounded `char` array (i.e., an array with a known size) will either be interpreted as an
array of single characters or be allowed to decay to a `char*`, depending on the
[`decay_char_array`](#decay_char_array) option.


Unbounded `char[]` arrays (i.e., arrays with an unknown size) will decay to `char*`
pointers, and a character pointer will be printed either as a string or as a pointer,
depending on the [`show_c_string`](#show_c_string) option.

The exact same logic as above applies to C strings of all character types, namely `char`,
`wchar_t`, `char8_t`, `char16_t`, and `char32_t`.


### Character Encoding

Character encoding in C++ is messy.

The `char8_t`, `char16_t`, and `char32_t` strings are well defined. They hold Unicode code
units of 8, 16, and 32 bits respectively, encoded in UTF-8, UTF-16, and UTF-32.

The `char` strings have a well defined code unit bit size (given by
[`CHAR_BIT`](https://en.cppreference.com/w/cpp/types/climits), usually 8 bits), but there
are no requirements about their encoding.

The `wchar_t` strings have [neither a well defined code unit
size](https://en.cppreference.com/w/cpp/language/types#Character_types), nor any
requirements about their encoding.

In a code like this:

```C++
auto const str = std::string{"foo"};
std::cout << str;
```

There are three character encoding points of interest. First, before compiling, the code
is written in a source file in an unspecified *source encoding*. Second, the compiled
binary stores the "foo" string in an unspecified *execution encoding*. Third, the "foo"
byte stream sent to `std::cout` is forwarded to the system, which expects it to be encoded
in an unspecified *output encoding*.

Of these three encoding points, both *execution encoding* and *output encoding* affect
Icecream-cpp's behavior, and there is no way to know for certain what encoding is used.
In the face of this uncertainty, the adopted strategy is to offer a reasonable default
transcoding function that tries to convert the data to the right encoding, while allowing
users to provide their own implementation when needed.

Except for wide and Unicode string types (discussed below), when printing other types
their serialized textual data is in *execution encoding*. This may or may not match the
*output encoding* expected by the configured [output](#output). Therefore, before sending
data to the output, it must be transcoded to ensure it is in *output encoding*. This is
done by sending the text data to the configured
[`output_transcoder`](#output_transcoder) function, which must ensure the correct encoding.

When printing the wide and Unicode string types, we need to have one more transcoding
level, because it is possible that the text data is in a distinct character encoding from
the expected *execution encoding*. Because of that, additional logic is applied to make
sure that the strings are in *execution encoding* before we send them to output. That is
done by applying the [`wide_string_transcoder`](#wide_string_transcoder) and
[`unicode_transcoder`](#unicode_transcoder) functions respectively.

### Macro disabling

All Icecream-cpp printing will be disabled in a translation unit if the
`ICECREAM_DISABLE` macro is defined before including `icecream.hpp`.

This can be done either by defining it in the source code:

```C++
#define ICECREAM_DISABLE
#include <icecream.hpp>
```

or as an argument to the compiler. A `-DICECREAM_DISABLE` with
[GCC](https://gcc.gnu.org/onlinedocs/gcc/Preprocessor-Options.html#index-D-1) and
[Clang](https://clang.llvm.org/docs/ClangCommandLineReference.html#cmdoption-clang-D-macro),
and a `/DICECREAM_DISABLE` in
[MSVC](https://learn.microsoft.com/en-us/cpp/build/reference/d-preprocessor-definitions)
for example.

This will disable only the printing output, all other functionalities will still work. In
particular, all the changes to the global [`IC_CONFIG`](#configuration) will be effective.


## Pitfalls

`IC(...)` is a preprocessor macro and can cause conflicts if there is another `IC`
identifier in the code. To use the longer `ICECREAM(...)` macro instead, define
`ICECREAM_LONG_NAME` before including `icecream.hpp`:

```C++
#define ICECREAM_LONG_NAME
#include "icecream.hpp"
```

While most compilers will work just fine, until C++20 the standard requires at least one
argument when calling a variadic macro. To handle this the nullary macros `IC0()` and
`ICECREAM0()` are defined alongside `IC(...)` and `ICECREAM(...)`. For the same reason the
pair `IC_A0(callable)` and `ICECREAM_A0(callable)` is defined alongside `IC_A(...)` and
`ICECREAM_A(...)`.


[CI.badge]: https://github.com/renatoGarcia/icecream-cpp/workflows/CI/badge.svg
[CI.link]: https://github.com/renatoGarcia/icecream-cpp/actions?query=workflow%3ACI

[LICENSE.badge]: https://img.shields.io/badge/licence-MIT-blue
[LICENSE.link]: https://raw.githubusercontent.com/renatoGarcia/icecream-cpp/master/LICENSE.txt
