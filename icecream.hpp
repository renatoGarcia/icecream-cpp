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
#include <codecvt>
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
    #if __has_builtin(__builtin_dump_struct)
        #define ICECREAM_DUMP_STRUCT_CLANG
        #include <climits>
    #endif
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
#define ICECREAM_UNPACK_30(F, ICM, ...) ICECREAM_UNPACK_29(F, ICM, __VA_ARGS__), std::get<39>(std::move(ret_tuple))
#define ICECREAM_UNPACK_31(F, ICM, ...) ICECREAM_UNPACK_30(F, ICM, __VA_ARGS__), std::get<30>(std::move(ret_tuple))
#define ICECREAM_UNPACK_32(F, ICM, ...) ICECREAM_UNPACK_31(F, ICM, __VA_ARGS__), std::get<31>(std::move(ret_tuple))

#define ICECREAM_APPLY_(F, ICM, N, ...) ICECREAM_UNPACK_##N(F, ICM, __VA_ARGS__) );}()
#define ICECREAM_APPLY(ICM, N, F, ...) ICECREAM_APPLY_(F, ICM, N, __VA_ARGS__)
#define ICECREAM_APPLY_S(ICM, N, S, F, ...) ICECREAM_APPLY_(F, ICM, N, S, __VA_ARGS__)

#if defined(ICECREAM_LONG_NAME)
    #define ICECREAM(...) ::icecream::detail::Dispatcher{__FILE__, __LINE__, ICECREAM_FUNCTION, #__VA_ARGS__}.ret(__VA_ARGS__)
    #define ICECREAM0() ::icecream::detail::Dispatcher{__FILE__, __LINE__, ICECREAM_FUNCTION, ""}.ret()
    #define ICECREAM_(S, ...) ICECREAM(::icecream::f_(S, __VA_ARGS__))
    #define ICECREAM_A(...) ICECREAM_EXPAND(ICECREAM_APPLY(ICECREAM, ICECREAM_ARGS_SIZE(__VA_ARGS__), __VA_ARGS__))
    #define ICECREAM_A_(S, ...) ICECREAM_EXPAND(ICECREAM_APPLY(ICECREAM_, ICECREAM_ARGS_SIZE(__VA_ARGS__), S, __VA_ARGS__))
#else
    #define IC(...) ::icecream::detail::Dispatcher{__FILE__, __LINE__, ICECREAM_FUNCTION, #__VA_ARGS__}.ret(__VA_ARGS__)
    #define IC0() ::icecream::detail::Dispatcher{__FILE__, __LINE__, ICECREAM_FUNCTION, ""}.ret()
    #define IC_(S, ...) IC(::icecream::f_(S, __VA_ARGS__))
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

        virtual ~deletable_facet() override = default;
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

    // -------------------------------------------------- Formatter

    // Holds a variable and its formatting string.
    template<typename T>
    struct Formatter
    {
        Formatter(std::string const& fmt_, T&& v_)
            : fmt{fmt_}
            , v{std::forward<T>(v_)}
        {}

        std::string fmt;
        T&& v;
    };

    // Holds a tuple with one Formatter to each `vs...`, all of them having the same `fmt` string.
    template<typename... Ts>
    struct FormatterPack
    {
        FormatterPack(std::string const& fmt, Ts&&... vs_)
            : vs{Formatter<Ts>(fmt, std::forward<Ts>(vs_))...}
        {
            static_assert(
                conjunction<
                    negation<is_instantiation<FormatterPack, typename std::decay<Ts>::type>>...
                >::value,
                "It is not possible to nest FormmaterPack's as in IC_(\"#\", 7, f_(\"#x\", 42))."
            );
        }

        std::tuple<Formatter<Ts>...> vs;
    };

    template <typename T>
    using is_formatter_pack = is_instantiation<FormatterPack, typename std::decay<T>::type>;

    template <typename... Ts, int... Is>
    auto drill_vars(FormatterPack<Ts...>&& fp, sequence<Is...>) -> std::tuple<Ts...>
    {
        return std::tuple<Ts...>{std::forward<Ts>(std::get<Is>(fp.vs).v) ...};
    }

    template <typename T>
    auto as_tuple(T&& t) -> std::tuple<decltype(std::forward<T>(t))>
    {
        return std::forward_as_tuple(std::forward<T>(t));
    };

    template <typename... Ts>
    auto as_tuple(FormatterPack<Ts...>&& fp) -> std::tuple<Ts...>
    {
        using Seq = typename gen_sequence<sizeof...(Ts)>::type;
        return drill_vars(std::move(fp), Seq{});
    };


    // Receive a sequence of parameters and return a tuple with all them. If there is any
    // FormatterPack's on that sequence, the variables inside them will be put on that
    // tuple, instead of the Formmaterpack itself.
    template<typename... Ts>
    auto flatten_formatter_pack(Ts&&... t) -> decltype(std::tuple_cat(as_tuple(std::forward<Ts>(t))...))
    {
        return std::tuple_cat(as_tuple(std::forward<Ts>(t))...);
    }


    // -------------------------------------------------- Tree

    // Needed to access the Icecream::show_c_string() method before the Icecream class
    // declaration.
    auto show_c_string() -> bool;

#if defined(ICECREAM_DUMP_STRUCT_CLANG)
    class Tree;
    static inline auto parse_struct_dump(char const* format, ...) -> int;
    static Tree* ds_this = nullptr;
    static std::ostringstream* ds_buf = nullptr;
