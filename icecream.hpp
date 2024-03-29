/*
 * Copyright (c) 2019-2020 The IceCream-Cpp Developers. See the AUTHORS file at the
 * top-level directory of this distribution and at
 * https://github.com/renatoGarcia/icecream-cpp
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef ICECREAM_HPP_INCLUDED
#define ICECREAM_HPP_INCLUDED

#include <cassert>
#include <cctype>
#include <codecvt>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <exception>
#include <functional>
#include <iomanip>
#include <ios>
#include <iostream>
#include <iterator>
#include <locale>
#include <memory>
#include <mutex>
#include <ostream>
#include <sstream>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <valarray>
#include <vector>

#if defined(__cpp_lib_optional) || (__cplusplus >= 201703L)
    #define ICECREAM_OPTIONAL_HEADER
    #include <optional>
#endif

#if defined(__cpp_lib_variant) || (__cplusplus >= 201703L)
    #define ICECREAM_VARIANT_HEADER
    #include <variant>
#endif

#if defined(__cpp_lib_string_view) || (__cplusplus >= 201703L)
    #define ICECREAM_STRING_VIEW_HEADER
    #include <string_view>
#endif

#if defined(__has_builtin) && defined(__clang__)
    #if __has_builtin(__builtin_dump_struct) && __clang_major__ >= 15
        #define ICECREAM_DUMP_STRUCT_CLANG
    #endif
#endif

#if defined(__cpp_if_constexpr)
    #define ICECREAM_IF_CONSTEXPR constexpr
#else
    #define ICECREAM_IF_CONSTEXPR
#endif

#define ICECREAM_DEV_HASH "$Format:%H$"

#if defined(__GNUC__)
    #define ICECREAM_FUNCTION __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
    #define ICECREAM_FUNCTION __FUNCSIG__
#else
    #define ICECREAM_FUNCTION __func__
#endif

// Used to force MSVC to unpack __VA_ARGS__
#define ICECREAM_EXPAND(X) X

// __VA_ARGS__ will have the callable name and the args. Returns the quantity of args.
#define ICECREAM_ARGS_SIZE(...) ICECREAM_EXPAND(ICECREAM_ARGS_SIZE_(__VA_ARGS__, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))
#define ICECREAM_ARGS_SIZE_(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, N, ...) N

#define ICECREAM_UNPACK_0(F, ICM, ...) [&](){ return F(
#define ICECREAM_UNPACK_1(F, ICM, ...) [&](){ auto&& ret_value = ICECREAM_EXPAND(ICM(__VA_ARGS__)); auto&& ret_tuple = ::icecream::detail::ensure_tuple(std::move(ret_value)); return F(std::get<0>(std::move(ret_tuple))
#define ICECREAM_UNPACK_2(F, ICM, ...) ICECREAM_UNPACK_1(F, ICM, __VA_ARGS__), std::get<1>(std::move(ret_tuple))
#define ICECREAM_UNPACK_3(F, ICM, ...) ICECREAM_UNPACK_2(F, ICM, __VA_ARGS__), std::get<2>(std::move(ret_tuple))
#define ICECREAM_UNPACK_4(F, ICM, ...) ICECREAM_UNPACK_3(F, ICM, __VA_ARGS__), std::get<3>(std::move(ret_tuple))
#define ICECREAM_UNPACK_5(F, ICM, ...) ICECREAM_UNPACK_4(F, ICM, __VA_ARGS__), std::get<4>(std::move(ret_tuple))
#define ICECREAM_UNPACK_6(F, ICM, ...) ICECREAM_UNPACK_5(F, ICM, __VA_ARGS__), std::get<5>(std::move(ret_tuple))
#define ICECREAM_UNPACK_7(F, ICM, ...) ICECREAM_UNPACK_6(F, ICM, __VA_ARGS__), std::get<6>(std::move(ret_tuple))
#define ICECREAM_UNPACK_8(F, ICM, ...) ICECREAM_UNPACK_7(F, ICM, __VA_ARGS__), std::get<7>(std::move(ret_tuple))
#define ICECREAM_UNPACK_9(F, ICM, ...) ICECREAM_UNPACK_8(F, ICM, __VA_ARGS__), std::get<8>(std::move(ret_tuple))
#define ICECREAM_UNPACK_10(F, ICM, ...) ICECREAM_UNPACK_9(F, ICM, __VA_ARGS__), std::get<9>(std::move(ret_tuple))
#define ICECREAM_UNPACK_11(F, ICM, ...) ICECREAM_UNPACK_10(F, ICM, __VA_ARGS__), std::get<10>(std::move(ret_tuple))
#define ICECREAM_UNPACK_12(F, ICM, ...) ICECREAM_UNPACK_11(F, ICM, __VA_ARGS__), std::get<11>(std::move(ret_tuple))
#define ICECREAM_UNPACK_13(F, ICM, ...) ICECREAM_UNPACK_12(F, ICM, __VA_ARGS__), std::get<12>(std::move(ret_tuple))
#define ICECREAM_UNPACK_14(F, ICM, ...) ICECREAM_UNPACK_13(F, ICM, __VA_ARGS__), std::get<13>(std::move(ret_tuple))
#define ICECREAM_UNPACK_15(F, ICM, ...) ICECREAM_UNPACK_14(F, ICM, __VA_ARGS__), std::get<14>(std::move(ret_tuple))
#define ICECREAM_UNPACK_16(F, ICM, ...) ICECREAM_UNPACK_15(F, ICM, __VA_ARGS__), std::get<15>(std::move(ret_tuple))
#define ICECREAM_UNPACK_17(F, ICM, ...) ICECREAM_UNPACK_16(F, ICM, __VA_ARGS__), std::get<16>(std::move(ret_tuple))
#define ICECREAM_UNPACK_18(F, ICM, ...) ICECREAM_UNPACK_17(F, ICM, __VA_ARGS__), std::get<17>(std::move(ret_tuple))
#define ICECREAM_UNPACK_19(F, ICM, ...) ICECREAM_UNPACK_18(F, ICM, __VA_ARGS__), std::get<18>(std::move(ret_tuple))
#define ICECREAM_UNPACK_20(F, ICM, ...) ICECREAM_UNPACK_19(F, ICM, __VA_ARGS__), std::get<19>(std::move(ret_tuple))
#define ICECREAM_UNPACK_21(F, ICM, ...) ICECREAM_UNPACK_20(F, ICM, __VA_ARGS__), std::get<20>(std::move(ret_tuple))
#define ICECREAM_UNPACK_22(F, ICM, ...) ICECREAM_UNPACK_21(F, ICM, __VA_ARGS__), std::get<21>(std::move(ret_tuple))
#define ICECREAM_UNPACK_23(F, ICM, ...) ICECREAM_UNPACK_22(F, ICM, __VA_ARGS__), std::get<22>(std::move(ret_tuple))
#define ICECREAM_UNPACK_24(F, ICM, ...) ICECREAM_UNPACK_23(F, ICM, __VA_ARGS__), std::get<23>(std::move(ret_tuple))
#define ICECREAM_UNPACK_25(F, ICM, ...) ICECREAM_UNPACK_24(F, ICM, __VA_ARGS__), std::get<24>(std::move(ret_tuple))
#define ICECREAM_UNPACK_26(F, ICM, ...) ICECREAM_UNPACK_25(F, ICM, __VA_ARGS__), std::get<25>(std::move(ret_tuple))
#define ICECREAM_UNPACK_27(F, ICM, ...) ICECREAM_UNPACK_26(F, ICM, __VA_ARGS__), std::get<26>(std::move(ret_tuple))
#define ICECREAM_UNPACK_28(F, ICM, ...) ICECREAM_UNPACK_27(F, ICM, __VA_ARGS__), std::get<27>(std::move(ret_tuple))
#define ICECREAM_UNPACK_29(F, ICM, ...) ICECREAM_UNPACK_28(F, ICM, __VA_ARGS__), std::get<28>(std::move(ret_tuple))
#define ICECREAM_UNPACK_30(F, ICM, ...) ICECREAM_UNPACK_29(F, ICM, __VA_ARGS__), std::get<29>(std::move(ret_tuple))
#define ICECREAM_UNPACK_31(F, ICM, ...) ICECREAM_UNPACK_30(F, ICM, __VA_ARGS__), std::get<30>(std::move(ret_tuple))
#define ICECREAM_UNPACK_32(F, ICM, ...) ICECREAM_UNPACK_31(F, ICM, __VA_ARGS__), std::get<31>(std::move(ret_tuple))

#define ICECREAM_APPLY_(F, ICM, N, ...) ICECREAM_UNPACK_##N(F, ICM, __VA_ARGS__) );}()
#define ICECREAM_APPLY(ICM, N, F, ...) ICECREAM_APPLY_(F, ICM, N, __VA_ARGS__)
#define ICECREAM_APPLY_S(ICM, N, S, F, ...) ICECREAM_APPLY_(F, ICM, N, S, __VA_ARGS__)

#if defined(ICECREAM_LONG_NAME)
    #define ICECREAM(...) ICECREAM_("", __VA_ARGS__)
    #define ICECREAM0() ::icecream::detail::Dispatcher{__FILE__, __LINE__, ICECREAM_FUNCTION, "", ""}.ret()
    #define ICECREAM_(S, ...) ::icecream::detail::Dispatcher{__FILE__, __LINE__, ICECREAM_FUNCTION, S, #__VA_ARGS__}.ret(__VA_ARGS__)
    #define ICECREAM_A(...) ICECREAM_EXPAND(ICECREAM_APPLY(ICECREAM, ICECREAM_ARGS_SIZE(__VA_ARGS__), __VA_ARGS__))
    #define ICECREAM_A_(S, ...) ICECREAM_EXPAND(ICECREAM_APPLY(ICECREAM_, ICECREAM_ARGS_SIZE(__VA_ARGS__), S, __VA_ARGS__))
#else
    #define IC(...) IC_("", __VA_ARGS__)
    #define IC0() ::icecream::detail::Dispatcher{__FILE__, __LINE__, ICECREAM_FUNCTION, "",  ""}.ret()
    #define IC_(S, ...) ::icecream::detail::Dispatcher{__FILE__, __LINE__, ICECREAM_FUNCTION, S, #__VA_ARGS__}.ret(__VA_ARGS__)
    #define IC_A(...) ICECREAM_EXPAND(ICECREAM_APPLY(IC, ICECREAM_ARGS_SIZE(__VA_ARGS__), __VA_ARGS__))
    #define IC_A_(S, ...) ICECREAM_EXPAND(ICECREAM_APPLY_S(IC_, ICECREAM_ARGS_SIZE(__VA_ARGS__), S, __VA_ARGS__))
#endif


#define ICECREAM_ASSERT(exp, msg) assert(((void)msg, exp))


namespace boost
{
    class exception;

    // Forward declare this internal function because boost::diagnostic_information has a
    // default argument, and if this icecream.hpp header is included before the boost
    // exception headers it will trigger a compile error: redeclaration of ‘template<class
    // T> std::string boost::diagnostic_information(const T&, bool)’ may not have default
    // arguments
    namespace exception_detail
    {
        std::string diagnostic_information_impl(
            boost::exception const*, std::exception const*, bool, bool
        );
    }

    template <typename T> class scoped_ptr;
    template <typename T> class weak_ptr;

    namespace variant2
    {
        template<typename... T> class variant;

        template <typename R, typename Visitor, typename Variant>
        constexpr auto visit(Visitor&& vis, Variant&& var) -> R;
    }
}


namespace icecream{ namespace detail
{
#if defined(_MSC_VER) && _MSC_VER <= 1916
    // Bug fix to VisualStudio not implementing char instantiations to std::codecvt<>,
    // but int instead.
    template<typename T> struct FIX;
    template<> struct FIX<char> {using type = int8_t;};
    template<> struct FIX<signed char> {using type = int8_t;};
    template<> struct FIX<unsigned char> {using type = uint8_t;};
    template<> struct FIX<wchar_t> {using type = int16_t;};
    template<> struct FIX<char16_t> {using type = int16_t;};
    template<> struct FIX<char32_t> {using type = int32_t;};
#else
    template<typename T> struct FIX {using type = T;};
#endif

    // Utility wrapper to adapt locale-bound facets for wstring/wbuffer convert
    template<class Facet>
    struct deletable_facet: Facet
    {
        template<typename... Ts>
        deletable_facet(Ts&& ...args)
            : Facet(std::forward<Ts>(args)...)
        {}

        virtual ~deletable_facet() = default;
    };


    // -------------------------------------------------- is_instantiation

    // Checks if a type T (like std::pair<int, float>) is an instantiation of a template
    // class U (like std::pair<typename T0, typename T1>).
    template <template<typename...> class, typename...>
    struct is_instantiation: std::false_type {};

    template <template<typename...> class U, typename... T>
    struct is_instantiation<U, U<T...>>: std::true_type {};


    // -------------------------------------------------- conjunction

    // Logical AND
    template <typename... Ts>
    struct conjunction: std::true_type {};

    template <typename T, typename... Ts>
    struct conjunction<T, Ts...>: std::conditional<
        T::value,
            conjunction<Ts...>,
            std::false_type
    >::type {};


    // -------------------------------------------------- disjunction

    // Logical OR
    template <typename... Ts>
    struct disjunction: std::false_type {};

    template <typename T, typename... Ts>
    struct disjunction<T, Ts...>: std::conditional<
        T::value,
            std::true_type,
            disjunction<Ts...>
    >::type {};


    // -------------------------------------------------- negation

    // Logical NOT
    template <typename T>
    using negation = typename std::conditional<
        T::value, std::false_type, std::true_type
    >::type;


    // -------------------------------------------------- sequence

    template<int... Is>
    struct sequence {};

    template<int N, int... Is>
    struct gen_sequence : gen_sequence<N - 1, N - 1, Is...> {};

    template<int... Is>
    struct gen_sequence<0, Is...>
    {
        using type = sequence<Is...>;
    };


    // -------------------------------------------------- is_one_of

    template <typename T, typename... Us>
    using is_one_of = typename disjunction<
        std::is_same<T, Us>...
    >::type;


    // -------------------------------------------------- is_bounded_array

    // Checks if T is an array with a known size.
    template <typename T>
    struct is_bounded_array_impl: std::false_type {};

    template <typename T, std::size_t N>
    struct is_bounded_array_impl<T[N]>: std::true_type {};

    template <typename T>
    using is_bounded_array = typename is_bounded_array_impl<
        typename std::remove_reference<T>::type
    >::type;


    // -------------------------------------------------- is_invocable

    // Checks if T is nullary invocable, i.e.: the statement T() is valid.
    template <typename T>
    auto is_invocable_impl(int) -> decltype (
        std::declval<T&>()(),
        std::true_type {}
    );

    template <typename T>
    auto is_invocable_impl(...) -> std::false_type;

    template <typename T>
    using is_invocable = decltype(is_invocable_impl<T>(0));


    // -------------------------------------------------- returned_type

    // Returns the result type of nullary function
    template <typename T>
    auto returned_type_impl(int) -> decltype(
        std::declval<T&>()()
    );

    template <typename T>
    auto returned_type_impl(...) -> void;

    template <typename T>
    using returned_type = decltype(returned_type_impl<T>(0));


    // -------------------------------------------------- is_iterable

    // To allow ADL with custom begin/end
    using std::begin;
    using std::end;

    // Checks if T is iterable, i.e.: to a variable `t` and an iterator `it`, are valid
    // the statements: begin(t), end(t), it != it, ++it, *it.
    template <typename T>
    auto is_iterable_impl(int) -> decltype (
        begin(std::declval<T const&>()) != end(std::declval<T const&>()), // begin, end, operator!=
        ++std::declval<decltype(begin(std::declval<T const&>()))&>(),     // operator++
        *begin(std::declval<T const&>()),                                 // operator*
        std::true_type {}
    );

    template <typename T>
    auto is_iterable_impl(...) -> std::false_type;

    template <typename T>
    using is_iterable = decltype(is_iterable_impl<T>(0));


    // -------------------------------------------------- has_push_back_T
    // Checks if the class `C` has a method push_back(`T`)

    template <typename C, typename T>
    auto has_push_back_T_impl(int) -> decltype(
        std::declval<C&>().push_back(std::declval<T&>()),
        std::true_type {}
    );

    template <typename C, typename T>
    auto has_push_back_T_impl(...) -> std::false_type;

    template <typename C, typename T>
    using has_push_back_T = decltype(has_push_back_T_impl<C, T>(0));


    // -------------------------------------------------- has_insertion

    // Checks if T has an insertion overload, i.e.: std::ostream& << T const&
    template <typename T>
    auto has_insertion_impl(int) -> decltype (
        std::declval<std::ostream&>() << std::declval<T const&>(),
        std::true_type {}
    );

    template <typename T>
    auto has_insertion_impl(...) -> std::false_type;

    template <typename T>
    using has_insertion = decltype(has_insertion_impl<T>(0));


    // -------------------------------------------------- is_tuple

    // Checks if T is a tuple like type, i.e.: an instantiation of one of
    // std::pair<typename U0, typename U1> or std::tuple<typename... Us>.
    template <typename T>
    using is_tuple = disjunction<
        is_instantiation<std::pair, typename std::decay<T>::type>,
        is_instantiation<std::tuple, typename std::decay<T>::type>
    >;


    // -------------------------------------------------- is_character

    // Checks if T is character type (char, char16_t, etc).
    template <typename T>
    using is_character = disjunction<
        std::is_same<typename std::decay<T>::type, char>,
        std::is_same<typename std::decay<T>::type, wchar_t>,
    #if defined(__cpp_char8_t)
        std::is_same<typename std::decay<T>::type, char8_t>,
    #endif
        std::is_same<typename std::decay<T>::type, char16_t>,
        std::is_same<typename std::decay<T>::type, char32_t>
    >;


    // -------------------------------------------------- is_xsig_char

    template <typename T>
    using is_xsig_char = disjunction<
        std::is_same<typename std::decay<T>::type, signed char>,
        std::is_same<typename std::decay<T>::type, unsigned char>
    >;


    // -------------------------------------------------- is_c_string

    // Checks if T is C string type, i.e.: either char* or char[]. A char[N] is not
    // considered a C string.
    template <typename T>
    using is_c_string = conjunction<
        negation<is_bounded_array<T>>,
        disjunction<
            std::is_same<typename std::decay<T>::type, char*>,
            std::is_same<typename std::decay<T>::type, char const*>,
            std::is_same<typename std::decay<T>::type, signed char*>,
            std::is_same<typename std::decay<T>::type, signed char const*>,
            std::is_same<typename std::decay<T>::type, unsigned char*>,
            std::is_same<typename std::decay<T>::type, unsigned char const*>,
            std::is_same<typename std::decay<T>::type, wchar_t*>,
            std::is_same<typename std::decay<T>::type, wchar_t const*>,
        #if defined(__cpp_char8_t)
            std::is_same<typename std::decay<T>::type, char8_t*>,
            std::is_same<typename std::decay<T>::type, char8_t const*>,
        #endif
            std::is_same<typename std::decay<T>::type, char16_t*>,
            std::is_same<typename std::decay<T>::type, char16_t const*>,
            std::is_same<typename std::decay<T>::type, char32_t*>,
            std::is_same<typename std::decay<T>::type, char32_t const*>
        >
    >;


    // -------------------------------------------------- is_std_string

    // Checks if T is a std::basic_string<typename U>
    template <typename T>
    using is_std_string = is_instantiation<std::basic_string, T>;


    // -------------------------------------------------- is_string_view

    // Checks if T is a std::basic_string_view<typename U>
#if defined(ICECREAM_STRING_VIEW_HEADER)
    template <typename T>
    using is_string_view = is_instantiation<std::basic_string_view, T>;
#else
    template <typename>
    using is_string_view = std::false_type;
#endif


    // -------------------------------------------------- is_collection

    // Checks if T is a collection, i.e.: an iterable type that is not a std::string.
    template <typename T>
    using is_collection = conjunction<
        is_iterable<T>, negation<is_std_string<T>>, negation<is_string_view<T>>
    >;


    // -------------------------------------------------- is_variant

    // Checks if T is a variant type.
    template <typename T>
    using is_variant = disjunction<
        is_instantiation<boost::variant2::variant, T>
    #if defined(ICECREAM_VARIANT_HEADER)
        , is_instantiation<std::variant, T>
    #endif
    >;


    // -------------------------------------------------- is_optional

    // Checks if T is an optional type.
    template <typename T>
    using is_optional = disjunction<
    #if defined(ICECREAM_OPTIONAL_HEADER)
        is_instantiation<std::optional, T>
    #endif
    >;


    // -------------------------------------------------- elements_type

    // Returns the elements type of a collection.
    template <typename T>
    auto elements_type_impl(int) -> decltype(
        *begin(std::declval<T&>())
    );

    template <typename T>
    auto elements_type_impl(...) -> void;

    template <typename T>
    using elements_type = decltype(elements_type_impl<T>(0));


    // -------------------------------------------------- is_not_streamable_ptr

    // Checks if T is either std::unique_ptr<typename U> instantiation (Until C++20), or a
    // boost::scoped_ptr<typename U>. Both are without an operator<<(ostream&) overload.
    template <typename T>
    using is_unstreamable_ptr = disjunction<
        is_instantiation<std::unique_ptr, T>, is_instantiation<boost::scoped_ptr, T>
    >;


    // -------------------------------------------------- is_weak_ptr

    // Checks if T is a instantiation if either std::weak_ptr<typename U> or
    // boost::weak_ptr<typename U>.
    template <typename T>
    using is_weak_ptr = disjunction<
        is_instantiation<std::weak_ptr, T>, is_instantiation<boost::weak_ptr, T>
    >;


    // -------------------------------------------------- is_valid_prefix

    // Checks if T can be used as prefix, i.e.: T is a string or a nullary function
    // returning a type that has a "ostream <<" overload.
    template <typename T>
    using is_valid_prefix = disjunction<
        is_std_string<T>,
        is_string_view<T>,
        is_c_string<typename std::decay<T>::type>,
        conjunction<
            is_invocable<T>,
            has_insertion<returned_type<T>>
        >
    >;


    // -------------------------------------------------- is_T_output_iterator
    // Checks if `Iterator` is an output iterator with type `Item`

    template <typename Iterator, typename Item>
    auto is_T_output_iterator_impl(int) -> decltype(
        *std::declval<Iterator&>() = std::declval<Item&>(),
        std::true_type {}
    );

    template <typename Iterator, typename Item>
    auto is_T_output_iterator_impl(...) -> std::false_type;

    template <typename Iterator, typename Item>
    using is_T_output_iterator = decltype(is_T_output_iterator_impl<Iterator, Item>(0));


    // // -------------------------------------------------- ensure_tuple

    template <typename T>
    auto ensure_tuple(T&& t) -> std::tuple<decltype(std::forward<T>(t))>
    {
        return std::forward_as_tuple(std::forward<T>(t));
    };

    template <typename... Ts>
    auto ensure_tuple(std::tuple<Ts...>&& t) -> std::tuple<Ts...>
    {
        return std::move(t);
    };

    // -------------------------------------------------- Tree

    // Needed to access the Icecream::show_c_string() method before the Icecream class
    // declaration.
    auto show_c_string() -> bool;

#if defined(ICECREAM_DUMP_STRUCT_CLANG)
    class Tree;
    static auto ds_this = static_cast<Tree*>(nullptr);
    static auto ds_delayed_structs = std::vector<std::pair<std::string, std::function<void()>>>{};
    static auto ds_stream_ref = static_cast<std::basic_ostream<char> const*>(nullptr);
    static auto ds_call_count = int{0};
    template<typename... T> auto parse_dump_struct(char const* format, T&& ...args) -> int;
#endif

    // Builds an ostringstream and sets its state accordingly to `fmt` string
    inline auto build_ostream(std::string const& fmt) -> std::tuple<bool, std::ostringstream>
    {
        // format_spec ::=  [[fill]align][sign]["#"][width]["." precision][type]
        // fill        ::=  <a character>
        // align       ::=  "<" | ">" | "v"
        // sign        ::=  "+" | "-"
        // width       ::=  integer
        // precision   ::=  integer
        // type        ::=  "a" | "A" | "d" | "e" | "E" | "f" | "F" | "g" | "G" | "o" | "x" | "X"
        // integer     ::=  digit+
        // digit       ::=  "0"..."9"

        auto os = std::ostringstream {};

        auto it = std::begin(fmt);
        auto end_it = std::end(fmt);

        if (it == end_it) return std::make_tuple(true, std::move(os));

        // [[fill]align]
        {
            auto fill_char = os.fill();
            if (*it != '<' && *it != '>' && *it != 'v')
            {
                auto la_it = it+1;
                if (la_it != end_it && (*la_it == '<' || *la_it == '>' || *la_it == 'v'))
                {
                    fill_char = *it;
                    ++it;
                }
            }
            if (it != end_it && *it == '<')
            {
                os << std::left << std::setfill(fill_char);
                ++it;
            }
            else if (it != end_it && *it == '>')
            {
                os << std::right << std::setfill(fill_char);
                ++it;
            }
            else if (it != end_it && *it == 'v')
            {
                os << std::internal << std::setfill(fill_char);
                ++it;
            }
        }

        // [sign]
        if (it != end_it && *it == '+')
        {
            os << std::showpos;
            ++it;
        }
        else if (it != end_it && *it == '-')
        {
            os << std::noshowpos;
            ++it;
        }

        // ["#"]
        if (it != end_it && *it == '#')
        {
            os << std::showbase << std::showpoint;
            ++it;
        }

        // [width]
        {
            auto b_it = it;
            while (it != end_it && *it >= '0' && *it <= '9') ++it;
            if (it != b_it)
                os << std::setw(std::stoi(std::string(b_it, it)));
        }

        // ["." precision]
        if (it != end_it && *it == '.')
        {
            auto b_it = it+1;
            auto p_it = b_it;
            while (p_it != end_it && *p_it >= '0' && *p_it <= '9') ++p_it;
            if (p_it != b_it)
            {
                os << std::setprecision(std::stoi(std::string(b_it, p_it)));
                it = p_it;
            }
        }

        // [type]
        if (it != end_it && *it == 'a')
        {
            os << std::hexfloat << std::nouppercase;
            ++it;
        }
        else if (it != end_it && *it == 'A')
        {
            os << std::hexfloat << std::uppercase;
            ++it;
        }
        else if (it != end_it && *it == 'd')
        {
            os << std::dec;
            ++it;
        }
        else if (it != end_it && *it == 'e')
        {
            os << std::scientific << std::nouppercase;
            ++it;
        }
        else if (it != end_it && *it == 'E')
        {
            os << std::scientific << std::uppercase;
            ++it;
        }
        else if (it != end_it && *it == 'f')
        {
            os << std::fixed << std::nouppercase;
            ++it;
        }
        else if (it != end_it && *it == 'F')
        {
            os << std::fixed << std::uppercase;
            ++it;
        }
        else if (it != end_it && *it == 'g')
        {
            os << std::defaultfloat << std::nouppercase;
            ++it;
        }
        else if (it != end_it && *it == 'G')
        {
            os << std::defaultfloat << std::uppercase;
            ++it;
        }
        else if (it != end_it && *it == 'o')
        {
            os << std::oct;
            ++it;
        }
        else if (it != end_it && *it == 'x')
        {
            os << std::hex << std::nouppercase;
            ++it;
        }
        else if (it != end_it && *it == 'X')
        {
            os << std::hex << std::uppercase;
            ++it;
        }

        return std::make_tuple((it == end_it), std::move(os));
    }

    class Tree
    {
    private:

        bool is_leaf_;

        union U
        {
            std::string leaf;

            struct Stem
            {
                std::string open;
                std::string separator;
                std::string close;
                std::vector<Tree> children;

                Stem(
                    std::string&& open_,
                    std::string&& separator_,
                    std::string&& close_,
                    std::vector<Tree>&& children_
                )
                    : open {std::move(open_)}
                    , separator {std::move(separator_)}
                    , close {std::move(close_)}
                    , children {std::move(children_)}
                {}

            } stem;

            U(U&& other, bool is_leaf)
            {
                if (is_leaf)
                    new (&leaf) std::string{std::move(other.leaf)};
                else
                    new (&stem) Stem{std::move(other.stem)};
            }

            U(std::string&& leaf_)
                : leaf {std::move(leaf_)}
            {}

            U(Stem&& stem_)
                : stem {std::move(stem_)}
            {}

            ~U() {}

        } content_;

        struct InnerTag {};

        Tree(InnerTag, std::string leaf)
            : is_leaf_ {true}
            , content_ {std::move(leaf)}
        {}

        Tree(InnerTag, U::Stem&& stem)
            : is_leaf_ {false}
            , content_ {std::move(stem)}
        {}

    public:

        auto is_leaf() const -> bool
        {
            return this->is_leaf_;
        }

        auto leaf() const -> std::string const&
        {
            ICECREAM_ASSERT(this->is_leaf_, "Expecting a leaf tree.");
            return this->content_.leaf;
        }

        auto stem() const -> U::Stem const&
        {
            ICECREAM_ASSERT(!this->is_leaf_, "Expecting a stem tree.");
            return this->content_.stem;
        }

        // Returns the sum of characters of the whole tree.
        auto count_chars() const -> std::size_t
        {
            auto const count_utf8_char = [&](std::string const& str) -> std::size_t
            {
                auto result = std::size_t{0};
                auto const n_bytes = str.length();
                for (std::size_t idx = 0; idx < n_bytes; ++idx, ++result)
                {
                    auto const c = static_cast<uint8_t>(str[idx]);
                    if (c<=127) idx+=0;
                    else if ((c & 0xE0) == 0xC0) idx+=1;
                    else if ((c & 0xF0) == 0xE0) idx+=2;
                    else if ((c & 0xF8) == 0xF0) idx+=3;
                    else continue; //invalid utf8, silently move on
                }

                return result;
            };

            if (this->is_leaf_)
            {
                return count_utf8_char(this->content_.leaf);
            }
            else
            {
                // The enclosing chars.
                auto result =
                    count_utf8_char(this->content_.stem.open) + count_utf8_char(this->content_.stem.close);

                // count the size of each child
                for (auto const& child : this->content_.stem.children)
                    result += child.count_chars();

                // The separators.
                if (this->content_.stem.children.size() > 1)
                    result +=
                        (this->content_.stem.children.size() - 1)
                        * count_utf8_char(this->content_.stem.separator);

                return result;
            }
        }


        Tree() = delete;
        Tree(Tree const&) = delete;
        Tree& operator=(Tree const&) = delete;

        Tree(Tree&& other)
            : is_leaf_ {other.is_leaf_}
            , content_ {std::move(other.content_), other.is_leaf_}
        {}

        Tree& operator=(Tree&& other)
        {
            if (this != &other)
            {
                this->~Tree();
                new (this) Tree{std::move(other)};
            }

            return *this;
        }

        ~Tree()
        {
            if (this->is_leaf_)
                this->content_.leaf.~basic_string();
            else
                this->content_.stem.~Stem();
        }

        static auto literal(std::string text) -> Tree
        {
            return Tree{InnerTag{}, std::move(text)};
        }

        Tree(
            std::string&& open,
            std::string&& separator,
            std::string&& close,
            std::vector<Tree>&& children
        )
            : Tree {
                InnerTag{},
                U::Stem{
                    std::move(open),
                    std::move(separator),
                    std::move(close),
                    std::move(children)
                }
            }
        {}

        // Print any class that overloads operator<<(std::ostream&, T)
        template <typename T>
        Tree(T const& value, std::basic_ostream<char> const& stream_ref,
            typename std::enable_if<
                has_insertion<T>::value
                && !is_c_string<T>::value
                && !is_character<T>::value
                && !is_xsig_char<T>::value
                && !is_std_string<T>::value
                && !is_string_view<T>::value
                && !std::is_array<T>::value
            >::type* = nullptr
        )
            : Tree {InnerTag{}, [&]
            {
                std::ostringstream buf;
                buf.copyfmt(stream_ref);
                buf << value;
                return buf.str();
            }()}
        {}

        // Print C string
        template <typename T>
        Tree(T const& value, std::basic_ostream<char> const& stream_ref,
            typename std::enable_if<
                is_c_string<T>::value
            >::type* = nullptr
        )
            : Tree {InnerTag{}, [&]
            {
                std::ostringstream buf;
                buf.copyfmt(stream_ref);

                if (show_c_string())
                {
                    using DT = typename std::decay<
                        typename std::remove_pointer<
                            typename std::decay<T>::type
                        >::type
                    >::type;

                    if ICECREAM_IF_CONSTEXPR (
                        // On MacOS, an identity cv.to_bytes operation (between two identical
                        // character types) throws an exception. If they are the same, we do not
                        // make any conversion.
                        std::is_same<DT, std::ostringstream::char_type>::value
                    #if defined(__cpp_char8_t)
                        || std::is_same<DT, char8_t>::value
                    #endif
                    )
                        buf << '"' << reinterpret_cast<std::ostringstream::char_type const*>(value) << '"';
                    else
                    {
                        using FF = typename FIX<DT>::type;
                        std::wstring_convert<
                            deletable_facet<std::codecvt<FF, std::ostringstream::char_type, std::mbstate_t>>,
                            FF
                        > cv {};

                    #if defined(_MSC_VER) && _MSC_VER <= 1916
                        FF const* e = (FF const*)value;
                        while (*e != 0) ++e;
                        buf << '"' << cv.to_bytes((FF const*)value, e) << '"';
                    #else
                        buf << '"' << cv.to_bytes(value) << '"';
                    #endif
                    }
                }
                else
                    buf << reinterpret_cast<void const*>(value);

                return buf.str();
            }()}
        {}

        // Print std::string
        template <typename T>
        Tree(T const& value, std::basic_ostream<char> const& stream_ref,
            typename std::enable_if<
                is_std_string<T>::value
            >::type* = nullptr
        )
            : Tree {InnerTag{}, [&]
            {
                using VT = typename T::value_type;

                std::ostringstream buf;
                buf.copyfmt(stream_ref);

                if ICECREAM_IF_CONSTEXPR (
                    // On MacOS, an identity cv.to_bytes operation (between two identical
                    // character types) throws an exception. If they are the same, we do not
                    // make any conversion.
                    std::is_same<VT, std::ostringstream::char_type>::value
                #if defined(__cpp_char8_t)
                    || std::is_same<VT, char8_t>::value
                #endif
                )
                {
                    // This is an idle reinterpret_cast, since both character types must
                    // be the same to the program reaches this lines. However, this is
                    // required because even not being reached, if the character types are
                    // not the same, a compiling error would happens.
                    using STR_PTR = std::basic_string<std::ostringstream::char_type> const*;
                    buf << '"' << *reinterpret_cast<STR_PTR>(&value) << '"';
                }
                else
                {
                    using FF = typename FIX<VT>::type;
                    std::wstring_convert<
                        deletable_facet<std::codecvt<FF, std::ostringstream::char_type, std::mbstate_t>>,
                        FF
                    > cv {};

                #if defined(_MSC_VER) && _MSC_VER <= 1916
                    FF const* b = (FF const*)value.data();
                    FF const* e = (FF const*)value.data();
                    while (*e != 0) ++e;
                    buf << '"' << cv.to_bytes(b, e) << '"';
                #else
                    buf << '"' << cv.to_bytes(value.data()) << '"';
                #endif
                }

                return buf.str();
            }()}
        {}

    #if defined(ICECREAM_STRING_VIEW_HEADER)
        // Print std::string_view
        template <typename T>
        Tree(T const& value, std::basic_ostream<char> const& stream_ref,
            typename std::enable_if<
                is_string_view<T>::value
            >::type* = nullptr
        )
            : Tree {std::basic_string<typename T::value_type>{value}, stream_ref}
        {}
    #endif

        // Print character
        template <typename T>
        Tree(T const& value, std::basic_ostream<char> const& stream_ref,
            typename std::enable_if<
                is_character<T>::value
            >::type* = nullptr
        )
            : Tree {InnerTag{}, [&]
            {
                using DT = typename std::decay<T>::type;
                using FF = typename FIX<DT>::type;

                std::ostringstream buf;
                buf.copyfmt(stream_ref);

                auto str = std::string {};
                switch (value)
                {
                case T{'\0'}:
                    str = "\\0";
                    break;

                case T{'\a'}:
                    str = "\\a";
                    break;

                case T{'\b'}:
                    str = "\\b";
                    break;

                case T{'\f'}:
                    str = "\\f";
                    break;

                case T{'\n'}:
                    str = "\\n";
                    break;

                case T{'\r'}:
                    str = "\\r";
                    break;

                case T{'\t'}:
                    str = "\\t";
                    break;

                case T{'\v'}:
                    str = "\\v";
                    break;

                default:
                    if ICECREAM_IF_CONSTEXPR (
                        // On MacOS, an identity cv.to_bytes operation (between two identical
                        // character types) throws an exception. If they are the same, we do not
                        // make any conversion.
                        std::is_same<DT, std::ostringstream::char_type>::value
                    #if defined(__cpp_char8_t)
                        || std::is_same<DT, char8_t>::value
                    #endif
                    )
                        // Innocuous cast, just to silence a warning with types where this line is not reached.
                        str = static_cast<std::ostringstream::char_type>(value);
                    else
                        {
                            std::wstring_convert<
                                deletable_facet<
                                    std::codecvt<FF, std::ostringstream::char_type, std::mbstate_t>
                                >,
                                FF
                            > cv {};
                            str = cv.to_bytes(value);
                        }
                    break;
                }

                buf << '\'' << str << '\'';

                return buf.str();
            }()}
        {}

        // Print signed and unsigned char
        template <typename T>
        Tree(T const& value, std::basic_ostream<char> const& stream_ref,
            typename std::enable_if<
                is_xsig_char<T>::value
            >::type* = nullptr
        )
            : Tree {InnerTag{}, [&]
            {
                using T0 = typename std::conditional<
                    std::is_signed<T>::value, int, unsigned int
                >::type;

                std::ostringstream buf;
                buf.copyfmt(stream_ref);
                buf << static_cast<T0>(value);
                return buf.str();
            }()}
        {}

        // Print smart pointers without an operator<<(ostream&) overload.
        template <typename T>
        Tree(T const& value, std::basic_ostream<char> const& stream_ref,
            typename std::enable_if<
                is_unstreamable_ptr<T>::value
                && !has_insertion<T>::value // On C++20 unique_ptr will have a << overload.
            >::type* = nullptr
        )
            : Tree {InnerTag{}, [&]
            {
                std::ostringstream buf;
                buf.copyfmt(stream_ref);
                buf << reinterpret_cast<void const*>(value.get());
                return buf.str();
            }()}
        {}

        // Print weak pointer classes
        template <typename T>
        Tree(T const& value, std::basic_ostream<char> const& stream_ref,
            typename std::enable_if<
                is_weak_ptr<T>::value
            >::type* = nullptr
        )
            : Tree {value.expired() ? Tree{InnerTag{}, "expired"} : Tree {value.lock(), stream_ref}}
        {}

    #if defined(ICECREAM_OPTIONAL_HEADER)
        // Print std::optional<> classes
        template <typename T>
        Tree(T const& value, std::basic_ostream<char> const& stream_ref,
            typename std::enable_if<
                is_optional<T>::value
                && !has_insertion<T>::value
            >::type* = nullptr
        )
            : Tree {value.has_value() ? Tree{*value, stream_ref} : Tree{InnerTag{}, "nullopt"}}
        {}
    #endif

        struct Visitor
        {
            Visitor(std::basic_ostream<char> const& stream_ref_)
                : stream_ref{stream_ref_}
            {}

            template <typename T>
            auto operator()(T const& arg) -> Tree
            {
                return Tree {arg, this->stream_ref};
            }
            std::basic_ostream<char> const& stream_ref;
        };

        // Print *::variant<Ts...> classes
        template <typename T>
        Tree(T const& value, std::basic_ostream<char> const& stream_ref,
            typename std::enable_if<
                is_variant<T>::value
                && !has_insertion<T>::value
            >::type* = nullptr
        )
            : Tree {visit(Tree::Visitor{stream_ref}, value)}
        {}

        // Fill this->content.stem.children with all the tuple elements
        template<typename T, std::size_t N = std::tuple_size<T>::value-1>
        static auto tuple_traverser(
            T const& t, std::basic_ostream<char> const& stream_ref
        ) -> std::vector<Tree>
        {
            auto result = N > 0 ?
                tuple_traverser<T, (N > 0) ? N-1 : 0>(t, stream_ref) : std::vector<Tree> {};

            result.emplace_back(std::get<N>(t), stream_ref);
            return result;
        }

        // Print tuple like classes
        template <typename T>
        Tree(T&& value, std::basic_ostream<char> const& stream_ref,
            typename std::enable_if<
                is_tuple<T>::value
                && !has_insertion<T>::value
            >::type* = nullptr
        )
            : Tree {InnerTag{}, U::Stem{"(", ", ", ")", Tree::tuple_traverser(value, stream_ref)}}
        {}

        // Print all items of any iterable class
        template <typename T>
        Tree(T const& value, std::basic_ostream<char> const& stream_ref,
            typename std::enable_if<
                (
                    is_iterable<T>::value
                    && !has_insertion<T>::value
                    && !is_std_string<T>::value
                    && !is_string_view<T>::value
                )
                || std::is_array<T>::value
            >::type* = nullptr
        )
            : Tree {
                InnerTag{},
                U::Stem{"[", ", ", "]", [&]
                {
                    auto result = std::vector<Tree> {};
                    for (auto const& i : value)
                    {
                        result.emplace_back(i, stream_ref);
                    }
                    return result;
                }()}}
        {}

        // Print classes deriving from only std::exception and not from boost::exception
        template <typename T>
        Tree(T const& value, std::basic_ostream<char> const&,
            typename std::enable_if<
                std::is_base_of<std::exception, T>::value
                && !std::is_base_of<boost::exception, T>::value
                && !has_insertion<T>::value
             >::type* = nullptr
        )
            : Tree {InnerTag{}, value.what()}
        {}

        // Print classes deriving from both std::exception and boost::exception
        template <typename T>
        Tree(T const& value, std::basic_ostream<char> const&,
            typename std::enable_if<
                std::is_base_of<std::exception, T>::value
                && std::is_base_of<boost::exception, T>::value
                && !has_insertion<T>::value
            >::type* = nullptr
        )
            : Tree {
                InnerTag{},
                (
                    value.what()
                    + std::string {"\n"}
                    + boost::exception_detail::diagnostic_information_impl(
                          &value, &value, true, true
                    )
                )
            }
        {}

    #if defined(ICECREAM_DUMP_STRUCT_CLANG)
        // Print classes using clang's __builtin_dump_struct (clang >= 15).
        template <typename T>
        Tree(T const& value, std::basic_ostream<char> const& stream_ref,
            typename std::enable_if<
                !is_collection<T>::value
                && !is_tuple<T>::value
                && !is_unstreamable_ptr<T>::value
                && !is_weak_ptr<T>::value
                && !is_std_string<T>::value
                && !is_string_view<T>::value
                && !is_variant<T>::value
                && !is_optional<T>::value
                && !has_insertion<T>::value
                && !is_character<T>::value
                && !is_c_string<T>::value
                && !std::is_base_of<std::exception, T>::value
                && !std::is_base_of<boost::exception, T>::value
            >::type* = nullptr
        )
            : Tree {InnerTag{}, ""}
        {
            // Search among the children of `this` Tree, by a Tree having `key` as its
            // content. Returns `nullptr` if no child has been found.
            auto find_tree = [this](std::string const& key) -> Tree*
            {
                auto to_visit = std::vector<Tree*>{this};
                while (!to_visit.empty())
                {
                    auto current = to_visit.back();
                    to_visit.pop_back();

                    if (!current->is_leaf_)
                    {
                        for (auto& child : current->content_.stem.children)
                        {
                            to_visit.push_back(&child);
                        }
                    }
                    else if (current->content_.leaf == key)
                    {
                        return current;
                    }
                }

                return nullptr;
            };

            if (!ds_this)  // If this is the outermost class being printed
            {
                ds_this = this;  // Put `this` on scope to be assigned inside `parse_dump_struct`
                ds_stream_ref = &stream_ref;
                __builtin_dump_struct(&value, &parse_dump_struct);  // Print the outermost class

                // Loop on each class that is an internal attribute of the outermost class
                while (!ds_delayed_structs.empty())
                {
                    auto const delayed_struct = ds_delayed_structs.back();
                    ds_delayed_structs.pop_back();

                    // Put that attribute class on scope to `parse_struct_dump`
                    ds_this = find_tree(delayed_struct.first);

                    if (ds_this == nullptr)
                    {
                        std::cerr
                            << "ICECREAM: Failure finding a child Tree, this should not have happened. "
                            "Please report a bug on https://github.com/renatoGarcia/icecream-cpp/issues\n";
                    }

                    delayed_struct.second();  // Print the class
                }

                ds_this = nullptr;
                ds_call_count = 0;
            }

            // Otherwise, the class at hand is an internal attribute from the outermost
            // class. If this is the case, this present calling of Tree constructor is
            // being made from inside a __builtin_dump_struct calling. So here we delay
            // the calling of __builtin_dump_struct to avoid a reentrant function call.
            else
            {
                auto const unique_id =
                    "icecream_415a8a88-aa51-44d5-8ccb-377d953413ef_" + std::to_string(ds_call_count);

                this->content_.leaf = unique_id;
                ds_delayed_structs.emplace_back(
                    unique_id,
                    [&value]()
                    {
                        __builtin_dump_struct(&value, &parse_dump_struct);
                    }
                );
                ds_call_count += 1;
            }
        }
    #endif

    };

#if defined(ICECREAM_DUMP_STRUCT_CLANG)

    // Receives an argument from parse_dump_struct and builds a Tree using it.
    //
    // When clang doesn't know how to format a value within __builtin_dump_struct, it will
    // pass a pointer to that value instead of a reference. This function specialization
    // will handle that scenario, as it specializes on pointers. However, to distinguish
    // between actual pointer values and a "don't know how to format" value, the `deref`
    // boolean is used.
    template<typename T>
    auto build_tree(bool deref, T* const& t) -> Tree
    {
        if (deref)
            return Tree {*t, *ds_stream_ref};
        else
            return Tree {t, *ds_stream_ref};
    }

    // Receives an argument from parse_dump_struct and builds a Tree using it.
    template<typename T>
    auto build_tree(bool, T const& t) -> Tree
    {
        return Tree {t, *ds_stream_ref};
    }

    // Receives all the variadic inputs from parse_dump_struct and returns a pair with the
    // argument name and its Tree representation.
    // This overload accepts zero, one, or two values from the variadic inputs. It is
    // here just so the code compiles and never will be called. When receiving these
    // number of inputs an argument to be printed will never be among them.
    inline
    auto build_name_tree_pair(
          bool, std::string const& = "", std::string const& = ""
    ) -> std::pair<std::string, Tree>
    {
        ICECREAM_ASSERT(false, "Shoud not reach here.");
        return std::pair<std::string, Tree>("", Tree::literal(""));
    }

    // Receives all the variadic inputs from parse_dump_struct and returns a pair with the
    // argument name and its Tree representation.
    // This overload will be called when starting the printing of an argument that is
    // itself a struct with other attributes. The empty `Tree` is a placeholder that will
    // be replaced when the actual tree is built.
    inline auto build_name_tree_pair(
        bool, std::string const&, std::string const&, std::string const& arg_name
    ) -> std::pair<std::string, Tree>
    {
        return std::pair<std::string, Tree>(arg_name, Tree::literal(""));
    }

    // Receives all the variadic inputs from parse_dump_struct and returns a pair with the
    // argument name and its Tree representation.
    template<typename T>
    auto build_name_tree_pair(
        bool deref,
        std::string const&,
        std::string const&,
        std::string const& arg_name,
        T const& value
    ) -> std::pair<std::string, Tree>
    {
        return std::pair<std::string, Tree>(arg_name, build_tree(deref, value));
    }

    // Each call to `parse_dump_struct` will deal with at most one attribute from the
    // struct being printed. This `attributes_stack` will hold the pairs (argument name,
    // Tree) constructed to each attribute until all of them are processed.
    static auto attributes_stack = std::vector<std::vector<std::pair<std::string, Tree>>> {};

    // When printing the attributes of a base class (inheritance), Clang will open and
    // close braces, similarly to when printing an attribute that is a struct
    // (composition). It is possible distinguish the two cases when at opening, but they
    // are identical when closing. This vector mark if the current context is inside a
    // inheritance attributes or a composition attributes.
    static auto is_inside_baseclass = std::vector<bool> {};

    template<typename... T>
    int parse_dump_struct(const char *format_, T&& ...args)
    {
        // Removes any left and right white spaces
        auto const trim =
            [](std::string const& str) -> std::string
            {
                auto const left = str.find_first_not_of(" \t\n");
                if (left == std::string::npos)
                    return "";

                auto const right = str.find_last_not_of(" \t\n");
                return str.substr(left, right-left+1);
            };

        // Check if the last format specifier, the one related to the value to be printed,
        // means that clang doesn't know how to format that value.
        auto const knows_how_to_format =
            [](std::string const& str) -> bool
            {
                auto const left = str.find_last_of(" \t");
                return str.substr(left+1) != "*%p";
            };

        auto const format = trim(format_);

        // When true, the next parse_dump_struct call will be the opening brace of a base
        // class scope.
        if (sizeof...(args) == 2)
        {
            is_inside_baseclass.push_back(true);
        }

        // When true, the next parse_dump_struct call will be the opening brace of a scope
        // printing the content of a struct attribute.
        else if (sizeof...(args) == 3)
        {
            is_inside_baseclass.push_back(false);

            attributes_stack.back().push_back(
                build_name_tree_pair(!knows_how_to_format(format), args...)
            );

            // The next attributes will be from this struct. Collect them all before
            // merging on a Tree.
            attributes_stack.emplace_back();
        }

        // When true, is printing an actual value. Like "int i = 7".
        else if (sizeof...(args) == 4)
        {
            attributes_stack.back().push_back(
                build_name_tree_pair(!knows_how_to_format(format), args...)
            );
        }

        // The closing brace of a baseclass attributes section
        else if (format.back() == '}' && is_inside_baseclass.back())
        {
            is_inside_baseclass.pop_back();
        }

        // The closing brace of either a struct attribute or the root struct being printed
        else if (format.back() == '}')
        {
            is_inside_baseclass.pop_back();

            auto ttributes_stack = std::move(attributes_stack.back());
            attributes_stack.pop_back();

            auto children = std::vector<Tree>{};
            for (auto& att : ttributes_stack)
            {
                auto t_pair = std::vector<Tree> {};
                t_pair.push_back(Tree::literal(std::get<0>(att)));
                t_pair.push_back(std::move(std::get<1>(att)));

                children.emplace_back("", ": ", "", std::move(t_pair));
            }

            auto tttree = Tree {"{", ", ", "}", std::move(children)};

            if (attributes_stack.empty())
                *ds_this = std::move(tttree);
            else
            {
                std::get<1>(attributes_stack.back().back()) = std::move(tttree);
            }
        }

        // The opening of the root struct being printed
        else if (sizeof...(args) == 1)
        {
            attributes_stack.emplace_back();
            is_inside_baseclass.push_back(false);
        }

        return 0;
    }

#endif  // ICECREAM_DUMP_STRUCT_CLANG

    // -------------------------------------------------- is_tree_argument

    template <typename T>
    auto is_tree_argument_impl(int) -> decltype (
        Tree {std::declval<T&>(), std::declval<std::basic_ostream<char> const&>()},
        std::true_type {}
    );

    template <typename T>
    auto is_tree_argument_impl(...) -> std::false_type;

    // If there exist a constructor of Tree accepting a T argument.
    template <typename T>
    using is_tree_argument = decltype(is_tree_argument_impl<T>(0));


    // -------------------------------------------------- is_printable

    // Check if IceCream can print the type T.
    template <typename T>
    struct is_printable: is_tree_argument<T> {};


    // -------------------------------------------------- to_invocable

    // If value is a string returns an function that returns it.
    template <typename T>
    auto to_invocable(T&& value,
        typename std::enable_if <
            is_std_string<T>::value
            || is_c_string<typename std::decay<T>::type>::value
        >::type* = nullptr
    ) -> std::function<std::string()>
    {
        auto const str = std::string {value};
        return [str]{return str;};
    }

    // If value is already invocable do nothing.
    template <typename T>
    auto to_invocable(T&& value,
        typename std::enable_if<
            is_invocable<T>::value
        >::type* = nullptr
    ) -> T&&
    {
        return std::forward<T>(value);
    }


    // -------------------------------------------------- Prefix
    class Prefix
    {
    private:
        template <typename T>
        struct Function
        {
            static_assert(is_invocable<T>::value, "");

            Function (T const& func_)
                : func{func_}
            {}

            auto operator()() -> std::string
            {
                auto buf = std::ostringstream {};
                buf << this->func();
                return buf.str();
            }

            T func;
        };

        std::vector<std::function<std::string()>> functions;

    public:
        Prefix() = delete;
        Prefix(Prefix const&) = delete;
        Prefix(Prefix&&) = default;
        Prefix& operator=(Prefix const&) = delete;
        Prefix& operator=(Prefix&&) = default;

        template <typename... Ts>
        Prefix(Ts&& ...funcs)
            : functions {}
        {
            // Hack to call this->functions.emplace_back to all funcs
            (void) std::initializer_list<int> {
                (
                    (void) this->functions.push_back(
                        Function<typename std::decay<Ts>::type> {
                            std::forward<Ts>(funcs)
                        }
                    ),
                    0
                 )...
            };
        }

        auto operator()() -> std::string
        {
            auto result = std::string {};
            for (auto const& func : this->functions)
                result.append(func());

            return result;
        }
    };


    // -------------------------------------------------- Icecream

    class Icecream
    {
    public:
        using size_type = std::size_t;

        Icecream(Icecream const&) = delete;
        Icecream(Icecream&&) = delete;
        Icecream& operator=(Icecream const&) = delete;
        Icecream& operator=(Icecream&&) = delete;

        static auto instance() -> Icecream&
        {
            static Icecream ic;
            return ic;
        }

        auto enable() -> void
        {
            std::lock_guard<std::mutex> guard(this->mutex);
            this->enabled_ = true;
        }

        auto disable() -> void
        {
            std::lock_guard<std::mutex> guard(this->mutex);
            this->enabled_ = false;
        }

        auto output(std::ostream& stream) -> void
        {
            std::lock_guard<std::mutex> guard(this->mutex);

            using OSIt = std::ostreambuf_iterator<char>;
            this->output_ = Output<OSIt>{OSIt{stream}};
        }

        template <typename T>
        auto output(T& container) ->
            typename std::enable_if<
                has_push_back_T<T, char>::value
            >::type
        {
            std::lock_guard<std::mutex> guard(this->mutex);

            using OSIt = std::back_insert_iterator<T>;
            this->output_ = Output<OSIt>{OSIt{container}};
        }

        template <typename T>
        auto output(T iterator) ->
            typename std::enable_if<
                is_T_output_iterator<T, char>::value
            >::type
        {
            std::lock_guard<std::mutex> guard(this->mutex);
            this->output_ = Output<T>{iterator};
        }

        template <typename... Ts>
        auto prefix(Ts&& ...value) -> void
        {
            std::lock_guard<std::mutex> guard(this->mutex);
            this->prefix_ = Prefix {to_invocable(std::forward<Ts>(value))...};
        }

        auto show_c_string() const -> bool
        {
            std::lock_guard<std::mutex> guard(this->mutex);
            return this->show_c_string_;
        }

        auto show_c_string(bool value) -> void
        {
            std::lock_guard<std::mutex> guard(this->mutex);
            this->show_c_string_ = value;
        }

        auto line_wrap_width() const -> std::size_t
        {
            std::lock_guard<std::mutex> guard(this->mutex);
            return this->line_wrap_width_;
        }

        auto line_wrap_width(std::size_t value) -> void
        {
            std::lock_guard<std::mutex> guard(this->mutex);
            this->line_wrap_width_ = value;
        }

        auto include_context() const -> bool
        {
            std::lock_guard<std::mutex> guard(this->mutex);
            return this->include_context_;
        }

        auto include_context(bool value) -> void
        {
            std::lock_guard<std::mutex> guard(this->mutex);
            this->include_context_ = value;
        }

        auto context_delimiter() const -> std::string
        {
            std::lock_guard<std::mutex> guard(this->mutex);
            return this->context_delimiter_;
        }

        auto context_delimiter(std::string value) -> void
        {
            std::lock_guard<std::mutex> guard(this->mutex);
            this->context_delimiter_ = value;
        }

        template <typename... Ts>
        auto print(
            std::string const& file,
            int line,
            std::string const& function,
            std::string const& format,
            std::vector<std::string> const& arg_names,
            Ts&&... args
        ) -> void
        {
            std::lock_guard<std::mutex> guard(this->mutex);

            if (!this->enabled_)
                return;

            auto const prefix = this->prefix_();
            auto context = std::string {};

            // If used an empty IC macro, i.e.: IC().
            if (sizeof...(Ts) == 0 || this->include_context_)
            {
             #if defined(_MSC_VER)
                auto const n = file.rfind('\\');
             #else
                auto const n = file.rfind('/');
             #endif
                context = file.substr(n+1) + ":" + std::to_string(line) + " in \"" + function + '"';
            }

            if (sizeof...(Ts) == 0)
            {
                this->output_(prefix);
                this->output_(context);
            }
            else
            {
                auto const forest = Icecream::build_forest(
                    format, std::begin(arg_names), std::forward<Ts>(args)...
                );
                this->print_forest(prefix, context, forest);
            }

            this->output_("\n");
        }

    private:
        template <typename T>
        struct Output
        {
            Output(T it_)
                : it{it_}
            {}

            auto operator()(std::string const& str) -> void
            {
                for (auto const& c : str)
                {
                    *this->it = c;
                    ++this->it;
                }
            }

            T it;
        };

        friend auto show_c_string() -> bool;

        constexpr static size_type INDENT_BASE = 4;

        mutable std::mutex mutex;

        bool enabled_ = true;

        std::function<void(std::string const&)> output_ {
            Output<std::ostreambuf_iterator<char>>{std::ostreambuf_iterator<char>{std::cerr}}
        };

        Prefix prefix_ {[]{return "ic| ";}};

        std::string context_delimiter_ = "- ";

        std::size_t line_wrap_width_ = 70;

        bool show_c_string_ = true;

        bool include_context_ = false;


        Icecream() = default;

        auto print_tree(
            Tree const& node, size_type const indent_level, bool const is_tree_multiline
        ) -> void
        {
            auto const break_line = [&](size_type indent)
            {
                this->output_("\n");
                this->output_(std::string(indent * Icecream::INDENT_BASE, ' '));
            };

            if (node.is_leaf())
                this->output_(node.leaf());
            else
            {
                this->output_(node.stem().open);

                if (is_tree_multiline)
                    break_line(indent_level+1);

                for (
                    auto it = node.stem().children.cbegin();
                    it != node.stem().children.cend();
                ){
                    auto const is_child_multiline = bool {[&]
                    {
                        // If the whole tree is a single line all children are too.
                        if (!is_tree_multiline)
                        {
                            return false;
                        }
                        // Else check this particular child.
                        else
                        {
                            auto const child_line_width =
                                (indent_level+1)*Icecream::INDENT_BASE + it->count_chars();
                            return child_line_width > this->line_wrap_width_;
                        }
                    }()};

                    // Print the child.
                    this->print_tree(*it, indent_level+1, is_child_multiline);

                    ++it;

                    // Print the separators between children.
                    if (it != node.stem().children.cend())
                    {
                        if (is_tree_multiline)
                        {
                            // Trim any right white space.
                            auto idx = node.stem().separator.find_last_not_of(" \t");
                            if (idx != std::string::npos)
                                idx += 1;
                            else
                                idx = node.stem().separator.size();

                            this->output_(node.stem().separator.substr(0, idx));
                            break_line(indent_level+1);
                        }
                        else
                            this->output_(node.stem().separator);
                    }
                    else
                        if (is_tree_multiline)
                            break_line(indent_level);
                }

                this->output_(node.stem().close);
            }
        }

        auto print_forest(
            std::string const& prefix,
            std::string const& context,
            std::vector<std::tuple<std::string, Tree>> const& forest
        ) -> void
        {
            auto const is_forest_multiline = bool {[&]
            {
                auto r = prefix.size();

                if (!context.empty())
                    r += context.size() + this->context_delimiter_.size();

                // The variables name, the separator ": ", and content
                for (auto const& t : forest)
                    r += std::get<0>(t).size() + 2 + std::get<1>(t).count_chars();

                // The ", " separators between variables.
                if (forest.size() > 1)
                    r += (forest.size() - 1) * 2;

                return r > this->line_wrap_width_;
            }()};

            // Print prefix and context
            this->output_(prefix);
            if (!context.empty())
            {
                this->output_(context);
                if (is_forest_multiline)
                {
                    this->output_("\n");
                    this->output_(std::string(Icecream::INDENT_BASE, ' '));
                }
                else
                    this->output_(this->context_delimiter_);
            }

            // The forest is built with trees in reverse order.
            for (auto it = forest.crbegin(); it != forest.crend(); ++it)
            {
                auto const& arg_name = std::get<0>(*it);
                auto const& tree = std::get<1>(*it);

                auto const is_tree_multiline = bool {[&]
                {
                    // If the whole forest is a single line all trees will be too.
                    if (!is_forest_multiline)
                    {
                        return false;
                    }
                    else
                    {
                        auto const tree_line_width = [&]
                        {
                            if (it == forest.rbegin() && context.empty())
                                return
                                    prefix.size()
                                    + arg_name.size()
                                    + 2
                                    + tree.count_chars();
                            else
                                return
                                    Icecream::INDENT_BASE
                                    + arg_name.size()
                                    + 2
                                    + tree.count_chars();
                        }();

                        return tree_line_width > this->line_wrap_width_;
                    }
                }()};

                this->output_(arg_name);
                this->output_(": ");
                this->print_tree(tree, 1, is_tree_multiline);

                if (it+1 != forest.crend())
                {
                    if (is_forest_multiline)
                    {
                        this->output_("\n");
                        this->output_(std::string(Icecream::INDENT_BASE, ' '));
                    }
                    else
                        this->output_(", ");
                }
            }
        }


        auto build_forest(
            std::string const& format,
            std::vector<std::string>::const_iterator
        ) -> std::vector<std::tuple<std::string, Tree>>
        {
            return std::vector<std::tuple<std::string, Tree>> {};
        }

        template<typename T, std::size_t N = std::tuple_size<T>::value-1>
        static auto fill_forest_from_tuple(
           std::vector<std::string> const& arg_names,
           T const& t,
           std::vector<std::tuple<std::string, Tree>>& forest
        ) -> void
        {
            forest.emplace_back(arg_names.at(N), std::get<N>(t));
            if (N > 0) fill_forest_from_tuple<T, (N > 0) ? N-1 : 0>(arg_names, t, forest);
        }

        template <typename T, typename... Ts>
        auto build_forest(
            std::string const& format,
            std::vector<std::string>::const_iterator arg_name,
            T&& arg_value,
            Ts&&... args_tail
        ) -> std::vector<std::tuple<std::string, Tree>>
        {
            auto forest = Icecream::build_forest(
                format, arg_name+1, std::forward<Ts>(args_tail)...
            );

            auto result_os = build_ostream(format);
            if (std::get<0>(result_os))
            {
                forest.emplace_back(
                    *arg_name,
                    Tree{std::move(arg_value), std::get<1>(result_os)}
                );
            }
            else
            {
                forest.emplace_back(
                    *arg_name,
                    Tree::literal("*Error* on formatting string")
                );
            }

            return forest;
        }
    };

    inline auto show_c_string() -> bool
    {
        return Icecream::instance().show_c_string_;
    }

}} // namespace icecream::detail


namespace icecream
{
    // The Icecream class is a singleton. This IcecreamAPI class only task is be a
    // syntactic sugar to make possible call ic.disable() instead of
    // Icecream::instance().disable().
    class IcecreamAPI
    {
    public:

        IcecreamAPI() = default;
        IcecreamAPI(IcecreamAPI const&) = delete;
        IcecreamAPI(IcecreamAPI&&) = delete;
        IcecreamAPI& operator=(IcecreamAPI const&) = delete;
        IcecreamAPI& operator=(IcecreamAPI&&) = delete;

        auto enable() -> IcecreamAPI&
        {
            detail::Icecream::instance().enable();
            return *this;
        }

        auto disable() -> IcecreamAPI&
        {
            detail::Icecream::instance().disable();
            return *this;
        }

        template <typename T>
        auto output(T&& t) ->
            typename std::enable_if<
                detail::disjunction<
                    std::is_base_of<std::ostream, typename std::decay<T>::type>,
                    detail::has_push_back_T<T, char>,
                    detail::is_T_output_iterator<T, char>
                >::value,
            IcecreamAPI&>::type
        {
            detail::Icecream::instance().output(std::forward<T>(t));
            return *this;
        }

        template <typename... Ts>
        auto prefix(Ts&& ...value) ->
            typename std::enable_if<
                detail::conjunction<detail::is_valid_prefix<Ts>...>::value,
            IcecreamAPI&>::type
        {
            detail::Icecream::instance().prefix(std::forward<Ts>(value)...);
            return *this;
        }

        auto show_c_string() const -> bool
        {
            return detail::Icecream::instance().show_c_string();
        }

        auto show_c_string(bool value) -> IcecreamAPI&
        {
            detail::Icecream::instance().show_c_string(value);
            return *this;
        }

        auto line_wrap_width() const -> std::size_t
        {
            return detail::Icecream::instance().line_wrap_width();
        }

        auto line_wrap_width(std::size_t value) -> IcecreamAPI&
        {
            detail::Icecream::instance().line_wrap_width(value);
            return *this;
        }

        auto include_context() const -> bool
        {
            return detail::Icecream::instance().include_context();
        }

        auto include_context(bool value) -> IcecreamAPI&
        {
            detail::Icecream::instance().include_context(value);
            return *this;
        }

        auto context_delimiter() const -> std::string
        {
            return detail::Icecream::instance().context_delimiter();
        }

        auto context_delimiter(std::string const& value) -> IcecreamAPI&
        {
            detail::Icecream::instance().context_delimiter(value);
            return *this;
        }

        template <typename... Ts>
        auto print(
            std::string const& file,
            int line,
            std::string const& function,
            std::string const& format,
            std::vector<std::string> const& arg_names,
            Ts&&... args
        ) -> typename std::enable_if<
                detail::conjunction<detail::is_printable<Ts>...>::value
            >::type
        {
            detail::Icecream::instance().print(
                file, line, function, format, arg_names, std::forward<Ts>(args)...
            );
        }
    };

    static IcecreamAPI ic {};

} // namespace icecream

namespace icecream{ namespace detail
{
    /** This function will receive a string as "foo, bar, baz" and return a vector with
     * all the arguments split, such as ["foo", "bar", "baz"].
     */
    inline auto split_arguments(std::string const& all_names) -> std::vector<std::string>
    {
        auto result = std::vector<std::string>{};

        if (all_names.empty())
            return result;

        // Check if the '>' char is the closing of a template arguments listing. It will
        // be a template closing at `std::is_same<int, int>::value` but not at `5 > 2`
        using crev_it = std::string::const_reverse_iterator;
        auto is_closing_template = [](crev_it left_it, crev_it right_it) -> bool
        {
            if (*left_it != '>' || left_it == right_it)
                return false;

            --left_it; // move to right
            while (
                left_it != right_it
                && (*left_it == ' ' || *left_it == '\t' || *left_it == '\n' || *left_it == '\r' || *left_it == '\f' || *left_it == '\v')
            )
            {
                --left_it; // move to right
            }

            return *left_it == ':' && (left_it != right_it) && *(left_it-1) == ':';
        };

        auto right_cut = all_names.crbegin();
        auto left_cut = all_names.crbegin();
        auto const left_end = all_names.crend();
        auto parenthesis_count = int{0};
        auto angle_bracket_count = int{0};

        // Parse the arguments string backward. It is easier this way to check if a '<' or
        // '>' character is either a comparison operator, or a opening and closing of
        // templates arguments.
        while(true)
        {
            if (left_cut != left_end && (left_cut+1) != left_end)
            {
                // Ignore commas inside quotations (single and double)

                if (*left_cut == '"' && *(left_cut+1) != '\\')
                {
                    // Don't split anything inside a string

                    ++left_cut;
                    while (
                        !(  // Will iterate with left_cut until the stop condition:
                            (left_cut+1) == left_end                        // The next position is the end iterator
                            || (*left_cut == '"' && *(left_cut+1) != '\\')  // The current char the closing quotation
                        )
                    ) {
                        ++left_cut;
                    }
                    ++left_cut;
                }
                else if (*left_cut == '\'' && *(left_cut+1) != '\\')
                {
                    // Don't split a ',' (a comma between single quotation marks)

                    ++left_cut;
                    while (
                        !(  // Will iterate with left_cut until the stop condition:
                            (left_cut+1) == left_end                         // The next position is the end iterator
                            || (*left_cut == '\'' && *(left_cut+1) != '\\')  // The current char is the closing quotation
                        )
                    ) {
                        ++left_cut;
                    }
                    ++left_cut;
                }
            }

            if (left_cut == left_end || (*left_cut == ',' && parenthesis_count == 0 && angle_bracket_count == 0))
            {
                // If it have found the comma separating two arguments, or the left ending
                // of the leftmost argument.

                // Remove the leading spaces
                auto e_it = left_cut - 1;
                while (*e_it == ' ') --e_it;
                ++e_it;

                // Remove the trailing spaces
                while (*right_cut == ' ') ++right_cut;

                result.emplace(result.begin(), e_it.base(), right_cut.base());
                if (left_cut != left_end)
                {
                    right_cut = left_cut + 1;
                }
            }

            // It won't cut on a comma within parentheses, such as when the argument is a
            // function call, as in IC(foo(1, 2))
            else if (*left_cut == ')')
            {
                ++parenthesis_count;
            }
            else if (*left_cut == '(')
            {
                --parenthesis_count;
            }

            // It won't cut on a comma within a template argument list, such as in
            // IC(std::is_same<int, int>::value)
            else if (is_closing_template(left_cut, right_cut))
            {
                ++angle_bracket_count;
            }
            else if (*left_cut == '<' && angle_bracket_count > 0)
            {
                --angle_bracket_count;
            }

            if (left_cut == left_end)
                break;
            else
                ++left_cut;
        }

        return result;
    }


    // The use of this struct instead of a free function is a needed hack because of the
    // trailing comma problem with __VA_ARGS__ expansion. A macro like:
    //
    // IC(...) print(__FILE__, __LINE__, ICECREAM_FUNCTION, #__VA_ARGS__, __VA_ARGS__)
    //
    // when used with no arguments would expand to one of:
    //
    // print("foo.cpp", 42, "void bar()", "",)
    //
    // print("foo.cpp", 42, "void bar()", ,)
    struct Dispatcher
    {
        std::string const file;
        int line;
        std::string const function;
        std::string const format;
        std::string const arg_names;

        // Used by compilers that expand an empyt __VA_ARGS__ in
        // Dispatcher{bla, #__VA_ARGS__} to Dispatcher{bla, ""}
        Dispatcher(
            std::string const& file_,
            int line_,
            std::string const& function_,
            std::string const& format_,
            std::string const& arg_names_
        )
            : file {file_}
            , line {line_}
            , function {function_}
            , format {format_}
            , arg_names {arg_names_}
        {}

        // Used by compilers that expand an empyt __VA_ARGS__ in
        // Dispatcher{bla, #__VA_ARGS__} to Dispatcher{bla, }
        Dispatcher(
            std::string const& file_,
            int line_,
            std::string const& function_,
            std::string const& format_
        )
            : file {file_}
            , line {line_}
            , function {function_}
            , format {format_}
            , arg_names {""}
        {}

        template <typename... Ts>
        auto print(Ts&&... args) -> void
        {
            auto arg_names = split_arguments(this->arg_names);
            ::icecream::ic.print(file, line, function, format, arg_names, std::forward<Ts>(args)...);
        }

        // Return a std::tuple with all the args
        template <typename... Ts>
        auto ret(Ts&&... args) -> decltype(std::forward_as_tuple(std::forward<Ts>(args)...))
        {
            this->print(args...);
            return std::forward_as_tuple(std::forward<Ts>(args)...);
        }

        // Return the unique arg
        template <typename T>
        auto ret(T&& arg) -> decltype(arg)
        {
            this->print(arg);
            return std::forward<decltype(arg)>(arg);
        }

        auto ret() -> void
        {
            this->print();
        }
    };

}} // namespace icecream::detail


#endif // ICECREAM_HPP_INCLUDED