#endif

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

        // Builds an ostringstream and sets its state accordingly to `fmt` string
        static auto build_ostream(std::string const& fmt) -> std::tuple<bool, std::ostringstream>
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

        template<typename T>
        explicit Tree(Formatter<T> const& v)
            : Tree {[&]
            {
                auto result_os = Tree::build_ostream(v.fmt);
                if (std::get<0>(result_os))
                    return Tree{v.v, std::move(std::get<1>(result_os))};
                else
                    return Tree::literal("*Error* on formatting string");
            }()}
        {}

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
        Tree(T const& value, std::ostringstream&& buf,
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
                buf << value;
                return buf.str();
            }()}
        {}

        // Print C string
        template <typename T>
        Tree(T const& value, std::ostringstream&& buf,
            typename std::enable_if<
                is_c_string<T>::value
            >::type* = nullptr
        )
            : Tree {InnerTag{}, [&]
            {
                if (show_c_string())
                {
                    using DT = typename std::decay<
                        typename std::remove_pointer<
                            typename std::decay<T>::type
                        >::type
                    >::type;

                    // On MacOS, an identity cv.to_bytes operation (between two identical
                    // character types) throws an exception. If they are the same, we do not
                    // make any conversion.
                    if (std::is_same<DT, std::ostringstream::char_type>::value)
                        buf << '"' << value << '"';
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
        Tree(T const& value, std::ostringstream&& buf,
            typename std::enable_if<
                is_std_string<T>::value
            >::type* = nullptr
        )
            : Tree {InnerTag{}, [&]
            {
                using VT = typename T::value_type;

                // On MacOS, an identity cv.to_bytes operation (between two identical
                // character types) throws an exception. If they are the same, we do not
                // make any conversion.
                if (std::is_same<VT, std::ostringstream::char_type>::value)
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
        Tree(T const& value, std::ostringstream&& buf,
            typename std::enable_if<
                is_string_view<T>::value
            >::type* = nullptr
        )
            : Tree {std::basic_string<typename T::value_type>{value}, std::move(buf)}
        {}
    #endif

        // Print character
        template <typename T>
        Tree(T const& value, std::ostringstream&& buf,
            typename std::enable_if<
                is_character<T>::value
            >::type* = nullptr
        )
            : Tree {InnerTag{}, [&]
            {
                using DT = typename std::decay<T>::type;
                using FF = typename FIX<DT>::type;
                std::wstring_convert<
                    deletable_facet<
                        std::codecvt<FF, std::ostringstream::char_type, std::mbstate_t>
                    >,
                    FF
                > cv {};

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
                    // On MacOS, an identity cv.to_bytes operation (between two identical
                    // character types) throws an exception. If they are the same, we do not
                    // make any conversion.
                    if (std::is_same<DT, std::ostringstream::char_type>::value)
                        // Innocuous cast, just to silence a warning with types where this line is not reached.
                        str = static_cast<std::ostringstream::char_type>(value);
                    else
                        str = cv.to_bytes(value);
                    break;
                }

                buf << '\'' << str << '\'';

                return buf.str();
            }()}
        {}

        // Print signed and unsigned char
        template <typename T>
        Tree(T const& value, std::ostringstream&& buf,
            typename std::enable_if<
                is_xsig_char<T>::value
            >::type* = nullptr
        )
            : Tree {InnerTag{}, [&]
            {
                using T0 = typename std::conditional<
                    std::is_signed<T>::value, int, unsigned int
                >::type;

                buf << static_cast<T0>(value);
                return buf.str();
            }()}
        {}

        // Print smart pointers without an operator<<(ostream&) overload.
        template <typename T>
        Tree(T const& value, std::ostringstream&& buf,
            typename std::enable_if<
                is_unstreamable_ptr<T>::value
                && !has_insertion<T>::value // On C++20 unique_ptr will have a << overload.
            >::type* = nullptr
        )
            : Tree {InnerTag{}, [&]
            {
                buf << reinterpret_cast<void const*>(value.get());
                return buf.str();
            }()}
        {}

        // Print weak pointer classes
        template <typename T>
        Tree(T const& value, std::ostringstream&& buf,
            typename std::enable_if<
                is_weak_ptr<T>::value
            >::type* = nullptr
        )
            : Tree {value.expired() ? Tree{InnerTag{}, "expired"} : Tree {value.lock(), std::move(buf)}}
        {}

    #if defined(ICECREAM_OPTIONAL_HEADER)
        // Print std::optional<> classes
        template <typename T>
        Tree(T const& value, std::ostringstream&& buf,
            typename std::enable_if<
                is_optional<T>::value
                && !has_insertion<T>::value
            >::type* = nullptr
        )
            : Tree {value.has_value() ? Tree{*value, std::move(buf)} : Tree{InnerTag{}, "nullopt"}}
        {}
    #endif

        struct Visitor
        {
            Visitor(std::ostringstream&& buf_)
                : buf{std::move(buf_)}
            {}

            template <typename T>
            auto operator()(T const& arg) -> Tree
            {
                return Tree {arg, std::move(this->buf)};
            }

            std::ostringstream buf;
        };

        // Print *::variant<Ts...> classes
        template <typename T>
        Tree(T const& value, std::ostringstream&& buf,
            typename std::enable_if<
                is_variant<T>::value
                && !has_insertion<T>::value
            >::type* = nullptr
        )
            : Tree {visit(Tree::Visitor{std::move(buf)}, value)}
        {}

        // Fill this->content.stem.children with all the tuple elements
        template<typename T, std::size_t N = std::tuple_size<T>::value-1>
        static auto tuple_traverser(T const& t, std::ostringstream&& buf) -> std::vector<Tree>
        {
            auto result = N > 0 ?
                tuple_traverser<T, (N > 0) ? N-1 : 0>(t, std::move(buf)) : std::vector<Tree> {};

            auto buf_ = std::ostringstream {};
            buf_.copyfmt(buf);

            result.emplace_back(std::get<N>(t), std::move(buf_));
            return result;
        }

        // Print tuple like classes
        template <typename T>
        Tree(T&& value, std::ostringstream&& buf,
            typename std::enable_if<
                is_tuple<T>::value
                && !has_insertion<T>::value
            >::type* = nullptr
        )
            : Tree {InnerTag{}, U::Stem{"(", ", ", ")", Tree::tuple_traverser(value, std::move(buf))}}
        {}

        // Print all items of any iterable class
        template <typename T>
        Tree(T const& value, std::ostringstream&& buf,
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
                        auto buf_ = std::ostringstream {};
                        buf_.copyfmt(buf);
                        result.emplace_back(i, std::move(buf_));
                    }
                    return result;
                }()}}
        {}

        // Print classes deriving from only std::exception and not from boost::exception
        template <typename T>
        Tree(T const& value, std::ostringstream&&,
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
        Tree(T const& value, std::ostringstream&&,
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
        template <typename T>
        Tree(T const& value, std::ostringstream&& buf,
            typename std::enable_if<
                std::is_standard_layout<T>::value
                && !is_collection<T>::value
                && !is_tuple<T>::value
                && !is_unstreamable_ptr<T>::value
                && !is_weak_ptr<T>::value
                && !is_std_string<T>::value
                && !is_string_view<T>::value
                && !is_variant<T>::value
                && !is_optional<T>::value
                && !has_insertion<T>::value
            >::type* = nullptr
        )
            : Tree {InnerTag{}, ""}
        {
            ds_this = this;
            ds_buf = &buf;
            __builtin_dump_struct(&value, &parse_struct_dump);
            ds_this = nullptr;
            ds_buf = nullptr;
        }
    #endif
    };

    // -------------------------------------------------- clang dump struct parsing

#if defined(ICECREAM_DUMP_STRUCT_CLANG)

    struct Tokens
    {
        struct TkArray
        {
            std::size_t Size = 0;

            TkArray() = default;
            TkArray(TkArray const&) = default;
            TkArray(TkArray&&) = default;
            TkArray& operator=(TkArray const&) = default;
            TkArray& operator=(TkArray&&) = default;

            TkArray(std::size_t size)
                : Size {size}
                , exist {true}
            {}

            explicit operator bool() const
            {
                return this->exist;
            }

        private:
            bool exist = false;
        };

        bool Bool = false;
        bool Int = false;

        bool Int8 = false;
        bool Int16 = false;
        bool Int32 = false;
        bool Int64 = false;
        bool Int_fast8 = false;
        bool Int_fast16 = false;
        bool Int_fast32 = false;
        bool Int_fast64 = false;
        bool Int_least8 = false;
        bool Int_least16 = false;
        bool Int_least32 = false;
        bool Int_least64 = false;
        bool Intmax = false;
        bool Intptr = false;

        bool Uint8 = false;
        bool Uint16 = false;
        bool Uint32 = false;
        bool Uint64 = false;
        bool Uint_fast8 = false;
        bool Uint_fast16 = false;
        bool Uint_fast32 = false;
        bool Uint_fast64 = false;
        bool Uint_least8 = false;
        bool Uint_least16 = false;
        bool Uint_least32 = false;
        bool Uint_least64 = false;
        bool Uintmax = false;
        bool Uintptr = false;

        bool Size_t = false;
        bool Ptrdiff = false;

        bool Double = false;
        bool Float = false;
        bool Char = false;
        bool Wchar = false;
        bool Char16 = false;
        bool Char32 = false;
        bool Signed = false;
        bool Unsigned = false;
        bool Short = false;
        int Long = 0;
        int Star = 0;
        bool Struct = false;
        bool Enum = false;
        TkArray Array {};
        int Unknown = 0;
        std::string Name = "";

        Tokens() = default;
        Tokens(Tokens const&) = default;
        Tokens(Tokens&&) = default;
        Tokens& operator=(Tokens const&) = default;
        Tokens& operator=(Tokens&&) = default;

        Tokens(std::string const& line)
        {
            static auto const& npos = std::string::npos;
            auto const line_size = line.size();
            auto left = std::string::size_type {0};
            auto right = std::string::size_type {0};

            auto lexemes = std::vector<std::string> {};
            while (true)
            {
                left = line.find_first_not_of(' ', left);
                if (left == npos) break;

                if (line.at(left) == '*')
                    right = left + 1;
                else
                    right = line.find_first_of(" *", left);
                right = right < line_size ? right : line_size;

                lexemes.push_back(line.substr(left, right-left));
                left = right;
            }

            this->Name = lexemes.back();
            lexemes.pop_back();
            for (auto const& t : lexemes)
            {
                if (t == "_Bool") this->Bool = true;
                else if (t == "int") this->Int = true;

                else if ((t == "std::int8_t") || (t == "int8_t")) this->Int8 = true;
                else if ((t == "std::int8_t") || (t == "int8_t")) this->Int8 = true;
                else if ((t == "std::int16_t") || (t == "int16_t")) this->Int16 = true;
                else if ((t == "std::int16_t") || (t == "int16_t")) this->Int16 = true;
                else if ((t == "std::int32_t") || (t == "int32_t")) this->Int32 = true;
                else if ((t == "std::int32_t") || (t == "int32_t")) this->Int32 = true;
                else if ((t == "std::int64_t") || (t == "int64_t")) this->Int64 = true;
                else if ((t == "std::int64_t") || (t == "int64_t")) this->Int64 = true;
                else if ((t == "std::int_fast8_t") || (t == "int_fast8_t")) this->Int_fast8 = true;
                else if ((t == "std::int_fast8_t") || (t == "int_fast8_t")) this->Int_fast8 = true;
                else if ((t == "std::int_fast16_t") || (t == "int_fast16_t")) this->Int_fast16 = true;
                else if ((t == "std::int_fast16_t") || (t == "int_fast16_t")) this->Int_fast16 = true;
                else if ((t == "std::int_fast32_t") || (t == "int_fast32_t")) this->Int_fast32 = true;
                else if ((t == "std::int_fast32_t") || (t == "int_fast32_t")) this->Int_fast32 = true;
                else if ((t == "std::int_fast64_t") || (t == "int_fast64_t")) this->Int_fast64 = true;
                else if ((t == "std::int_fast64_t") || (t == "int_fast64_t")) this->Int_fast64 = true;
                else if ((t == "std::int_least8_t") || (t == "int_least8_t")) this->Int_least8 = true;
                else if ((t == "std::int_least8_t") || (t == "int_least8_t")) this->Int_least8 = true;
                else if ((t == "std::int_least16_t") || (t == "int_least16_t")) this->Int_least16 = true;
                else if ((t == "std::int_least16_t") || (t == "int_least16_t")) this->Int_least16 = true;
                else if ((t == "std::int_least32_t") || (t == "int_least32_t")) this->Int_least32 = true;
                else if ((t == "std::int_least32_t") || (t == "int_least32_t")) this->Int_least32 = true;
                else if ((t == "std::int_least64_t") || (t == "int_least64_t")) this->Int_least64 = true;
                else if ((t == "std::int_least64_t") || (t == "int_least64_t")) this->Int_least64 = true;
                else if ((t == "std::intmax_t") || (t == "intmax_t")) this->Intmax = true;
                else if ((t == "std::intptr_t") || (t == "intptr_t")) this->Intptr = true;

                else if ((t == "std::uint8_t") || (t == "uint8_t")) this->Uint8 = true;
                else if ((t == "std::uint8_t") || (t == "uint8_t")) this->Uint8 = true;
                else if ((t == "std::uint16_t") || (t == "uint16_t")) this->Uint16 = true;
                else if ((t == "std::uint16_t") || (t == "uint16_t")) this->Uint16 = true;
                else if ((t == "std::uint32_t") || (t == "uint32_t")) this->Uint32 = true;
                else if ((t == "std::uint32_t") || (t == "uint32_t")) this->Uint32 = true;
                else if ((t == "std::uint64_t") || (t == "uint64_t")) this->Uint64 = true;
                else if ((t == "std::uint64_t") || (t == "uint64_t")) this->Uint64 = true;
                else if ((t == "std::uint_fast8_t") || (t == "uint_fast8_t")) this->Uint_fast8 = true;
                else if ((t == "std::uint_fast8_t") || (t == "uint_fast8_t")) this->Uint_fast8 = true;
                else if ((t == "std::uint_fast16_t") || (t == "uint_fast16_t")) this->Uint_fast16 = true;
                else if ((t == "std::uint_fast16_t") || (t == "uint_fast16_t")) this->Uint_fast16 = true;
                else if ((t == "std::uint_fast32_t") || (t == "uint_fast32_t")) this->Uint_fast32 = true;
                else if ((t == "std::uint_fast32_t") || (t == "uint_fast32_t")) this->Uint_fast32 = true;
                else if ((t == "std::uint_fast64_t") || (t == "uint_fast64_t")) this->Uint_fast64 = true;
                else if ((t == "std::uint_fast64_t") || (t == "uint_fast64_t")) this->Uint_fast64 = true;
                else if ((t == "std::uint_least8_t") || (t == "uint_least8_t")) this->Uint_least8 = true;
                else if ((t == "std::uint_least8_t") || (t == "uint_least8_t")) this->Uint_least8 = true;
                else if ((t == "std::uint_least16_t") || (t == "uint_least16_t")) this->Uint_least16 = true;
                else if ((t == "std::uint_least16_t") || (t == "uint_least16_t")) this->Uint_least16 = true;
                else if ((t == "std::uint_least32_t") || (t == "uint_least32_t")) this->Uint_least32 = true;
                else if ((t == "std::uint_least32_t") || (t == "uint_least32_t")) this->Uint_least32 = true;
                else if ((t == "std::uint_least64_t") || (t == "uint_least64_t")) this->Uint_least64 = true;
                else if ((t == "std::uint_least64_t") || (t == "uint_least64_t")) this->Uint_least64 = true;
                else if ((t == "std::uintmax_t") || (t == "uintmax_t")) this->Uintmax = true;
                else if ((t == "std::uintptr_t") || (t == "uintptr_t")) this->Uintptr = true;

                else if ((t == "std::size_t") || (t == "size_t")) this->Size_t = true;
                else if ((t == "std::ptrdiff_t") || (t == "ptrdiff_t")) this->Ptrdiff = true;

                else if (t == "double") this->Double = true;
                else if (t == "float") this->Float = true;
                else if (t == "char") this->Char = true;
                else if (t == "wchar_t") this->Wchar = true;
                else if (t == "char16_t") this->Char16 = true;
                else if (t == "char32_t") this->Char32 = true;
                else if (t == "signed") this->Signed = true;
                else if (t == "unsigned") this->Unsigned = true;
                else if (t == "long") this->Long++;
                else if (t == "short") this->Short = true;
                else if (t == "struct") this->Struct = true;
                else if (t == "enum") this->Enum = true;
                else if (t == "*") this->Star++;
                else if (t.front() == '[') this->Array = std::stoul(t.substr(1));
                else this->Unknown++;
            }
        }
    };

    // 1 - If T is a pointer result will be T.
    // 2.0 - If T is a [signed|unsigned] integral narrower than [signed|unsigned] int,
    //       then result will be [signed|unsigned] int. Otherwise result will be T.
    // 2.1 - if T is either double or long double, result will be T.
    template <typename T>
    using promoted_type = typename std::common_type<
        T,
        typename std::conditional<
            std::is_pointer<T>::value,
            T,
            typename std::conditional<
                std::is_signed<T>::value, int, unsigned int
            >::type
        >::type
    >::type;

    template <typename T>
    using transition_type = typename std::conditional<
        is_one_of<T, wchar_t, char16_t, char32_t>::value,
        std::ptrdiff_t,
        T
    >::type;

    template <typename T>
    static auto va_list_to_tree(va_list& args, std::size_t size) -> Tree
    {
        using T0 = promoted_type<T>;

        auto result = std::vector<T> {};
        for (std::size_t i = 0; i < size; ++i)
        {
            result.push_back(static_cast<T>(va_arg(args, T0)));
        }


        auto buf_ = std::ostringstream {};
        buf_.copyfmt(*ds_buf);
        return Tree{result, std::move(buf_)};
    }

    template <>
    auto va_list_to_tree<bool>(va_list& args, std::size_t size) -> Tree
    {
        auto result = std::vector<bool> {};
        for (std::size_t i = 0; i < size; ++i)
        {
            auto v = va_arg(args, int) & 0x01;
            result.push_back(static_cast<bool>(v));
        }

        auto buf_ = std::ostringstream {};
        buf_.copyfmt(*ds_buf);
        return Tree{result, std::move(buf_)};
    }

    template <>
    auto va_list_to_tree<float>(va_list& args, std::size_t size) -> Tree
    {
        auto result = std::vector<float> {};
        for (std::size_t i = 0; i < size;)
        {
            // See https://bugs.llvm.org/show_bug.cgi?id=45143
            // Hackish empirical workaround, found by inspecting the bit patterns in
            // memory. It works on the tested machines, but can or can not work on other
            // architectures or future Clang releases. Please report any problem.
            double d = va_arg(args, double);
            result.push_back(*reinterpret_cast<float*>(&d));
            ++i;

            if ((sizeof(void*)*CHAR_BIT == 32) && i < size)
            {
                result.push_back(*(reinterpret_cast<float*>(&d) + 1));
                ++i;
            }
        }
        auto buf_ = std::ostringstream {};
        buf_.copyfmt(*ds_buf);
        return Tree{result, std::move(buf_)};
    }

    template <typename T>
    static auto va_list_to_tree(va_list& args) -> Tree
    {
        using T0 = typename std::conditional<
            is_one_of<T, wchar_t, char16_t, char32_t>::value,
            void*,
            promoted_type<T>
        >::type;
        using T1 = transition_type<T>;

        auto buf_ = std::ostringstream {};
        buf_.copyfmt(*ds_buf);
        return Tree{(T)(T1)va_arg(args, T0), std::move(buf_)};
    }

    template <>
    auto va_list_to_tree<bool>(va_list& args) -> Tree
    {
        auto i = va_arg(args, int) & 0x01;

        auto buf_ = std::ostringstream {};
        buf_.copyfmt(*ds_buf);
        return Tree{static_cast<bool>(i), std::move(buf_)};
    }

    template <>
    auto va_list_to_tree<float>(va_list& args) -> Tree
    {
        // See https://bugs.llvm.org/show_bug.cgi?id=45143
        // Hackish empirical workaround, found by inspecting the bit patterns in
        // memory. It works on the tested machines, but can or can not work on other
        // architectures or future Clang releases. Please report any problem.
        auto d = va_arg(args, double);

        auto buf_ = std::ostringstream {};
        buf_.copyfmt(*ds_buf);
        return Tree{*reinterpret_cast<float*>(&d), std::move(buf_)};
    }

    static auto parse_array_dump(va_list& args, Tokens const& tokens) -> Tree
    {
    #define ICECREAM_CND(condition, type) \
        (condition) return va_list_to_tree<type>(args, tokens.Array.Size)

        auto const& t = tokens;

        if ICECREAM_CND(t.Star, void*);
        else if ICECREAM_CND(t.Bool, bool);

        else if ICECREAM_CND(t.Short && !t.Unsigned, short int);
        else if ICECREAM_CND(t.Short && t.Unsigned, unsigned short int);
        else if ICECREAM_CND(t.Int && !t.Unsigned && !t.Long, int);
        else if ICECREAM_CND(t.Signed && !t.Char && !t.Long, int);
        else if ICECREAM_CND(t.Unsigned && !t.Char && !t.Long, unsigned int);
        else if ICECREAM_CND((t.Long==1) && !t.Unsigned && !t.Double && !t.Float, long int);
        else if ICECREAM_CND((t.Long==1) && t.Unsigned, unsigned long int);
        else if ICECREAM_CND((t.Long==2) && !t.Unsigned, long long int);
        else if ICECREAM_CND((t.Long==2) && t.Unsigned, unsigned long long int);
        else if ICECREAM_CND(t.Char && !t.Unsigned && !t.Signed, char);
        else if ICECREAM_CND(t.Char && t.Signed, signed char);
        else if ICECREAM_CND(t.Char && t.Unsigned, unsigned char);

        else if ICECREAM_CND(t.Wchar, wchar_t);
        else if ICECREAM_CND(t.Char16, char16_t);
        else if ICECREAM_CND(t.Char32, char32_t);

        else if ICECREAM_CND(t.Float, float);
        else if ICECREAM_CND(t.Double && !t.Long, double);
        else if ICECREAM_CND(t.Double && t.Long, long double);

    #if defined(INT8_MAX)
        else if ICECREAM_CND(t.Int8, std::int8_t);
    #endif
    #if defined(INT16_MAX)
        else if ICECREAM_CND(t.Int16, std::int16_t);
    #endif
    #if defined(INT32_MAX)
        else if ICECREAM_CND(t.Int32, std::int32_t);
    #endif
    #if defined(INT64_MAX)
        else if ICECREAM_CND(t.Int64, std::int64_t);
    #endif
        else if ICECREAM_CND(t.Int_fast8, std::int_fast8_t);
        else if ICECREAM_CND(t.Int_fast16, std::int_fast16_t);
        else if ICECREAM_CND(t.Int_fast32, std::int_fast32_t);
        else if ICECREAM_CND(t.Int_fast64, std::int_fast64_t);
        else if ICECREAM_CND(t.Int_least8, std::int_least8_t);
        else if ICECREAM_CND(t.Int_least16, std::int_least16_t);
        else if ICECREAM_CND(t.Int_least32, std::int_least32_t);
        else if ICECREAM_CND(t.Int_least64, std::int_least64_t);
        else if ICECREAM_CND(t.Intmax, std::intmax_t);
    #if defined(INTPTR_MAX)
        else if ICECREAM_CND(t.Intptr, std::intptr_t);
    #endif

    #if defined(UINT8_MAX)
        else if ICECREAM_CND(t.Uint8, std::uint8_t);
    #endif
    #if defined(UINT16_MAX)
        else if ICECREAM_CND(t.Uint16, std::uint16_t);
    #endif
    #if defined(UINT32_MAX)
        else if ICECREAM_CND(t.Uint32, std::uint32_t);
    #endif
    #if defined(UINT64_MAX)
        else if ICECREAM_CND(t.Uint64, std::uint64_t);
    #endif
        else if ICECREAM_CND(t.Uint_fast8, std::uint_fast8_t);
        else if ICECREAM_CND(t.Uint_fast16, std::uint_fast16_t);
        else if ICECREAM_CND(t.Uint_fast32, std::uint_fast32_t);
        else if ICECREAM_CND(t.Uint_fast64, std::uint_fast64_t);
        else if ICECREAM_CND(t.Uint_least8, std::uint_least8_t);
        else if ICECREAM_CND(t.Uint_least16, std::uint_least16_t);
        else if ICECREAM_CND(t.Uint_least32, std::uint_least32_t);
        else if ICECREAM_CND(t.Uint_least64, std::uint_least64_t);
        else if ICECREAM_CND(t.Uintmax, std::uintmax_t);
    #if defined(UINTPTR_MAX)
        else if ICECREAM_CND(t.Uintptr, std::uintptr_t);
    #endif

        else if ICECREAM_CND(t.Size_t, std::size_t);
        else if ICECREAM_CND(t.Ptrdiff, std::ptrdiff_t);

        else if (t.Enum) return Tree::literal("<array of enums, see issue #7 on github>");
        else if (t.Struct) return Tree::literal("<array of structs, see issue #7 on github>");
        else return Tree::literal("<type aliased array, see issue #7 on github>");
    #undef ICECREAM_CND
    }

    static auto parse_attribute_dump(
        va_list& args, std::string const& spec, Tokens const& tokens
    ) -> Tree
    {
    #define ICECREAM_CND(spec_str, type) \
        (spec == spec_str) return va_list_to_tree<type>(args)
    #define ICECREAM_MSG(spec_str, message) \
        (spec == spec_str) return Tree::literal(message)

        if ICECREAM_CND("%c", char);
        else if ICECREAM_CND("%s", char*);
        else if ICECREAM_CND("%hhd", signed char);
        else if ICECREAM_CND("%hhi", signed char);
        else if ICECREAM_CND("%hd", short int);
        else if ICECREAM_CND("%hi", short int);
        else if ICECREAM_CND("%d" && tokens.Bool, bool);
        else if ICECREAM_CND("%i" && tokens.Bool, bool);
        else if ICECREAM_CND("%d" && !tokens.Bool, int);
        else if ICECREAM_CND("%i" && !tokens.Bool, int);
        else if ICECREAM_CND("%ld", long int);
        else if ICECREAM_CND("%li", long int);
        else if ICECREAM_CND("%lld", long long int);
        else if ICECREAM_CND("%lli", long long int);
        else if ICECREAM_CND("%jd", std::intmax_t);
        else if ICECREAM_CND("%ji", std::intmax_t);
        else if ICECREAM_MSG("%zd", "<unimplemented %zd specifiers>");
        else if ICECREAM_MSG("%zi", "<unimplemented %zi specifiers>");
        else if ICECREAM_CND("%td", std::ptrdiff_t);
        else if ICECREAM_CND("%ti", std::ptrdiff_t);
        else if ICECREAM_CND("%hhu", unsigned char);
        else if ICECREAM_CND("%hu", unsigned short int);
        else if ICECREAM_CND("%u", unsigned int);
        else if ICECREAM_CND("%lu", unsigned long int);
        else if ICECREAM_CND("%llu", unsigned long long int);
        else if ICECREAM_CND("%ju", std::uintmax_t);
        else if ICECREAM_CND("%zu", std::size_t);
        else if ICECREAM_MSG("%tu", "<unimplemented %tu specifiers>");
        else if ICECREAM_CND("%f" && tokens.Float, float);
        else if ICECREAM_CND("%f" && tokens.Double, double);
        else if (spec == "%f" && !(tokens.Float || tokens.Double))
        {
            auto t = va_list_to_tree<double>(args);
            return Tree::literal(t.leaf() + " <NOT RELIABLE, see issue #6 on github>");
        }
        else if ICECREAM_CND("%lf", double);
        else if ICECREAM_CND("%Lf", long double);

        // Clang delivers arrays and wide characters with a %p specifier. This implemented
        // heuristic solution works with canonical names only, aliased names will be
        // interpreted as pointers yet.
        else if (spec == "%p" && tokens.Array) return parse_array_dump(args, tokens);
        else if ICECREAM_CND("%p" && (tokens.Wchar && !tokens.Star), wchar_t);
        else if ICECREAM_CND("%p" && (tokens.Wchar && tokens.Star == 1), wchar_t*);
        else if ICECREAM_CND("%p" && (tokens.Char16 && !tokens.Star), char16_t);
        else if ICECREAM_CND("%p" && (tokens.Char16 && tokens.Star == 1), char16_t*);
        else if ICECREAM_CND("%p" && (tokens.Char32 && !tokens.Star), char32_t);
        else if ICECREAM_CND("%p" && (tokens.Char32 && tokens.Star == 1), char32_t*);
        else if ICECREAM_MSG("%p" && tokens.Enum, "<enum member, see issue #7 on github>");
        else if ICECREAM_CND("%p", void*);

        else ICECREAM_ASSERT(false, "Invalid va_arg code");

        // To silence a non returning warning
        return Tree::literal("");

    #undef ICECREAM_CND
    #undef ICECREAM_MSG
    }

    static inline auto parse_struct_dump(char const* format, ...) -> int
    {
        auto trim = [](std::string const& str) -> std::string
        {
            auto left = str.find_first_not_of(" \t\n");
            if (left == std::string::npos)
                return "";

            auto right = str.find_last_not_of(" \t\n");
            return str.substr(left, right-left+1);
        };

        va_list args;
        va_start(args, format);

        // Stack with trees being constructed. Each level is a set with the children of
        // trees under construction. The top level is the set of the tree currently being
        // constructed. Once it is closed, i.e. the token '}' is read, a new Tree object
        // will be built with all top elements as children and added to the level below.
        // The elements at index 0 are the elements of `ds_this` tree.
        static auto tree_stack = std::vector<std::vector<Tree>> {};

        static auto name_stack = std::vector<Tree> {};
        static auto tokens = Tokens {};

        auto line = trim(format);

        if(line.back() == '{') // "struct <type> {" the opening of a new struct
        {
            tree_stack.push_back(std::vector<Tree>{});
        }

        // "<qualifiers>* <type> <array>? <name> :" type and name of an attribute
        else if(line.back() == ':')
        {
            line.pop_back(); // Remove the trailing ':'
            tokens = Tokens{line};
            name_stack.push_back(Tree::literal(tokens.Name));
        }
        else if (line.front() == '%') // an attribute value
        {
            auto t_pair = std::vector<Tree> {};
            t_pair.push_back(std::move(name_stack.back()));
            t_pair.push_back(parse_attribute_dump(args, line, tokens));

            tree_stack.back().emplace_back("", ": ", "", std::move(t_pair));
            name_stack.pop_back();
        }
        else if (line == "}")  // The closing of an struct
        {
            auto children = std::move(tree_stack.back());
            tree_stack.pop_back();

            if (tree_stack.empty())
                *ds_this = Tree {"{", ", ", "}", std::move(children)};
            else
            {
                auto t_pair = std::vector<Tree> {};
                t_pair.push_back(std::move(name_stack.back()));
                t_pair.emplace_back("{", ", ", "}", std::move(children));

                tree_stack.back().emplace_back("", ": ", "", std::move(t_pair));
                name_stack.pop_back();
            }
        }

        va_end(args);
        return 0;
    }

#endif // ICECREAM_DUMP_STRUCT_CLANG


    // -------------------------------------------------- is_tree_argument

    template <typename T>
    auto is_tree_argument_impl(int) -> decltype (
        Tree {std::declval<T&>(), std::declval<std::ostringstream&&>()},
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
    struct is_printable: std::conditional<
        is_collection<T>::value,
            disjunction<
                conjunction<
                    is_tree_argument<T>,
                    is_printable<elements_type<T>>
                >,
                has_insertion<T>
            >,
            is_tree_argument<T>
    >::type {};

    template <typename... Ts>
    struct is_printable<FormatterPack<Ts...>&>: conjunction<
        is_printable<Ts>...
    > {};

    template <typename T0, typename T1>
    struct is_printable<std::pair<T0, T1>&>: conjunction<
        is_printable<T0>, is_printable<T1>
    > {};

    template <typename... Ts>
    struct is_printable<std::tuple<Ts...>&>: conjunction<
        is_printable<Ts>...
    > {};

#if defined(ICECREAM_VARIANT_HEADER)
    template <typename... Ts>
    struct is_printable<std::variant<Ts...>&>: conjunction<
        is_printable<Ts>...
    > {};
#endif

    template <typename... Ts>
    struct is_printable<boost::variant2::variant<Ts...>&>: conjunction<
        is_printable<Ts>...
    > {};


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
                    std::begin(arg_names), std::forward<Ts>(args)...
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

        static
        auto clean_variable_name(std::string const& var_name) -> std::string
        {
            auto b_it = std::begin(var_name);
            auto e_it = std::end(var_name) - 1;

            auto par_count = int{0};

            // Remove outer left parenthesis
            while (*b_it == ' ' || *b_it == '\t' || *b_it == '\n' || *b_it == '(')
            {
                if (*b_it == '(')
                    par_count += 1;
                ++b_it;
            }

            // Remove outer right parenthesis
            while (par_count > 0)
            {
                if (*e_it == ')')
                    par_count -= 1;
                --e_it;
            }

            // Remove right white spaces
            while (*e_it == ' ' || *e_it == '\t' || *e_it == '\n')
                --e_it;

            return std::string{b_it, e_it+1};
        }

        // Receive a string with a `icecream::f_("0v#4x", a, b, c)` call, and returns a vector with all the variable names.
        // In this example, a vector with ["a", "b", "c"].
        static
        auto split_variable_names(std::string const& var_name) -> std::vector<std::string>
        {
            auto b_it = std::begin(var_name);
            auto e_it = std::end(var_name) - 1;

            std::vector<std::string> result;

            // Find format string/value splitting comma
            {
                auto nesting_deep = int{0};
                b_it = e_it;
                do
                {
                    if (*b_it == ')')
                        nesting_deep += 1;
                    else if (*b_it == '(')
                        nesting_deep -= 1;
                    else if (*b_it == ',' && nesting_deep == 1)
                    {
                        result.push_back(clean_variable_name(std::string{b_it+1, e_it}));
                        e_it = b_it;
                    }

                    --b_it;
                } while (nesting_deep > 0);
                ++b_it;
            }

            return std::vector<std::string>{result.rbegin(), result.rend()};
        }

        auto build_forest(
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
            std::vector<std::string>::const_iterator arg_name,
            T&& arg_value,
            Ts&&... args_tail
        ) ->
            typename std::enable_if<
                is_formatter_pack<T>::value,
                std::vector<std::tuple<std::string, Tree>>
            >::type
        {
            auto forest = Icecream::build_forest(
                arg_name+1, std::forward<Ts>(args_tail)...
            );

            Icecream::fill_forest_from_tuple(
                Icecream::split_variable_names(Icecream::clean_variable_name(*arg_name)),
                arg_value.vs,
                forest
            );

            return forest;
        }

        template <typename T, typename... Ts>
        auto build_forest(
            std::vector<std::string>::const_iterator arg_name,
            T&& arg_value,
            Ts&&... args_tail
        ) ->
            typename std::enable_if<
                !is_formatter_pack<T>::value,
                std::vector<std::tuple<std::string, Tree>>
            >::type
        {
            auto forest = Icecream::build_forest(
                arg_name+1, std::forward<Ts>(args_tail)...
            );
            forest.emplace_back(
                Icecream::clean_variable_name(*arg_name),
                Tree{std::move(arg_value), std::ostringstream{}});
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
            std::vector<std::string> const& arg_names,
            Ts&&... args
        ) -> typename std::enable_if<
                detail::conjunction<detail::is_printable<Ts>...>::value
            >::type
        {
            detail::Icecream::instance().print(
                file, line, function, arg_names, std::forward<Ts>(args)...
            );
        }
    };

    static IcecreamAPI ic {};

    template<typename... Ts>
    auto f_(std::string const& fmt, Ts&&... vs) -> detail::FormatterPack<decltype(std::forward<Ts>(vs))...>
    {
        return detail::FormatterPack<decltype(std::forward<Ts>(vs))...>{fmt, std::forward<Ts>(vs)...};
    }

} // namespace icecream

namespace icecream{ namespace detail
{
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
        std::string const arg_names;

        // Used by compilers that expand an empyt __VA_ARGS__ in
        // Dispatcher{bla, #__VA_ARGS__} to Dispatcher{bla, ""}
        Dispatcher(
            std::string const& file_,
            int line_,
            std::string const& function_,
            std::string const& arg_names_
        )
            : file {file_}
            , line {line_}
            , function {function_}
            , arg_names {arg_names_}
        {}

        // Used by compilers that expand an empyt __VA_ARGS__ in
        // Dispatcher{bla, #__VA_ARGS__} to Dispatcher{bla, }
        Dispatcher(
            std::string const& file_,
            int line_,
            std::string const& function_
        )
            : file {file_}
            , line {line_}
            , function {function_}
            , arg_names {""}
        {}

        template <typename... Ts>
        auto print(Ts&&... args) -> void
        {
            auto split_names = std::vector<std::string> {};
            auto b_it = std::begin(arg_names);
            auto it = std::begin(arg_names);
            auto par_count = int {0};

            // Split the the arg_names
            if (!this->arg_names.empty())
                while (true)
                {
                    if (it == std::end(arg_names) || (*it == ',' && par_count == 0))
                    {
                        // Remove the trailing spaces
                        auto e_it = it - 1;
                        while (*e_it == ' ') --e_it;
                        ++e_it;

                        // Remove the leading spaces
                        while (*b_it == ' ') ++b_it;

                        split_names.emplace_back(b_it, e_it);
                        if (it != std::end(arg_names)) b_it = it + 1;
                    }
                    else if (*it == '(')
                    {
                        ++par_count;
                    }
                    else if (*it == ')')
                    {
                        --par_count;
                    }

                    if (it == std::end(arg_names))
                    {
                        break;
                    }
                    else
                    {
                        ++it;
                    }
                }

            ::icecream::ic.print(file, line, function, split_names, std::forward<Ts>(args)...);
        }

        // Return a std::tuple with all the args, flattening the content of any FormatterPack
        template <typename... Ts>
        auto ret(Ts&&... args) -> decltype(flatten_formatter_pack(std::forward<Ts>(args)...))
        {
            this->print(args...);
            return flatten_formatter_pack(std::forward<Ts>(args)...);
        }

        // Return the unique arg
        template <typename T>
        auto ret(T&& arg) -> T
        {
            this->print(arg);
            return std::forward<T>(arg);
        }

        // Return the flattened unique arg of FormatterPack
        template <typename T>
        auto ret(FormatterPack<T>&& arg) -> T
        {
            this->print(arg);
            return std::forward<T>(std::get<0>(arg.vs).v);
        }

        // Return a std::tuple with the content of the FormatterPack
        template <typename... Ts>
        auto ret(FormatterPack<Ts...>&& arg) -> std::tuple<Ts...>
        {
            this->print(arg);
            return flatten_formatter_pack(std::move(arg));
        }

        auto ret() -> void
        {
            this->print();
        }
    };

}} // namespace icecream::detail


#endif // ICECREAM_HPP_INCLUDED
