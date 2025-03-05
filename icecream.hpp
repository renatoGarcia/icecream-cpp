/*
 * Copyright (c) 2019-2025 The IceCream-Cpp Developers. See the AUTHORS file at the
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
#include <cerrno>
#include <climits>
#include <clocale>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cwchar>
#include <exception>
#include <functional>
#include <iomanip>
#include <ios>
#include <iostream>
#include <iterator>
#include <limits>
#include <memory>
#include <mutex>
#include <ostream>
#include <sstream>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>


#if !defined(__APPLE__) && (!defined(_LIBCPP_VERSION) || _LIBCPP_VERSION >= 15000)
    #define ICECREAM_CUCHAR_HEADER
    #include <cuchar>
#endif

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

#if defined(__has_include) && __has_include(<ranges>)
    #include <ranges>
    #if defined(__cpp_lib_ranges)
        #define ICECREAM_LIB_RANGES
    #endif
#endif

#if !defined(__APPLE__) && defined(__has_include) && __has_include(<source_location>)
    #include <source_location>
    #if defined(__cpp_lib_source_location)
        #define ICECREAM_SOURCE_LOCATION
    #endif
#endif

#if defined(__has_include) && __has_include(<format>)
    #include <format>
    #if defined(__cpp_lib_format) || (_LIBCPP_VERSION >= 170000 && __cplusplus >= 202002L)
        // libc++ just defines the '__cpp_lib_format' macro start from version 19. However
        // from version 17 it already implemens all functionalities that we need.
        #define ICECREAM_STL_FORMAT
    #endif
    #if defined(__cpp_lib_format_ranges)
        #define ICECREAM_STL_FORMAT_RANGES
    #endif
#endif

// ICECREAM_FMT is the macro which can be defined to enable {fmt} support regardless of
// any other condition.
//
// The "defined()" test for FMT_VERSION macro is an attempt to check if any {fmt} header
// was "#included" before the including of this Icecream-cpp header. If so, the {fmt}
// support will be enabled.
#if defined(ICECREAM_FMT) || defined(FMT_VERSION)
    // All the {fmt} headers from supoorted versions (5.0 and newer) will directly or
    // indirectly include a source file where FMT_VERSION is defined.
    #include <fmt/format.h>
    #define ICECREAM_FMT_ENABLED
#endif

// ICECREAM_RANGE_V3 is the macro which can be defined to enable range-v3 support
// regardless of any other condition.
//
// The "defined()" test for RANGES_V3_DETAIL_CONFIG_HPP macro is an attempt to check if
// any range-v3 header was "#included" before the including of this icecream-cpp header.
// If so, the range-v3 support will be enabled.
#if defined(ICECREAM_RANGE_V3) || defined(RANGES_V3_DETAIL_CONFIG_HPP)
    // The RANGES_V3_DETAIL_CONFIG_HPP is the include guard of the header
    // <range/v3/detail/config.hpp>. This header is present in all range-v3 releases, and
    // is direct or indirectly included by all other range-v3 headers, excepting a few
    // ones like <range/v3/version.hpp>. Because of that, checking by the definition of
    // RANGES_V3_DETAIL_CONFIG_HPP is a good way to determine if any range-v3 header was
    // previously included.

    #define ICECREAM_RANGE_V3_ENABLED
    #include <range/v3/version.hpp>
    #include <range/v3/view/transform.hpp>

    namespace icecream { namespace detail {
      #if RANGE_V3_VERSION <= 500
        namespace rv3v = ::ranges::view;
      #else
        namespace rv3v = ::ranges::views;
      #endif
    }}
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

// Returns the number of args of a callable on IC_A macro. To be able to measure
// 0 arguments, the first name of the input __VA_ARGS__ must be the callable
// itself. In other words, this macro will return one less than the size of its
// inputs.
#define ICECREAM_ARGS_SIZE(...) ICECREAM_EXPAND(ICECREAM_ARGS_SIZE_( \
    __VA_ARGS__, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22,         \
    21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7,         \
    6, 5, 4, 3, 2, 1, 0))
#define ICECREAM_ARGS_SIZE_(                                    \
    _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, \
    _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, \
    _26, _27, _28, _29, _30, _31, _32, N, ...) N

#define ICECREAM_UNPACK_0
#define ICECREAM_UNPACK_1 std::get<0>(std::move(ret_tuple))
#define ICECREAM_UNPACK_2 ICECREAM_UNPACK_1, std::get<1>(std::move(ret_tuple))
#define ICECREAM_UNPACK_3 ICECREAM_UNPACK_2, std::get<2>(std::move(ret_tuple))
#define ICECREAM_UNPACK_4 ICECREAM_UNPACK_3, std::get<3>(std::move(ret_tuple))
#define ICECREAM_UNPACK_5 ICECREAM_UNPACK_4, std::get<4>(std::move(ret_tuple))
#define ICECREAM_UNPACK_6 ICECREAM_UNPACK_5, std::get<5>(std::move(ret_tuple))
#define ICECREAM_UNPACK_7 ICECREAM_UNPACK_6, std::get<6>(std::move(ret_tuple))
#define ICECREAM_UNPACK_8 ICECREAM_UNPACK_7, std::get<7>(std::move(ret_tuple))
#define ICECREAM_UNPACK_9 ICECREAM_UNPACK_8, std::get<8>(std::move(ret_tuple))
#define ICECREAM_UNPACK_10 ICECREAM_UNPACK_9, std::get<9>(std::move(ret_tuple))
#define ICECREAM_UNPACK_11 ICECREAM_UNPACK_10, std::get<10>(std::move(ret_tuple))
#define ICECREAM_UNPACK_12 ICECREAM_UNPACK_11, std::get<11>(std::move(ret_tuple))
#define ICECREAM_UNPACK_13 ICECREAM_UNPACK_12, std::get<12>(std::move(ret_tuple))
#define ICECREAM_UNPACK_14 ICECREAM_UNPACK_13, std::get<13>(std::move(ret_tuple))
#define ICECREAM_UNPACK_15 ICECREAM_UNPACK_14, std::get<14>(std::move(ret_tuple))
#define ICECREAM_UNPACK_16 ICECREAM_UNPACK_15, std::get<15>(std::move(ret_tuple))
#define ICECREAM_UNPACK_17 ICECREAM_UNPACK_16, std::get<16>(std::move(ret_tuple))
#define ICECREAM_UNPACK_18 ICECREAM_UNPACK_17, std::get<17>(std::move(ret_tuple))
#define ICECREAM_UNPACK_19 ICECREAM_UNPACK_18, std::get<18>(std::move(ret_tuple))
#define ICECREAM_UNPACK_20 ICECREAM_UNPACK_19, std::get<19>(std::move(ret_tuple))
#define ICECREAM_UNPACK_21 ICECREAM_UNPACK_20, std::get<20>(std::move(ret_tuple))
#define ICECREAM_UNPACK_22 ICECREAM_UNPACK_21, std::get<21>(std::move(ret_tuple))
#define ICECREAM_UNPACK_23 ICECREAM_UNPACK_22, std::get<22>(std::move(ret_tuple))
#define ICECREAM_UNPACK_24 ICECREAM_UNPACK_23, std::get<23>(std::move(ret_tuple))
#define ICECREAM_UNPACK_25 ICECREAM_UNPACK_24, std::get<24>(std::move(ret_tuple))
#define ICECREAM_UNPACK_26 ICECREAM_UNPACK_25, std::get<25>(std::move(ret_tuple))
#define ICECREAM_UNPACK_27 ICECREAM_UNPACK_26, std::get<26>(std::move(ret_tuple))
#define ICECREAM_UNPACK_28 ICECREAM_UNPACK_27, std::get<27>(std::move(ret_tuple))
#define ICECREAM_UNPACK_29 ICECREAM_UNPACK_28, std::get<28>(std::move(ret_tuple))
#define ICECREAM_UNPACK_30 ICECREAM_UNPACK_29, std::get<29>(std::move(ret_tuple))
#define ICECREAM_UNPACK_31 ICECREAM_UNPACK_30, std::get<30>(std::move(ret_tuple))
#define ICECREAM_UNPACK_32 ICECREAM_UNPACK_31, std::get<31>(std::move(ret_tuple))

#define ICECREAM_APPLY_(fmt, argument_names, N, callable, ...)                         \
    [&]()                                                                              \
    {                                                                                  \
        auto ret_tuple = ::icecream::detail::ensure_tuple(                             \
            ICECREAM_EXPAND(ICECREAM_DISPATCH(true, fmt, argument_names, __VA_ARGS__)) \
        );                                                                             \
        (void) ret_tuple;                                                              \
        return callable(ICECREAM_UNPACK_##N);                                          \
    }()

#define ICECREAM_APPLY(fmt, argument_names, N, ...)                       \
    ICECREAM_EXPAND(ICECREAM_APPLY_(fmt, argument_names, N, __VA_ARGS__))

#define ICECREAM_DISPATCH(is_ic_apply, fmt, argument_names, ...)                                                      \
    ::icecream::detail::Dispatcher{                                                                                   \
        is_ic_apply, icecream_private_config_5f803a3bcdb4, __FILE__, __LINE__, ICECREAM_FUNCTION, fmt, argument_names \
    }.ret(__VA_ARGS__)

#if defined(ICECREAM_LONG_NAME)
    #define ICECREAM(...) ICECREAM_DISPATCH(false, "", #__VA_ARGS__, __VA_ARGS__)
    #define ICECREAM0() ICECREAM_DISPATCH(false, "", "")
    #define ICECREAM_F(fmt, ...) ICECREAM_DISPATCH(false, fmt, #__VA_ARGS__, __VA_ARGS__)
    #define ICECREAM_A(...) ICECREAM_APPLY("", #__VA_ARGS__, ICECREAM_ARGS_SIZE(__VA_ARGS__), __VA_ARGS__)
    #define ICECREAM_FA(fmt, ...) ICECREAM_APPLY(fmt, #__VA_ARGS__, ICECREAM_ARGS_SIZE(__VA_ARGS__), __VA_ARGS__)
    #define ICECREAM_(...) ::icecream::detail::make_formatting_argument(__VA_ARGS__)
    #define ICECREAM_V(...) ::icecream::detail::IC_V_(__VA_ARGS__).complete(icecream_private_config_5f803a3bcdb4, __LINE__, __FILE__, ICECREAM_FUNCTION)
    #define ICECREAM_FV(...) ::icecream::detail::IC_FV_(__VA_ARGS__).complete(icecream_private_config_5f803a3bcdb4, __LINE__, __FILE__, ICECREAM_FUNCTION)
    #define ICECREAM_CONFIG_SCOPE()                                                                            \
        auto const* const icecream_parent_config_5f803a3bcdb4 = &icecream_private_config_5f803a3bcdb4;         \
        ::icecream::detail::Config_ icecream_private_config_5f803a3bcdb4(icecream_parent_config_5f803a3bcdb4); \
        ::icecream::Config& icecream_public_config_5f803a3bcdb4 = icecream_private_config_5f803a3bcdb4;
    #define ICECREAM_CONFIG icecream_public_config_5f803a3bcdb4
#else
    #define IC(...) ICECREAM_DISPATCH(false, "", #__VA_ARGS__, __VA_ARGS__)
    #define IC0() ICECREAM_DISPATCH(false, "", "")
    #define IC_F(fmt, ...) ICECREAM_DISPATCH(false, fmt, #__VA_ARGS__, __VA_ARGS__)
    #define IC_A(...) ICECREAM_APPLY("", #__VA_ARGS__, ICECREAM_ARGS_SIZE(__VA_ARGS__), __VA_ARGS__)
    #define IC_FA(fmt, ...) ICECREAM_APPLY(fmt, #__VA_ARGS__, ICECREAM_ARGS_SIZE(__VA_ARGS__), __VA_ARGS__)
    #define IC_(...) ::icecream::detail::make_formatting_argument(__VA_ARGS__)
    #define IC_V(...) ::icecream::detail::IC_V_(__VA_ARGS__).complete(icecream_private_config_5f803a3bcdb4, __LINE__, __FILE__, ICECREAM_FUNCTION)
    #define IC_FV(...) ::icecream::detail::IC_FV_(__VA_ARGS__).complete(icecream_private_config_5f803a3bcdb4, __LINE__, __FILE__, ICECREAM_FUNCTION)
    #define IC_CONFIG_SCOPE()                                                                                  \
        auto const* const icecream_parent_config_5f803a3bcdb4 = &icecream_private_config_5f803a3bcdb4;         \
        ::icecream::detail::Config_ icecream_private_config_5f803a3bcdb4(icecream_parent_config_5f803a3bcdb4); \
        ::icecream::Config& icecream_public_config_5f803a3bcdb4 = icecream_private_config_5f803a3bcdb4;
    #define IC_CONFIG icecream_public_config_5f803a3bcdb4
#endif


#define ICECREAM_UNREACHABLE assert(((void)"Should not reach here. Please report the bug", false))


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

        template<class T> struct variant_size;

        template <typename R, typename Visitor, typename Variant>
        constexpr auto visit(Visitor&& vis, Variant&& var) -> R;
    }
}

namespace icecream{ namespace detail
{
    // To allow ADL to find custom `begin`, `end`, `size`, and `to_string` function overloads
    using std::begin;
    using std::end;
  #if defined(__cpp_lib_nonmember_container_access)
    using std::size;
  #endif
    using std::to_string;


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
    struct conjunction<T, Ts...>:
        std::conditional<
            T::value,
            conjunction<Ts...>,
            std::false_type
        >::type
    {};


    // -------------------------------------------------- disjunction

    // Logical OR
    template <typename... Ts>
    struct disjunction: std::false_type {};

    template <typename T, typename... Ts>
    struct disjunction<T, Ts...>:
        std::conditional<
            T::value,
            std::true_type,
            disjunction<Ts...>
        >::type
    {};


    // -------------------------------------------------- negation

    // Logical NOT
    template <typename T>
    using negation =
        typename std::conditional<
            T::value,
            std::false_type,
            std::true_type
        >::type;


    // -------------------------------------------------- remove_ref_t

    template <typename T>
    using remove_ref_t = typename std::remove_reference<T>::type;


    // -------------------------------------------------- remove_cvref_t

    template <typename T>
    using remove_cvref_t =
        typename std::remove_cv<typename std::remove_reference<T>::type>::type;


    // -------------------------------------------------- int_sequence

    // A call to `make_int_sequence<N>` will return a type `int_sequence<0, 1, 2, ..., N-1>`

    template <int...>
    struct int_sequence {};

    template <int N, int... S>
    struct int_sequence_generator: int_sequence_generator<N-1, N-1, S...> {};

    template <int... S>
    struct int_sequence_generator<0, S...>
    {
        typedef int_sequence<S...> type;
    };

    template <int N>
    using make_int_sequence = typename int_sequence_generator<N>::type;


    // -------------------------------------------------- is_bounded_array

    // Checks if T is an array with a known size.
    //
    // is_bounded_array<int[5]> will return true
    // is_bounded_array<int[]> will return false
    // is_bounded_array<int> will return false

    template <typename T>
    struct is_bounded_array_impl: std::false_type {};

    template <typename T, size_t N>
    struct is_bounded_array_impl<T[N]>: std::true_type {};

    template <typename T>
    using is_bounded_array = typename is_bounded_array_impl<T>::type;


    // -------------------------------------------------- is_invocable

    // Checks if T is nullary invocable, i.e.: the statement T() is valid.

    template <typename T>
    auto is_invocable_impl(int) -> decltype(std::declval<T&>()(), std::true_type{});

    template <typename T>
    auto is_invocable_impl(...) -> std::false_type;

    template <typename T>
    using is_invocable = decltype(is_invocable_impl<T>(0));


    // -------------------------------------------------- is_string_convertible

    // Checks if T is "string convertible", i.e.: is accepted as argument to a std::string
    // constructor.

    template <typename T>
    auto is_string_convertible_impl(int) -> decltype(std::string(std::declval<T&>()), std::true_type{});

    template <typename T>
    auto is_string_convertible_impl(...) -> std::false_type;

    template <typename T>
    using is_string_convertible = decltype(is_string_convertible_impl<T>(0));


    // -------------------------------------------------- returned_type

    // Returns the result type of nullary function

    template <typename T>
    auto returned_type_impl(int) -> decltype(std::declval<T&>()());

    template <typename T>
    auto returned_type_impl(...) -> void;

    template <typename T>
    using returned_type = decltype(returned_type_impl<T>(0));


    // -------------------------------------------------- resolve_view_t

    // Applies the pipe operator between a range and a value of type T, and returns the
    // resulting view type.

    template <typename T>
    auto resolve_view_t_impl(int) -> decltype(std::declval<std::vector<int>&>() | std::declval<T&>());

    template <typename T>
    auto resolve_view_t_impl(...) -> void;

    template <typename T>
    using resolve_view_t = decltype(resolve_view_t_impl<T>(0));


    // -------------------------------------------------- is_sized

    // Checks if a class `T` has either a `size()` method or a `size(T const&)` overload.

    template <typename T>
    auto has_size_method_impl(int) ->
        decltype(
            std::declval<T&>().size(),
            std::true_type{}
        );

    template <typename T>
    auto has_size_method_impl(...) -> std::false_type;

    template <typename T>
    auto has_size_overload_impl(int) ->
        decltype(
            size(std::declval<T&>()),
            std::true_type{}
        );

    template <typename T>
    auto has_size_overload_impl(...) -> std::false_type;

    template <typename T>
    using is_sized =
        typename disjunction<
            decltype(has_size_method_impl<T>(0)),
            decltype(has_size_overload_impl<T>(0))
        >::type;

    template <typename T>
    using has_size_method = decltype(has_size_method_impl<T>(0));

    template <typename T>
    using has_size_function_overload = decltype(has_size_overload_impl<T>(0));


    // -------------------------------------------------- is_sentinel_for

    // Checks if `S` is a sentinel type to an iterator `I`

    template <typename S, typename I>
    auto is_sentinel_for_impl(int) ->
        decltype (
            S(std::declval<S&>()),
            std::declval<S&>() = std::declval<S&>(),
            std::declval<I&>() != std::declval<S&>(),
            std::declval<I&>() == std::declval<S&>(),
            std::true_type{}
        );

    template <typename S, typename I>
    auto is_sentinel_for_impl(...) -> std::false_type;

    template <typename S, typename I>
    using is_sentinel_for = decltype(is_sentinel_for_impl<S, I>(0));


    // -------------------------------------------------- is_range

    // Checks if the type `R` is a range.

    template <typename R>
    auto is_range_impl(int) ->
        decltype (
            begin(std::declval<R&>()),
            end(std::declval<R&>()),
            std::true_type{}
        );

    template <typename R>
    auto is_range_impl(...) -> std::false_type;

    template <typename R>
    using is_range = decltype(is_range_impl<R>(0));


    // -------------------------------------------------- get_iterator_t

    // Gets the iterator type of a range `R`

    template <typename R>
    using get_iterator_t = decltype(begin(std::declval<R&>()));


    // -------------------------------------------------- get_reference_t

    // Gets the reference type (the derreference result) of an iterator `I`

    template <typename I>
    using get_reference_t = decltype(*std::declval<I&>());


    // -------------------------------------------------- is_input_iterator

    template <typename T>
    auto is_input_iterator_impl(int) ->
        decltype (
            *std::declval<T&>(),    // is dereferenciable
            ++std::declval<T&>(),   // is pre-incrementable
            std::true_type{}
        );

    template <typename T>
    auto is_input_iterator_impl(...) -> std::false_type;

    template <typename T>
    using is_input_iterator = decltype(is_input_iterator_impl<T>(0));


    // -------------------------------------------------- is_input_range

    template <typename T>
    using is_input_range =
        typename conjunction<
            is_range<T>,
            is_input_iterator<get_iterator_t<T>>
        >::type;


    // -------------------------------------------------- is_forward_iterator

    template <typename I>
    auto is_forward_iterator_impl(int) ->
        decltype (
            I(std::declval<I&>()),   // is copyable
            I(),                     // is default initializable
            std::declval<I&>() == std::declval<I&>(),   // is copyable
            std::declval<I&>() != std::declval<I&>(),   // is copyable
            std::true_type{}
        );

    template <typename I>
    auto is_forward_iterator_impl(...) -> std::false_type;

    template <typename I>
    using is_forward_iterator =
        typename conjunction<
            is_input_iterator<I>,
            is_sentinel_for<I, I>,
            decltype(is_forward_iterator_impl<I>(0))
        >::type;


    // -------------------------------------------------- is_forward_range

    template <typename R>
    using is_forward_range =
        typename conjunction<
            is_input_range<R>,
            is_forward_iterator<get_iterator_t<R>>
        >::type;


    // -------------------------------------------------- is_bidirectional_iterator

    template <typename I>
    auto is_bidirectional_iterator_impl(int) ->
        decltype (
            --std::declval<I&>(),
            std::declval<I&>()--,
            std::true_type{}
        );

    template <typename I>
    auto is_bidirectional_iterator_impl(...) -> std::false_type;

    template <typename I>
    using is_bidirectional_iterator =
        typename conjunction<
            is_forward_iterator<I>,
            decltype(is_bidirectional_iterator_impl<I>(0))
        >::type;


    // -------------------------------------------------- is_bidirectional_range

    template <typename R>
    using is_bidirectional_range =
        typename conjunction<
            is_forward_range<R>,
            is_bidirectional_iterator<get_iterator_t<R>>
        >::type;


    // -------------------------------------------------- has_push_back_T

    // Checks if the `C` class has a push_back(`T`) method

    template <typename C, typename T>
    auto has_push_back_T_impl(int) ->
        decltype(
            std::declval<C&>().push_back(std::declval<T&>()),
            std::true_type{}
        );

    template <typename C, typename T>
    auto has_push_back_T_impl(...) -> std::false_type;

    template <typename C, typename T>
    using has_push_back_T = decltype(has_push_back_T_impl<C, T>(0));


    // --------------------------------------------------is_streamable

    // Checks if `T` has an insertion overload, i.e.: `std::ostream& << T&`

    template <typename T>
    auto is_streamable_impl(int) ->
        decltype(
            std::declval<std::ostream&>() << std::declval<T&>(),
            std::true_type{}
        );

    template <typename T>
    auto is_streamable_impl(...) -> std::false_type;

    template <typename T>
    using is_streamable = decltype(is_streamable_impl<T>(0));


    // --------------------------------------------------is_stl_formattable

    // Checks if type `T` is formattable by STL Formatting library

  #if defined(ICECREAM_STL_FORMAT)
    template <class T>
    auto is_stl_formattable_impl(int) ->
        decltype(
            std::formatter<T, char>{},
            std::true_type{}
        );
  #endif

    template <class T>
    auto is_stl_formattable_impl(...) -> std::false_type ;

    template <class T>
    using is_stl_formattable = decltype(is_stl_formattable_impl<remove_cvref_t<T>>(0));


    // --------------------------------------------------is_fmt_formattable

    // Checks if type `T` is formattable by {fmt} library

  #if defined(ICECREAM_FMT_ENABLED)
    template <class T>
    auto is_fmt_formattable_impl(int) ->
        decltype(
            fmt::formatter<T, char>{},
            std::true_type{}
        );
  #endif

    template <class T>
    auto is_fmt_formattable_impl(...) -> std::false_type ;

    template <class T>
    using is_fmt_formattable = decltype(is_fmt_formattable_impl<remove_cvref_t<T>>(0));


    // --------------------------------------------------is_baseline_printable

    template <typename T>
    using is_baseline_printable =
        typename disjunction<
        is_streamable<T>, is_stl_formattable<T>, is_fmt_formattable<T>
        >::type;


    // -------------------------------------------------- has_to_string

    // Checks if there is a `size(T const&)` overload to a `T` type.

    template <typename T>
    auto has_to_string_impl(int) ->
        decltype(
            to_string(std::declval<T&>()),
            std::true_type{}
        );

    template <typename T>
    auto has_to_string_impl(...) -> std::false_type;

    template <typename T>
    using has_to_string = decltype(has_to_string_impl<T>(0));


    // -------------------------------------------------- is_tuple

    // Checks if `T` is a tuple like type, i.e.: an instantiation of one of
    // std::pair<typename U0, typename U1> or std::tuple<typename... Us>.

    template <typename T>
    using is_tuple =
        typename disjunction<
            is_instantiation<std::pair, T>,
            is_instantiation<std::tuple, T>
        >::type;


    // -------------------------------------------------- is_character

    // Checks if T is character type [const, volatile]?[char, wchar_t, char8_t, char16_t, char32].

    template <typename T>
    using is_character =
        typename disjunction<
            std::is_same<typename std::remove_cv<T>::type, char>,
            std::is_same<typename std::remove_cv<T>::type, wchar_t>,
          #if defined(__cpp_char8_t)
            std::is_same<typename std::remove_cv<T>::type, char8_t>,
          #endif
            std::is_same<typename std::remove_cv<T>::type, char16_t>,
            std::is_same<typename std::remove_cv<T>::type, char32_t>
        >::type;


    // -------------------------------------------------- is_xsig_char

    template <typename T>
    using is_xsig_char =
        typename disjunction<
            std::is_same<typename std::remove_cv<T>::type, signed char>,
            std::is_same<typename std::remove_cv<T>::type, unsigned char>
        >::type;


    // -------------------------------------------------- is_c_string

    // Checks if T is a C string type, i.e.: either char*, a char[], or a char[N]; of any
    // character type

    template <typename T>
    using is_c_string =
        typename disjunction<
            conjunction<
                std::is_pointer<T>, is_character<typename std::remove_pointer<T>::type>
            >,
            conjunction<
                std::is_array<T>, is_character<typename std::remove_extent<T>::type>
            >
        >::type;


    // -------------------------------------------------- is_std_string

    // Checks if T is a std::basic_string<typename U>

    template <typename T>
    using is_std_string =
        typename is_instantiation<std::basic_string, typename std::remove_cv<T>::type>::type;


    // -------------------------------------------------- is_string_view

    // Checks if T is a std::basic_string_view<typename U>

    template <typename T>
    using is_string_view =
        typename disjunction<
          #if defined(ICECREAM_STRING_VIEW_HEADER)
            is_instantiation<std::basic_string_view, typename std::remove_cv<T>::type>
          #endif
        >::type;


    // -------------------------------------------------- bypass_baseline_printing

    // Checks if a type T would be baseline printable, but uses a custom strategy instead

    template <typename T>
    using bypass_baseline_printing =
        typename disjunction<
            is_character<remove_ref_t<T>>,
            is_c_string<remove_ref_t<T>>,
            is_xsig_char<remove_ref_t<T>>,
            is_std_string<remove_ref_t<T>>,
            is_string_view<remove_ref_t<T>>,
            std::is_array<remove_ref_t<T>>
        >::type;


    // -------------------------------------------------- is_variant

    // Checks if T is a variant type.

    template <typename T>
    using is_variant =
        typename disjunction<
            is_instantiation<boost::variant2::variant, typename std::remove_cv<T>::type>
          #if defined(ICECREAM_VARIANT_HEADER)
            , is_instantiation<std::variant, typename std::remove_cv<T>::type>
          #endif
        >::type;


    // -------------------------------------------------- variant_size

    template <typename... Ts>
    struct variant_size;

  #if defined(ICECREAM_VARIANT_HEADER)
    template <typename... Ts>
    struct variant_size<std::variant<Ts...>>
    {
        static size_t const value = std::variant_size_v<std::variant<Ts...>>;
    };
  #endif

    template <typename... Ts>
    struct variant_size<boost::variant2::variant<Ts...>>
    {
        static size_t const value =
            boost::variant2::variant_size<boost::variant2::variant<Ts...>>::value;
    };


    // -------------------------------------------------- is_optional

    // Checks if T is an optional type.

    template <typename T>
    using is_optional =
        typename disjunction<
          #if defined(ICECREAM_OPTIONAL_HEADER)
            is_instantiation<std::optional, typename std::remove_cv<T>::type>
          #endif
        >::type;


    // -------------------------------------------------- is_not_streamable_ptr

    // Checks if T is either std::unique_ptr<typename U> instantiation (Until C++20), or a
    // boost::scoped_ptr<typename U>. Both are without an operator<<(ostream&) overload.

    template <typename T>
    using is_unstreamable_ptr =
        typename disjunction<
            is_instantiation<std::unique_ptr, typename std::remove_cv<T>::type>,
            is_instantiation<boost::scoped_ptr, typename std::remove_cv<T>::type>
        >::type;


    // -------------------------------------------------- is_weak_ptr

    // Checks if T is a instantiation if either std::weak_ptr<typename U> or
    // boost::weak_ptr<typename U>.

    template <typename T>
    using is_weak_ptr =
        typename disjunction<
            is_instantiation<std::weak_ptr, typename std::remove_cv<T>::type>,
            is_instantiation<boost::weak_ptr, typename std::remove_cv<T>::type>
        >::type;


    // -------------------------------------------------- is_valid_prefix

    // Checks if T can be used as prefix, i.e.: T is a string or a nullary function
    // returning a type that has a "ostream <<" overload.

    template <typename T>
    using is_valid_prefix =
        typename disjunction<
            is_std_string<remove_ref_t<T>>,
            is_string_view<remove_ref_t<T>>,
            is_c_string<remove_ref_t<T>>,
            conjunction<
                is_invocable<T>,
                is_streamable<returned_type<T>>
            >
        >::type;


    // -------------------------------------------------- is_T_output_iterator

    // Checks if `Iterator` is an output iterator with type `Item`

    template <typename Iterator, typename Item>
    auto is_T_output_iterator_impl(int) ->
        decltype(
            *std::declval<Iterator&>() = std::declval<Item&>(),
            std::true_type{}
        );

    template <typename Iterator, typename Item>
    auto is_T_output_iterator_impl(...) -> std::false_type;

    template <typename Iterator, typename Item>
    using is_T_output_iterator = decltype(is_T_output_iterator_impl<Iterator, Item>(0));


    // -------------------------------------------------- is_handled_by_clang_dump_struct

    template <typename T>
    using is_handled_by_clang_dump_struct =
        typename negation<
            disjunction<
                is_range<T>,
                is_tuple<remove_cvref_t<T>>,
                is_unstreamable_ptr<remove_ref_t<T>>,
                is_weak_ptr<remove_ref_t<T>>,
                is_std_string<remove_ref_t<T>>,
                is_string_view<remove_ref_t<T>>,
                is_variant<remove_ref_t<T>>,
                is_optional<remove_ref_t<T>>,
                is_baseline_printable<T>,
                is_character<remove_ref_t<T>>,
                is_c_string<remove_ref_t<T>>,
                std::is_base_of<std::exception, remove_cvref_t<T>>,
                std::is_base_of<boost::exception, remove_cvref_t<T>>
            >
        >::type;


    // -------------------------------------------------- ensure_tuple

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


    // -------------------------------------------------- Identity

    struct Identity
    {
        template <typename T>
        auto operator()(T&& t) const -> T&&
        {
            return std::forward<T>(t);
        }
    };


    // -------------------------------------------------- min

    template <typename T>
    auto min(T const& lho, T const& rho) -> T const&
    {
        return (rho < lho) ? rho : lho;
    }


    // -------------------------------------------------- StringView

    template <typename CharT>
    class BasicStringView
    {
    public:
        static constexpr size_t npos = size_t(-1);

        using traits_type = std::char_traits<CharT>;
        using value_type = CharT;
        using iterator = CharT const*;
        using reverse_iterator = std::reverse_iterator<iterator>;

        BasicStringView() = default;

        BasicStringView(value_type const* s, size_t count)
            : s_{s}
            , count_{count}
        {}

        BasicStringView(value_type const* s)
            : s_{s}
            , count_{traits_type::length(s)}
        {}

        BasicStringView(std::basic_string<CharT> const& s)
            : s_{s.data()}
            , count_{s.size()}
        {}

      #if defined(ICECREAM_STRING_VIEW_HEADER)
        BasicStringView(std::basic_string_view<value_type> s)
            : s_{s.data()}
            , count_{s.size()}
        {}
      #endif

        BasicStringView(iterator first, iterator last)
            : s_{first}
            , count_{static_cast<size_t>(last - first)}
        {}

        auto empty() const -> bool
        {
            return this->count_ == 0;
        }

        auto size() const -> size_t
        {
            return this->count_;
        }

        auto data() const -> value_type const*
        {
            return this->s_;
        }

        auto begin() const -> iterator
        {
            return this->s_;
        }

        auto end() const -> iterator
        {
            return this->s_ + this->count_;
        }

        auto rbegin() const -> reverse_iterator
        {
            return reverse_iterator(this->s_ + this->count_);
        }

        auto rend() const -> reverse_iterator
        {
            return reverse_iterator(this->s_);
        }

        auto operator[](size_t idx) const -> value_type
        {
            return this->s_[idx];
        }

        auto front() const -> value_type
        {
            return this->s_[0];
        }

        auto back() const -> value_type
        {
            return this->s_[this->count_ - 1];
        }

        friend auto operator==(BasicStringView lho, BasicStringView rho) -> bool
        {
            if (lho.count_ != rho.count_) return false;
            return traits_type::compare(lho.s_, rho.s_, lho.count_) == 0;
        }

        auto remove_prefix(size_t n) -> void
        {
            this->s_ += n;
            this->count_ -= n;
        }

        auto remove_suffix(size_t n) -> void
        {
            this->count_ -= n;
        }

        auto substr(size_t pos = 0, size_t count = npos) const -> BasicStringView
        {
            return BasicStringView(this->s_ + pos, min(count, this->count_ - pos));
        }

        auto find(value_type const* s, size_t pos = 0) const -> size_t
        {
            if (*s == '\0')
            {
                return 0;  // Empty string is always found at index 0
            }

            for (auto i = pos; i < this->count_; ++i)
            {
                if (this->s_[i] == s[0])
                {
                    auto j = size_t{1};
                    while (i + j < this->count_ && this->s_[i + j] == s[j] && s[j] != '\0')
                    {
                        ++j;
                    }

                    // If we reached the end of input string s it was found
                    if (s[j] == '\0')
                    {
                        return i;  // Return the starting index
                    }
                }
            }

            return npos; // if not found
        }

        auto rfind(value_type ch, size_t pos = npos) const -> size_t
        {
            auto const size = this->count_;

            if (size == 0) return npos;

            auto idx = pos < size ? pos : size;

            for (++idx; idx-- > 0;)
            {
                if (this->s_[idx] == ch) return idx;
            }

            return npos;
        }

        auto trim() -> void
        {
            if (this->count_ == 0) return;

            auto* c0 = this->s_;
            auto* c1 = this->s_ + this->count_;

            while (
                c0 < c1
                && (*c0 == ' ' || *c0 == '\t' || *c0 == '\n' || *c0 == '\r' || *c0 == '\f' || *c0 == '\v')
            ){
                ++c0;
            }

            while (
                c1 > c0
                && (
                    c1[-1] == ' '
                    || c1[-1] == '\t'
                    || c1[-1] == '\n'
                    || c1[-1] == '\r'
                    || c1[-1] == '\f'
                    || c1[-1] == '\v'
                )
            ){
                --c1;
            }

            this->s_ = c0;
            this->count_ = c1 - c0;
        };

        auto to_string() const -> std::basic_string<CharT>
        {
            return std::basic_string<CharT>(this->s_, this->count_);
        }

    private:
        value_type const* s_ = nullptr;
        size_t count_ = 0;
    };

    using StringView = BasicStringView<char>;
    using WStringView = BasicStringView<wchar_t>;
  #if defined(__cpp_char8_t)
    using U8StringView = BasicStringView<char8_t>;
  #endif
    using U16StringView = BasicStringView<char16_t>;
    using U32StringView = BasicStringView<char32_t>;

    // -------------------------------------------------- Variant

    template <typename T0, typename T1, typename T>
    struct GetHelper;

    template <typename T0, typename T1>
    class Variant
    {
        template <typename U0, typename U1, typename U2>
        friend struct GetHelper;

    private:
        std::size_t type_index;

        union
        {
            T0 as_t0;
            T1 as_t1;
        };

    public:
        Variant()
            : type_index(0)
            , as_t0()
        {}

        Variant(T0 const& v)
            : type_index(0)
            , as_t0(v)
        {}

        Variant(T1 const& v)
            : type_index(1)
            , as_t1(v)
        {}

        Variant(T0&& v)
            : type_index(0)
            , as_t0(std::move(v))
        {}

        Variant(T1&& v)
            : type_index(1)
            , as_t1(std::move(v))
        {}

        Variant(Variant<T0, T1> const& v)
            : type_index(v.type_index)
        {
            switch (this->type_index)
            {
            case 0:
                new (&this->as_t0) T0(v.as_t0);
                break;
            case 1:
                new (&this->as_t1) T1(v.as_t1);
                break;
            default:
                ICECREAM_UNREACHABLE;
            }
        }

        Variant(Variant<T0, T1>&& v)
            : type_index(v.type_index)
        {
            switch (this->type_index)
            {
            case 0:
                new (&this->as_t0) T0(std::move(v.as_t0));
                break;
            case 1:
                new (&this->as_t1) T1(std::move(v.as_t1));
                break;
            default:
                ICECREAM_UNREACHABLE;
            }
        }

        ~Variant()
        {
            switch (this->type_index)
            {
            case 0:
                this->as_t0.~T0();
                break;
            case 1:
                this->as_t1.~T1();
                break;
            default:
                ICECREAM_UNREACHABLE;
            }
        }

        Variant& operator=(Variant&& other)
        {
            if (this != &other)
            {
                this->~Variant();
                new (this) Variant(std::move(other));
            }

            return *this;
        }

        auto index() const -> std::size_t
        {
            return this->type_index;
        }
    };

    template <typename T0, typename T1>
    struct GetHelper<T0, T1, T0>
    {
        auto operator()(Variant<T0, T1> const& v) -> T0 const&
        {
            return v.as_t0;
        }

        auto operator()(Variant<T0, T1>& v) -> T0&
        {
            return v.as_t0;
        }

        auto operator()(Variant<T0, T1>&& v) -> T0&
        {
            return v.as_t0;
        }
    };

    template <typename T0, typename T1>
    struct GetHelper<T0, T1, T1>
    {
        auto operator()(Variant<T0, T1> const& v) -> T1 const&
        {
            return v.as_t1;
        }

        auto operator()(Variant<T0, T1>& v) -> T1&
        {
            return v.as_t1;
        }

        auto operator()(Variant<T0, T1>&& v) -> T1&
        {
            return v.as_t1;
        }
    };

    template <typename T, typename T0, typename T1>
    auto get(Variant<T0, T1>& v) -> T&
    {
        return GetHelper<T0, T1, T>()(v);
    }

    template <typename T, typename T0, typename T1>
    auto get(Variant<T0, T1> const& v) -> T const&
    {
        return GetHelper<T0, T1, T>()(v);
    }

    template <typename T, typename T0, typename T1>
    auto get(Variant<T0, T1>&& v) -> T&&
    {
        return GetHelper<T0, T1, T>()(std::move(v));
    }


    // -------------------------------------------------- Optional

    template <typename T>
    class Optional
    {
    public:
        Optional() = default;

        Optional(T const& v)
            : storage(v)
        {}

        Optional(T&& v)
            : storage(std::move(v))
        {}

        Optional(Optional<T> const& v) = default;

        Optional(Optional<T>&& v) = default;

        Optional& operator=(Optional&& other)
        {
            if (this != &other)
            {
                this->storage = std::move(other.storage);
            }

            return *this;
        }

        operator bool() const
        {
            return this->storage.index() != 0;
        }

        auto value() const -> T const&
        {
            return get<T>(this->storage);
        }

        auto operator*() -> T&
        {
            return get<T>(this->storage);
        }

        auto operator*() const -> T const&
        {
            return get<T>(this->storage);
        }

        auto operator->() -> T*
        {
            return &get<T>(this->storage);
        }

    private:
        struct empty {};

        Variant<empty, T> storage;
    };


    template <typename T>
    class Optional<T&>
    {
    public:
        Optional() = default;

        Optional(T& v)
            : storage(v)
        {}

        Optional(T&& v)
            : storage(std::move(v))
        {}

        Optional(Optional<T&> const& v) = default;

        Optional(Optional<T&>&& v) = default;

        Optional& operator=(Optional&& other)
        {
            if (this != &other)
            {
                this->storage = std::move(other.storage);
            }

            return *this;
        }

        operator bool() const
        {
            return this->storage.index() != 0;
        }

        auto value() const -> T const&
        {
            return get<std::reference_wrapper<T>>(this->storage);
        }

        auto operator*() -> T&
        {
            return get<std::reference_wrapper<T>>(this->storage);
        }

        auto operator*() const -> T const&
        {
            return get<std::reference_wrapper<T>>(this->storage);
        }

        auto operator->() -> T*
        {
            return &(get<std::reference_wrapper<T>>(this->storage).get());
        }

    private:
        struct empty {};

        Variant<empty, std::reference_wrapper<T>> storage;
    };


    // -------------------------------------------- Forward declare all make_printing_branch overloads

    // This is required because the declarations must be visible by some implementation
    // that can need to delegate the printing to other overload. For example when a
    // printing a std::vector<std::tuple<int, float>>>, fist the range implementation will
    // be called, then it will delegate the printing to the tuple overload, then finally
    // it will delegate the printing to the operator<<(std::ostream&, T) overload.

    class Config_;
    class PrintingNode;

    template <typename T>
    auto do_print_integral(
        T const&, Config_ const&, std::ostringstream&
    ) -> typename std::enable_if<!std::is_integral<remove_cvref_t<T>>::value, PrintingNode>::type;

    template <typename T>
    auto do_print_integral(
        T const&, Config_ const&, std::ostringstream&
    ) -> typename std::enable_if<std::is_integral<remove_cvref_t<T>>::value, PrintingNode>::type;

    // Print any class that overloads operator<<(std::ostream&, T)
    template <typename T>
    auto make_printing_branch(
        T&&, StringView, Config_ const&
    ) ->
        typename std::enable_if<
            is_streamable<T>::value
            && !is_stl_formattable<T>::value
            && !is_fmt_formattable<T>::value
            && !bypass_baseline_printing<T>::value
            , PrintingNode
        >::type;

  #if defined(ICECREAM_STL_FORMAT)
    // Print any class that specializes std::formatter<T, char>
    template <typename T>
    auto make_printing_branch(
        T&&, StringView, Config_ const&
    ) ->
        typename std::enable_if<
            is_stl_formattable<T>::value
            && !is_fmt_formattable<T>::value
            && !bypass_baseline_printing<T>::value
            , PrintingNode
        >::type;
  #endif

  #if defined(ICECREAM_FMT_ENABLED)
    // Print any class that specializes fmt::formatter<T, char>
    template <typename T>
    auto make_printing_branch(
        T&&, StringView, Config_ const&
    ) ->
        typename std::enable_if<
            is_fmt_formattable<T>::value
            && !bypass_baseline_printing<T>::value
            , PrintingNode
        >::type;
  #endif

    // Print C string
    template <typename T>
    auto make_printing_branch(
        T&&, StringView, Config_ const&
   ) -> typename std::enable_if<is_c_string<remove_ref_t<T>>::value, PrintingNode>::type;

    // Print std::string and std::string_view
    template <typename T>
    auto make_printing_branch(
        T&&, StringView, Config_ const&
    ) ->
        typename std::enable_if<
            is_std_string<remove_ref_t<T>>::value || is_string_view<remove_ref_t<T>>::value,
            PrintingNode
        >::type;

    template <typename T>
    auto do_print_char(T, Config_ const&, std::ostringstream&) -> PrintingNode;

    // Print character
    template <typename T>
    auto make_printing_branch(
        T&&, StringView, Config_ const&
    ) -> typename std::enable_if<
            is_character<remove_ref_t<T>>::value, PrintingNode
        >::type;

    // Print signed and unsigned char
    template <typename T>
    auto make_printing_branch(
        T&&, StringView, Config_ const&
    ) -> typename std::enable_if<is_xsig_char<T>::value, PrintingNode>::type;

    // Print smart pointers without an operator<<(ostream&) overload.
    template <typename T>
    auto make_printing_branch(
        T&&, StringView, Config_ const&
    ) -> typename std::enable_if<
        is_unstreamable_ptr<remove_ref_t<T>>::value && !is_baseline_printable<T>::value,
        PrintingNode
    >::type;

    // Print weak pointer classes
    template <typename T>
    auto make_printing_branch(
        T&&, StringView, Config_ const&
    ) -> typename std::enable_if<is_weak_ptr<remove_ref_t<T>>::value, PrintingNode>::type;

  #if defined(ICECREAM_OPTIONAL_HEADER)
    // Print std::optional<> classes
    template <typename T>
    auto make_printing_branch(
        T&&, StringView, Config_ const&
    ) -> typename std::enable_if<
        is_optional<remove_ref_t<T>>::value && !is_baseline_printable<T>::value,
        PrintingNode
    >::type;
  #endif

    template <typename T>
    auto do_print_variant(
        T&&, StringView, Config_ const&
    ) -> typename std::enable_if<!is_variant<remove_ref_t<T>>::value, PrintingNode>::type;

    template <typename T>
    auto do_print_variant(
        T&&, StringView, Config_ const&
    ) -> typename std::enable_if<is_variant<remove_ref_t<T>>::value, PrintingNode>::type;

    // Print *::variant<Ts...> classes
    template <typename T>
    auto make_printing_branch(
        T&&, StringView, Config_ const&
    ) -> typename std::enable_if<
        is_variant<remove_ref_t<T>>::value && !is_baseline_printable<T>::value,
        PrintingNode
    >::type;

    template <typename T>
    auto do_print_tuple(
        T&&, StringView, Config_ const&
    ) -> typename std::enable_if<
            !is_tuple<remove_cvref_t<T>>::value
            , PrintingNode
        >::type;

    template <typename T>
    auto do_print_tuple(
        T&&, StringView, Config_ const&
    ) -> typename std::enable_if<
            is_tuple<remove_cvref_t<T>>::value
            , PrintingNode
        >::type;

    // Print tuple like classes
    template <typename T>
    auto make_printing_branch(
        T&&, StringView, Config_ const&
    ) -> typename std::enable_if<
        is_tuple<remove_cvref_t<T>>::value && !is_baseline_printable<T>::value,
        PrintingNode
    >::type;

    template <typename T>
    auto do_print_range(
        T&&, StringView, Config_ const&
    ) -> typename std::enable_if<!is_range<T>::value, PrintingNode>::type;

    template <typename T>
    auto do_print_range(
        T&&, StringView, Config_ const&
    ) -> typename std::enable_if<is_range<T>::value , PrintingNode>::type;

    // Print all elements of a range
    template <typename T>
    auto make_printing_branch(
        T&&, StringView, Config_ const&
    ) -> typename std::enable_if<
        (
            is_range<T>::value                      // it is range
            && !is_baseline_printable<T>::value     // but it is not printable by baseline strategies
            && !bypass_baseline_printing<T>::value  // and it hasn't its own strategy
         ) || (
            // Except by arrays of anything other than character, which are baseline
            // printable, but should be printed by this strategy instead.
            std::is_array<remove_ref_t<T>>::value
            && !is_c_string<remove_ref_t<T>>::value
        )
        , PrintingNode
    >::type;

    // Print classes deriving from only std::exception and not from boost::exception
    template <typename T>
    auto make_printing_branch(
        T&&, StringView, Config_ const&
    ) -> typename std::enable_if<
        std::is_base_of<std::exception, remove_cvref_t<T>>::value
        && !std::is_base_of<boost::exception, remove_cvref_t<T>>::value
        && !is_baseline_printable<T>::value,
        PrintingNode
    >::type;

    // Print classes deriving from both std::exception and boost::exception
    template <typename T>
    auto make_printing_branch(
        T&&, StringView, Config_ const&
    ) -> typename std::enable_if<
        std::is_base_of<std::exception, remove_cvref_t<T>>::value
        && std::is_base_of<boost::exception, remove_cvref_t<T>>::value
        && !is_baseline_printable<T>::value,
        PrintingNode
    >::type;

  #if defined(ICECREAM_DUMP_STRUCT_CLANG)
    // Forward declare so that it can be called by the overload printing a collection for instance,
    // when the elements type shoud be printed using clang dump_struct.
    template <typename T>
    auto make_printing_branch(
        T&&, StringView, Config_ const&
    ) -> typename std::enable_if<is_handled_by_clang_dump_struct<T>::value, PrintingNode>::type;
  #endif


    // -------------------------------------------------- Character transcoders

    // Read the next UTF-16 char16_t code units and convert them to a UTF-32 char32_t.
    //
    // Returns an updated StringView without the processed code units, and a char32_t if
    // successfully converted or a char16_t with the invalid code unit otherwise.
    inline auto to_codepoint(U16StringView input)
        -> std::tuple<U16StringView, Variant<char32_t, char16_t>>
    {
        if ((input[0] - 0xD800u) >= 2048u)  // is not surrogate
        {
            auto const codepoint = char32_t{input[0]};
            return std::make_tuple(input.substr(1), codepoint);
        }
        else if (
            (input[0] & 0xFFFFFC00u) == 0xD800u  // is high surrogate
            && input.size() >= 2
            && (input[1] & 0xFFFFFC00u) == 0xDC00u  // is low surrogate
        ){
            auto const high = uint32_t{input[0]};
            auto const low = uint32_t{input[1]};
            auto const codepoint = char32_t{(high << 10) + low - 0x35FDC00u};
            return std::make_tuple(input.substr(2), codepoint);
        }
        else
        {
            // Encoding error
            return std::make_tuple(input.substr(1), input[0]);
        }
    }

  #if defined(__cpp_char8_t)
    // Reads the next UTF-8 char8_t code units and convert them to a UTF-32 char32_t.
    // `input` MUST not be empty.
    //
    // Returns an updated StringView without the processed code units, and a char32_t if
    // successfully converted or a char8_t with the invalid code unit otherwise.
    inline auto to_codepoint(U8StringView input)
        -> std::tuple<U8StringView, Variant<char32_t, char8_t>>
    {
        if ((input[0] & 0x80) == 0)  // 0xxxxxxx
        {
            return std::make_tuple(input.substr(1), static_cast<char32_t>(input[0]));
        }
        else if ((input[0] & 0xE0) == 0xC0)  // 110xxxxx 10xxxxxx
        {
            if (input.size() < 2 || (input[1] & 0xC0) != 0x80)
            {
                return std::make_tuple(input.substr(1), input[0]);
            }
            auto codepoint = static_cast<char32_t>(
                ((input[0] & 0x1F) << 6) | (input[1] & 0x3F)
            );
            return std::make_tuple(input.substr(2), codepoint);
        }
        else if ((input[0] & 0xF0) == 0xE0)  // 1110xxxx 10xxxxxx 10xxxxxx
        {
            if (input.size() < 3 || (input[1] & 0xC0) != 0x80 || (input[2] & 0xC0) != 0x80)
            {
                return std::make_tuple(input.substr(1), input[0]);
            }
            auto codepoint = static_cast<char32_t>(
                ((input[0] & 0x0F) << 12) | ((input[1] & 0x3F) << 6) | (input[2] & 0x3F)
            );
            return std::make_tuple(input.substr(3), codepoint);
        }
        else if ((input[0] & 0xF8) == 0xF0)  // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        {
            if (
                input.size() < 4
                || (input[1] & 0xC0) != 0x80
                || (input[2] & 0xC0) != 0x80
                || (input[3] & 0xC0) != 0x80
            ) {
                return std::make_tuple(input.substr(1), input[0]);
            }
            auto codepoint = static_cast<char32_t>(
                ((input[0] & 0x07) << 18)
                | ((input[1] & 0x3F) << 12)
                | ((input[2] & 0x3F) << 6)
                | (input[3] & 0x3F)
            );
            return std::make_tuple(input.substr(4), codepoint);
        }
        else
        {
            return std::make_tuple(input.substr(1), input[0]);
        }
    }
  #endif  // defined(__cpp_char8_t)

    // Transcodes an UTF-X string with a CharT code units to an UTF-32 string with
    // char32_t code units.
    template <typename CharT>
    auto to_utf32_u32string(BasicStringView<CharT> input) -> std::u32string
    {
        auto result = std::u32string{};
        while (!input.empty())
        {
            auto codepoint = Variant<char32_t, CharT>{};
            std::tie(input, codepoint) = to_codepoint(input);

            if (codepoint.index() == 0)
            {
                result.push_back(get<char32_t>(codepoint));
            }
            else
            {
                // Encoding error, print the REPLACEMENT CHARACTER
                result.push_back(0xFFFD);
            }
        }
        return result;
    }

    // Transcodes an UTF-32 string with char32_t code units to an UTF-8 string with char
    // code units.
    inline auto to_utf8_string(U32StringView input) -> std::string
    {
        auto result = std::string{};

        for (auto i = size_t{0}; i < input.size(); ++i)
        {
            if (input[i] < 0x80)
            {
                result.push_back(static_cast<char>(input[i]));  // 0xxxxxxx
            }
            else if (input[i] < 0x800)  // 00000yyy yyxxxxxx
            {
                result.push_back(static_cast<char>(0xC0 | (input[i] >> 6)));    // 110yyyyy
                result.push_back(static_cast<char>(0x80 | (input[i] & 0x3F)));  // 10xxxxxx
            }
            else if (input[i] < 0x10000)   // zzzzyyyy yyxxxxxx
            {
                result.push_back(static_cast<char>(0xE0 | (input[i] >> 12)));          // 1110zzzz
                result.push_back(static_cast<char>(0x80 | ((input[i] >> 6) & 0x3F)));  // 10yyyyyy
                result.push_back(static_cast<char>(0x80 | (input[i] & 0x3F)));         // 10xxxxxx
            }
            else if (input[i] < 0x200000)  // 000uuuuu zzzzyyyy yyxxxxxx
            {
                result.push_back(static_cast<char>(0xF0 | (input[i] >> 18)));           // 11110uuu
                result.push_back(static_cast<char>(0x80 | ((input[i] >> 12) & 0x3F)));  // 10uuzzzz
                result.push_back(static_cast<char>(0x80 | ((input[i] >> 6)  & 0x3F)));  // 10yyyyyy
                result.push_back(static_cast<char>(0x80 | (input[i] & 0x3F)));          // 10xxxxxx
            }
            else  // Encoding error, print the REPLACEMENT CHARACTER
            {
                result.push_back(static_cast<char>(0xEF));
                result.push_back(static_cast<char>(0xBF));
                result.push_back(static_cast<char>(0xBD));
            }
        }
        return result;
    }

    // Converts a wide string to a "execution encoded" narrow multibyte char string, using
    // the appropriate std::*rtomb function. This function will be used when the running
    // program has set the current locale.
    template<typename CharT, size_t(*tomb)(char*, CharT, mbstate_t*)>
    auto xrtomb(BasicStringView<CharT> str) -> std::string
    {
        auto result = std::string{};

        auto state = std::mbstate_t{};
        for (auto c : str)
        {
            auto mb = std::string(MB_CUR_MAX, '\0');
            if (tomb(&mb[0], c, &state) == static_cast<size_t>(-1))
            {
                result.append("<?>");
            }
            else
            {
                result.append(mb);
            }
        }

        return result;
    }

    inline auto count_utf8_code_point(StringView str) -> size_t
    {
        auto result = size_t{0};
        auto const n_bytes = str.size();
        for (size_t idx = 0; idx < n_bytes; ++idx, ++result)
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

    // Checks if the code_unit is a valid codepoint.
    // Returns 1 if it is a valid codepoint, returns 0 otherwise
    inline auto is_code_point_valid(uint_least32_t const code_point) -> bool
    {
        // U+10FFFF is the current largest defined code point
        // the [U+D800 - U+DFFF] interval are the surrogates
        return code_point <= 0x10FFFF && !(0xD800 <= code_point && code_point <= 0xDFFF);
    }


    // -------------------------------------------------- Split

    // Split the `whole` string in pieces, cutting at the indexes within `cut_idxs`. The
    // chars at the cutting indexes won't be at the result pieces.
    inline auto split(
        StringView whole, std::vector<size_t> const& cut_idxs
    ) -> std::vector<StringView>
    {
        auto result = std::vector<StringView>{};

        auto cut_a = size_t{0};
        for (auto cut_b : cut_idxs)
        {
            result.push_back(whole.substr(cut_a, cut_b - cut_a));
            cut_a = cut_b + 1;
        }

        result.push_back(whole.substr(cut_a, StringView::npos));

        return result;
    }

    // Split the `whole` string in pieces, cutting at the places where `sep` is.
    inline auto split(StringView whole, char sep) -> std::vector<StringView>
    {
        auto result = std::vector<StringView>{};

        auto cut_a = size_t{0};
        for (auto i = size_t{0}; i < whole.size(); ++i)
        {
            if (whole[i] == sep)
            {
                result.push_back(whole.substr(cut_a, i - cut_a));
                cut_a = i + 1;
            }
        }

        result.push_back(whole.substr(cut_a, StringView::npos));

        return result;
    }

    // -------------------------------------------------- Prefix

    class Prefix
    {
    private:
        template <typename T>
        struct Function
        {
            static_assert(is_invocable<T>::value, "");

            Function(T const& func)
                : func{func}
            {}

            auto operator()() -> std::string
            {
                auto buf = std::ostringstream{};
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
        Prefix(Ts&&... funcs)
            : functions{}
        {
            // Call this->functions.emplace_back to all funcs
            (void) std::initializer_list<int>{
                (
                    (void) this->functions.push_back(
                        Function<typename std::decay<Ts>::type>{
                            std::forward<Ts>(funcs)
                        }
                    ),
                    0
                )...
            };
        }

        auto operator()() const -> std::string
        {
            auto result = std::string{};
            for (auto const& func : this->functions)
                result.append(func());

            return result;
        }
    };


    // -------------------------------------------------- to_invocable

    // If value is a string returns an function that returns it.
    template <typename T>
    auto to_invocable(T&& value) ->
        typename std::enable_if<
            is_std_string<remove_ref_t<T>>::value || is_c_string<remove_ref_t<T>>::value
            , std::function<std::string()>
        >::type
    {
        auto const str = std::string{value};
        return [str]{return str;};
    }

    // If value is already invocable do nothing.
    template <typename T>
    auto to_invocable(T&& value) ->
        typename std::enable_if<
            is_invocable<T>::value
            , T&&
        >::type
    {
        return std::forward<T>(value);
    }


    // -------------------------------------------------- Output

    template <typename T>
    class Output
    {
    public:
        Output(T it)
            : it{it}
        {}

        // Expects `str` in "output encoding".
        auto operator()(StringView str) -> void
        {
            for (auto const& c : str)
            {
                *this->it = c;
                ++this->it;
            }
        }

    private:
        T it;
    };


    // -------------------------------------------------- Hereditary

    // A hereditary object can optionally hold a value, but will always produce a value
    // when requested, by delegating the request to its parent if needed.
    template <typename T>
    class Hereditary
    {
    public:
        // A child constructed without a value will delegate the value requests to its
        // parent.
        Hereditary(Hereditary<T> const& parent)
            : parent{&parent}
        {}

        // A root object (without a parent) must always have a value.
        Hereditary(T const& value)
            : storage(value)
            , parent{nullptr}
        {}

        // A root object (without a parent) must always have a value.
        Hereditary(T&& value)
            : storage(std::move(value))
            , parent{nullptr}
        {}

        auto operator=(T const& value) -> Hereditary&
        {
            this->storage = value;
            return *this;
        }

        auto operator=(T&& value) -> Hereditary&
        {
            this->storage = std::move(value);
            return *this;
        }

        auto value() const -> T const&
        {
            if (this->storage)
            {
                return this->storage.value();
            }
            else if (this->parent)
            {
                return this->parent->value();
            }
            else
            {
                ICECREAM_UNREACHABLE;
                return this->storage.value();
            }
        }

    private:
        Optional<T> storage;
        Hereditary<T> const* parent;
    };

} // namespace detail


    // -------------------------------------------------- Config

    class Config
    {
    public:

        explicit Config(Config const* parent)
            : enabled_(parent->enabled_)
            , output_(parent->output_)
            , prefix_(parent->prefix_)
            , decay_char_array_(parent->decay_char_array_)
            , show_c_string_(parent->show_c_string_)
            , force_range_strategy_(parent->force_range_strategy_)
            , force_tuple_strategy_(parent->force_tuple_strategy_)
            , force_variant_strategy_(parent->force_variant_strategy_)
            , wide_string_transcoder_(parent->wide_string_transcoder_)
            , unicode_transcoder_(parent->unicode_transcoder_)
            , output_transcoder_(parent->output_transcoder_)
            , line_wrap_width_(parent->line_wrap_width_)
            , include_context_(parent->include_context_)
            , context_delimiter_(parent->context_delimiter_)
        {}

        auto is_enabled() const -> bool
        {
            std::lock_guard<std::mutex> guard(this->attribute_mutex);
            return this->enabled_.value();
        }

        auto enable() -> Config&
        {
            std::lock_guard<std::mutex> guard(this->attribute_mutex);
            this->enabled_ = true;
            return *this;
        }

        auto disable() -> Config&
        {
            std::lock_guard<std::mutex> guard(this->attribute_mutex);
            this->enabled_ = false;
            return *this;
        }

        // Gatekeeper function to emmit better error messages in invalid input.
        template <typename T>
        auto output(T&& t) ->
            typename std::enable_if<
                detail::disjunction<
                    std::is_base_of<std::ostream, typename std::decay<T>::type>,
                    detail::has_push_back_T<T, char>,
                    detail::is_T_output_iterator<T, char>
                >::value,
                Config&
            >::type
        {
            this->set_output(std::forward<T>(t));
            return *this;
        }

        template <typename... Ts>
        auto prefix(Ts&&... value) ->
            typename std::enable_if<
                sizeof...(Ts) >= 1
                && detail::conjunction<
                    detail::is_valid_prefix<detail::remove_ref_t<Ts>>...
                >::value
                , Config&
            >::type
        {
            std::lock_guard<std::mutex> guard(this->attribute_mutex);
            this->prefix_ = detail::Prefix(detail::to_invocable(std::forward<Ts>(value))...);
            return *this;
        }

        auto decay_char_array() const -> bool
        {
            std::lock_guard<std::mutex> guard(this->attribute_mutex);
            return this->decay_char_array_.value();
        }

        auto decay_char_array(bool value) -> Config&
        {
            std::lock_guard<std::mutex> guard(this->attribute_mutex);
            this->decay_char_array_ = value;
            return *this;
        }

        auto show_c_string() const -> bool
        {
            std::lock_guard<std::mutex> guard(this->attribute_mutex);
            return this->show_c_string_.value();
        }

        auto show_c_string(bool value) -> Config&
        {
            std::lock_guard<std::mutex> guard(this->attribute_mutex);
            this->show_c_string_ = value;
            return *this;
        }

        auto force_range_strategy() const -> bool
        {
            std::lock_guard<std::mutex> guard(this->attribute_mutex);
            return this->force_range_strategy_.value();
        }

        auto force_range_strategy(bool value) -> Config&
        {
            std::lock_guard<std::mutex> guard(this->attribute_mutex);
            this->force_range_strategy_ = value;
            return *this;
        }

        auto force_tuple_strategy() const -> bool
        {
            std::lock_guard<std::mutex> guard(this->attribute_mutex);
            return this->force_tuple_strategy_.value();
        }

        auto force_tuple_strategy(bool value) -> Config&
        {
            std::lock_guard<std::mutex> guard(this->attribute_mutex);
            this->force_tuple_strategy_ = value;
            return *this;
        }

        auto force_variant_strategy() const -> bool
        {
            std::lock_guard<std::mutex> guard(this->attribute_mutex);
            return this->force_variant_strategy_.value();
        }

        auto force_variant_strategy(bool value) -> Config&
        {
            std::lock_guard<std::mutex> guard(this->attribute_mutex);
            this->force_variant_strategy_ = value;
            return *this;
        }
        auto wide_string_transcoder(
            std::function<std::string(wchar_t const*, size_t)>&& transcoder
        ) -> Config&
        {
            std::lock_guard<std::mutex> guard(this->attribute_mutex);
            this->wide_string_transcoder_ =
                [transcoder](detail::WStringView str) -> std::string
                {
                    return transcoder(str.data(), str.size());
                };
            return *this;
        }

      #if defined(ICECREAM_STRING_VIEW_HEADER)
        auto wide_string_transcoder(
            std::function<std::string(std::wstring_view)>&& transcoder
        ) -> Config&
        {
            std::lock_guard<std::mutex> guard(this->attribute_mutex);
            this->wide_string_transcoder_ =
                [transcoder](detail::WStringView str) -> std::string
                {
                    return transcoder({str.data(), str.size()});
                };
            return *this;
        }
      #endif

        auto unicode_transcoder(
            std::function<std::string(char32_t const*, size_t)>&& transcoder
        ) -> Config&
        {
            std::lock_guard<std::mutex> guard(this->attribute_mutex);
            this->unicode_transcoder_ =
                [transcoder](detail::U32StringView str) -> std::string
                {
                    return transcoder(str.data(), str.size());
                };
            return *this;
        }

      #if defined(ICECREAM_STRING_VIEW_HEADER)
        auto unicode_transcoder(
            std::function<std::string(std::u32string_view)>&& transcoder
        ) -> Config&
        {
            std::lock_guard<std::mutex> guard(this->attribute_mutex);
            this->unicode_transcoder_ =
                [transcoder](detail::U32StringView str) -> std::string
                {
                    return transcoder({str.data(), str.size()});
                };
            return *this;
        }
      #endif

        auto output_transcoder(
            std::function<std::string(char const*, size_t)>&& transcoder
        ) -> Config&
        {
            std::lock_guard<std::mutex> guard(this->attribute_mutex);
            this->output_transcoder_ =
                [transcoder](detail::StringView str) -> std::string
                {
                    return transcoder(str.data(), str.size());
                };
            return *this;
        }

    #if defined(ICECREAM_STRING_VIEW_HEADER)
        auto output_transcoder(std::function<std::string(std::string_view)>&& transcoder) -> Config&
        {
            std::lock_guard<std::mutex> guard(this->attribute_mutex);
            this->output_transcoder_ =
                [transcoder](detail::StringView str) -> std::string
                {
                    return transcoder({str.data(), str.size()});
                };
            return *this;
        }
    #endif

        auto line_wrap_width() const -> size_t
        {
            std::lock_guard<std::mutex> guard(this->attribute_mutex);
            return this->line_wrap_width_.value();
        }

        auto line_wrap_width(size_t value) -> Config&
        {
            std::lock_guard<std::mutex> guard(this->attribute_mutex);
            this->line_wrap_width_ = value;
            return *this;
        }

        auto include_context() const -> bool
        {
            std::lock_guard<std::mutex> guard(this->attribute_mutex);
            return this->include_context_.value();
        }

        auto include_context(bool value) -> Config&
        {
            std::lock_guard<std::mutex> guard(this->attribute_mutex);
            this->include_context_ = value;
            return *this;
        }

        auto context_delimiter() const -> std::string
        {
            std::lock_guard<std::mutex> guard(this->attribute_mutex);
            return this->context_delimiter_.value();
        }

        auto context_delimiter(std::string value) -> Config&
        {
            std::lock_guard<std::mutex> guard(this->attribute_mutex);
            this->context_delimiter_ = value;
            return *this;
        }

    protected:

        Config() = default;

        auto set_output(std::ostream& stream) -> void
        {
            std::lock_guard<std::mutex> guard(this->attribute_mutex);

            using OSIt = std::ostreambuf_iterator<char>;
            this->output_ = detail::Output<OSIt>{OSIt{stream}};
        }

        template <typename T>
        auto set_output(T& container) ->
            typename std::enable_if<
                detail::has_push_back_T<T, char>::value
            >::type
        {
            std::lock_guard<std::mutex> guard(this->attribute_mutex);

            using OSIt = std::back_insert_iterator<T>;
            this->output_ = detail::Output<OSIt>{OSIt{container}};
        }

        template <typename T>
        auto set_output(T iterator) ->
            typename std::enable_if<
            detail::is_T_output_iterator<T, char>::value
            >::type
        {
            std::lock_guard<std::mutex> guard(this->attribute_mutex);
            this->output_ = detail::Output<T>{iterator};
        }

        mutable std::mutex attribute_mutex;

        detail::Hereditary<bool> enabled_{true};

        detail::Hereditary<std::function<void(std::string const&)>> output_{
            detail::Output<std::ostreambuf_iterator<char>>{
                std::ostreambuf_iterator<char>{std::cerr}
            }
        };

        detail::Hereditary<detail::Prefix> prefix_{
            detail::Prefix(
                []() -> std::string
                {
                    return "ic| ";
                }
            )
        };

        detail::Hereditary<bool> decay_char_array_{false};

        detail::Hereditary<bool> show_c_string_{true};

        detail::Hereditary<bool> force_range_strategy_{true};

        detail::Hereditary<bool> force_tuple_strategy_{true};

        detail::Hereditary<bool> force_variant_strategy_{true};

        detail::Hereditary<std::function<std::string(detail::WStringView)>> wide_string_transcoder_{
            [](detail::WStringView str) -> std::string
            {
                auto const c_locale = std::string{std::setlocale(LC_ALL, nullptr)};
                if (c_locale != "C" && c_locale != "POSIX")
                {
                    return detail::xrtomb<wchar_t, std::wcrtomb>(str);
                }
                else
                {
                    switch (sizeof(wchar_t))
                    {
                    case 2:
                        {
                            auto const utf32_str =
                                detail::to_utf32_u32string(
                                    detail::U16StringView(
                                        reinterpret_cast<char16_t const*>(str.data()), str.size()
                                    )
                                );
                            return detail::to_utf8_string(utf32_str);
                        }
                    case 4:
                        return detail::to_utf8_string(
                            detail::U32StringView(
                                reinterpret_cast<char32_t const*>(str.data()), str.size()
                            )
                        );
                    default:
                        return "<?>";
                    }
                }
            }
        };

        detail::Hereditary<std::function<std::string(detail::U32StringView str)>> unicode_transcoder_{
            [](detail::U32StringView str) -> std::string
            {
              #ifdef ICECREAM_CUCHAR_HEADER
                auto const c_locale = std::string{std::setlocale(LC_ALL, nullptr)};
                if (c_locale != "C" && c_locale != "POSIX")
                {
                    return detail::xrtomb<char32_t, std::c32rtomb>(str);
                }
                else
              #endif
                {
                    return detail::to_utf8_string(str);
                }
            }
        };

        // Function to convert a string in "execution encoding" to "output encoding"
        detail::Hereditary<std::function<std::string(detail::StringView)>> output_transcoder_{
            [](detail::StringView str) -> std::string
            {
                return str.to_string();
            }
        };

        detail::Hereditary<size_t> line_wrap_width_{70};

        detail::Hereditary<bool> include_context_{false};

        detail::Hereditary<std::string> context_delimiter_{"- "};
    };


namespace detail {

    // Inherits from icecream::Config, and implements access to attributes which shoud not
    // be exposed to public.
    class Config_
        : public ::icecream::Config
    {
    public:

        using Config::Config;

        constexpr static size_t INDENT_BASE = 4;

        static auto global() -> Config_&
        {
            static Config_ global_{};
            return global_;
        }

        auto output() const -> std::function<void(std::string const&)>
        {
            std::lock_guard<std::mutex> guard(this->attribute_mutex);
            auto const& output = this->output_.value();
            return [&output](std::string const& str) -> void
            {
                return output(str);
            };
        }

        auto prefix() const -> std::function<std::string()>
        {
            std::lock_guard<std::mutex> guard(this->attribute_mutex);
            auto const& prefix = this->prefix_.value();
            return [&prefix]() -> std::string
            {
                return prefix();
            };
        }

        auto wide_string_transcoder() const -> std::function<std::string(WStringView)>
        {
            std::lock_guard<std::mutex> guard(this->attribute_mutex);
            return this->wide_string_transcoder_.value();
        }

        auto unicode_transcoder() const -> std::function<std::string(U32StringView)>
        {
            std::lock_guard<std::mutex> guard(this->attribute_mutex);
            return this->unicode_transcoder_.value();
        }

        auto output_transcoder() const -> std::function<std::string(StringView)>
        {
            std::lock_guard<std::mutex> guard(this->attribute_mutex);
            return this->output_transcoder_.value();
        }

    private:

        Config_() = default;
    };


    // -------------------------------------------------- Tree

    enum class OstreamTypeMode : long
    {
        none,
        debug,
        binary,
        BINARY,
        character,
        string,
        non_binary_integer
    };

    inline auto xidx_type_mode() -> int
    {
        static auto const xindex = std::ios_base::xalloc();
        return xindex;
    }

    inline auto getOstreamTypeMode(std::ostringstream& ostrm) -> OstreamTypeMode
    {
        return static_cast<OstreamTypeMode>(ostrm.iword(xidx_type_mode()));
    }

    inline auto setOstreamTypeMode(std::ostringstream& ostrm, OstreamTypeMode type) -> void
    {
        ostrm.iword(xidx_type_mode()) = static_cast<long>(type);
    }

    // Builds an ostringstream and sets its state accordingly to `fmt` string
    inline auto build_ostream(StringView fmt) -> Optional<std::ostringstream>
    {
        // format_spec ::=  [[fill]align][sign]["#"][width]["." precision][type]
        // fill        ::=  <a character>
        // align       ::=  "<" | ">" | "^"
        // sign        ::=  "+" | "-"
        // width       ::=  integer
        // precision   ::=  integer
        // type        ::=  "a" | "A" | "b" | "B" | "c" | "d" | "e" | "E" | "f" | "F" | "g" | "G" | "o" | "s" | "x" | "X" | "?"
        // integer     ::=  digit+
        // digit       ::=  "0"..."9"

        auto os = std::ostringstream{};
        os << std::boolalpha;
        setOstreamTypeMode(os, OstreamTypeMode::none);

        auto it = std::begin(fmt);
        auto end_it = std::end(fmt);

        if (it == end_it) return os;

        // [[fill]align]
        {
            auto fill_char = os.fill();
            if (*it != '<' && *it != '>' && *it != '^')
            {
                auto la_it = it+1;
                if (la_it != end_it && (*la_it == '<' || *la_it == '>' || *la_it == '^'))
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
            else if (it != end_it && *it == '^')
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
            {
                os << std::setw(std::stoi(std::string(b_it, it)));
            }
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
        else if (it != end_it && *it == 'b')
        {
            setOstreamTypeMode(os, OstreamTypeMode::binary);
            ++it;
        }
        else if (it != end_it && *it == 'B')
        {
            setOstreamTypeMode(os, OstreamTypeMode::BINARY);
            ++it;
        }
        else if (it != end_it && *it == 'c')
        {
            setOstreamTypeMode(os, OstreamTypeMode::character);
            ++it;
        }
        else if (it != end_it && *it == 'd')
        {
            os << std::dec << std::noboolalpha;
            setOstreamTypeMode(os, OstreamTypeMode::non_binary_integer);
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
            os << std::oct << std::noboolalpha;
            setOstreamTypeMode(os, OstreamTypeMode::non_binary_integer);
            ++it;
        }
        else if (it != end_it && *it == 's')
        {
            setOstreamTypeMode(os, OstreamTypeMode::string);
            ++it;
        }
        else if (it != end_it && *it == 'x')
        {
            os << std::hex << std::nouppercase << std::noboolalpha;
            setOstreamTypeMode(os, OstreamTypeMode::non_binary_integer);
            ++it;
        }
        else if (it != end_it && *it == 'X')
        {
            os << std::hex << std::uppercase << std::noboolalpha;
            setOstreamTypeMode(os, OstreamTypeMode::non_binary_integer);
            ++it;
        }
        else if (it != end_it && *it == '?')
        {
            setOstreamTypeMode(os, OstreamTypeMode::debug);
            ++it;
        }

        if (it == end_it)
        {
            return os;
        }
        else
        {
            return {};
        }
    }

    // char -> char
    inline auto transcoder_dispatcher(Config_ const&, StringView str) -> std::string
    {
        return str.to_string();
    }

    // wchar_t -> char
    inline auto transcoder_dispatcher(Config_ const& config, WStringView str) -> std::string
    {
        return config.wide_string_transcoder()(str);
    }

  #if defined(__cpp_char8_t)
    // char8_t -> char
    inline auto transcoder_dispatcher(Config_ const& config, U8StringView str) -> std::string
    {
        return config.unicode_transcoder()(to_utf32_u32string(str));
    }
  #endif

    // char16_t -> char
    inline auto transcoder_dispatcher(Config_ const& config, U16StringView str) -> std::string
    {
        return config.unicode_transcoder()(to_utf32_u32string(str));
    }

    // char32_t -> char
    inline auto transcoder_dispatcher(Config_ const& config, U32StringView str) -> std::string
    {
        return config.unicode_transcoder()(str);
    }

    class PrintingNode
    {
    private:
        struct Stem
        {
            std::string open;
            std::string separator;
            std::string close;
            std::vector<PrintingNode> children;

            Stem(
                StringView open_,
                StringView separator_,
                StringView close_,
                std::vector<PrintingNode>&& children_
            )
                : open(open_.to_string())
                , separator(separator_.to_string())
                , close(close_.to_string())
                , children(std::move(children_))
            {}

        };

        Variant<std::string, Stem> content;

        size_t n_code_unit;
        size_t n_code_point;

        auto is_leaf() const -> bool
        {
            return this->content.index() == 0;
        }

        auto get_leaf() const -> StringView
        {
            return get<std::string>(this->content);
        }

        auto get_stem() -> Stem&
        {
            return get<Stem>(this->content);
        }

        auto get_stem() const -> Stem const&
        {
            return get<Stem>(this->content);
        }

    public:
        PrintingNode() = delete;
        PrintingNode(PrintingNode const&) = delete;
        PrintingNode& operator=(PrintingNode const&) = delete;

        PrintingNode(PrintingNode&& other)
            : content(std::move(other.content))
            , n_code_unit(other.n_code_unit)
            , n_code_point(other.n_code_point)
        {}

        PrintingNode& operator=(PrintingNode&& other)
        {
            if (this != &other)
            {
                this->~PrintingNode();
                new (this) PrintingNode(std::move(other));
            }

            return *this;
        }

        explicit PrintingNode(StringView leaf)
            : content(leaf.to_string())
            , n_code_unit(this->get_leaf().size())
            , n_code_point(count_utf8_code_point(this->get_leaf()))
        {}

        PrintingNode(
            StringView open,
            StringView separator,
            StringView close,
            std::vector<PrintingNode> children
        )
            : content(
                Stem(open, separator, close, std::move(children))
            )
            , n_code_unit{
                [this]()
                {
                    // The enclosing chars.
                    auto result = this->get_stem().open.size() + this->get_stem().close.size();

                    // count the size of each child
                    for (auto const& child : this->get_stem().children)
                    {
                        result += child.n_code_unit;
                    }

                    // The separators.
                    if (this->get_stem().children.size() > 1)
                    {
                        result +=
                            (this->get_stem().children.size() - 1)
                            * this->get_stem().separator.size();
                    }

                    return result;
                }()
            }
            , n_code_point{
                [this]()
                {
                    // The enclosing chars.
                    auto result =
                        count_utf8_code_point(this->get_stem().open)
                        + count_utf8_code_point(this->get_stem().close);

                    // count the size of each child
                    for (auto const& child : this->get_stem().children)
                    {
                        result += child.n_code_point;
                    }

                    // The separators.
                    if (this->get_stem().children.size() > 1)
                    {
                        result +=
                            (this->get_stem().children.size() - 1)
                            * count_utf8_code_point(this->get_stem().separator);
                    }

                    return result;
                }()
            }
        {}

        auto code_point_length() const -> size_t
        {
            return this->n_code_point;
        }

        // Search among the children of `this` Tree, by a Tree having `key` as its
        // content. Returns `nullptr` if no child has been found.
        auto find_leaf(StringView key) -> PrintingNode*
        {
            auto to_visit = std::vector<PrintingNode*>{this};
            while (!to_visit.empty())
            {
                auto current = to_visit.back();
                to_visit.pop_back();

                if (!current->is_leaf())
                {
                    for (auto& child : current->get_stem().children)
                    {
                        to_visit.push_back(&child);
                    }
                }
                else if (current->get_leaf() == key)
                {
                    return current;
                }
            }

            return nullptr;
        }

        // Print the branch content, in a unique line
        auto print() const -> std::string
        {
            auto result = std::string{};
            result.reserve(this->n_code_unit);

            if (this->is_leaf())
            {
                result = this->get_leaf().to_string();
            }
            else
            {
                result = this->get_stem().open;

                for (
                    auto it = this->get_stem().children.cbegin();
                    it != this->get_stem().children.cend();
                ){
                    result += it->print();

                    ++it;

                    if (it != this->get_stem().children.cend())
                    {
                        result += this->get_stem().separator;
                    }
                }

                result += this->get_stem().close;
            }

            return result;
        }

        auto print(size_t const indent_level, size_t const line_wrap_width) const -> std::string
        {
            auto result = std::string{};
            result.reserve(this->n_code_unit);

            // Leaf nodes will print its content regardless of being longer than the maximum line
            // length, since there is no children to be split.
            if (this->is_leaf())
            {
                result = this->get_leaf().to_string();
            }
            else
            {
                result = this->get_stem().open;
                result += "\n";

                auto const indent_lenght = indent_level * Config_::INDENT_BASE ;
                for (
                    auto it = this->get_stem().children.cbegin();
                    it != this->get_stem().children.cend();
                ){
                    result += std::string(indent_level * Config_::INDENT_BASE, ' ');

                    if (indent_lenght + it->n_code_point <= line_wrap_width)
                    {
                        result += it->print();
                    }
                    else
                    {
                        result += it->print(indent_level+1, line_wrap_width);
                    }

                    ++it;

                    if (it != this->get_stem().children.cend())
                    {
                        result += this->get_stem().separator;
                        result += "\n";
                    }
                    else
                    {
                        result += "\n";
                    }
                }

                result +=
                    std::string((indent_level-1) * Config_::INDENT_BASE, ' ')
                    + this->get_stem().close;
            }

            return result;
        }
    };


    // -------------------------------------------------- make_printing_branch functions

    template <typename T>
    auto do_print_integral(
        T const&, Config_ const&, std::ostringstream&
    ) -> typename std::enable_if<!std::is_integral<remove_cvref_t<T>>::value, PrintingNode>::type
    {
        ICECREAM_UNREACHABLE;
        return PrintingNode("");
    }

    template <typename T>
    auto do_print_integral(
        T const& value, Config_ const& config, std::ostringstream& ostrm
    ) -> typename std::enable_if<std::is_integral<remove_cvref_t<T>>::value, PrintingNode>::type
    {
        auto print_binary =
            [&](bool uppercase_base)
            {
                if (ostrm.flags() & std::ios_base::showbase)
                {
                    ostrm << (uppercase_base ? "0B" : "0b");
                }

                auto const n_bits = sizeof(value) * CHAR_BIT;

                // Flag to skip leading zeros
                auto found_one = false;

                // Loop through each bit from the most significant to the least significant
                for (auto i = n_bits; (i--) > 0;)
                {
                    auto const bit = (value >> i) & 1;

                    if (bit == 1) found_one = true;

                    if (found_one || i == 0) // Always print at least the last bit
                    {
                        ostrm << bit;
                    }
                }
            };

        switch (getOstreamTypeMode(ostrm))
        {
        case OstreamTypeMode::character:
            if (value > 127)
            {
                return PrintingNode("*Error* Integral value outside the range of the char type");
            }
            else if (std::is_same<remove_cvref_t<T>, bool>::value)
            {
                return PrintingNode("*Error* in formatting string");
            }
            else
            {
                return do_print_char(static_cast<char>(value), config, ostrm);
            }

        case OstreamTypeMode::none:
        case OstreamTypeMode::non_binary_integer:
            ostrm << value;
            return PrintingNode(ostrm.str());

        case OstreamTypeMode::binary:
            print_binary(false);
            return PrintingNode(ostrm.str());

        case OstreamTypeMode::BINARY:
            print_binary(true);
            return PrintingNode(ostrm.str());

        default:
            return PrintingNode("*Error* in formatting string");
        }
    }

    // Print any class that overloads operator<<(std::ostream&, T)
    template <typename T>
    auto make_printing_branch(
        T&& value, StringView fmt, Config_ const& config
    ) ->
        typename std::enable_if<
            is_streamable<T>::value
            && !is_stl_formattable<T>::value
            && !is_fmt_formattable<T>::value
            && !bypass_baseline_printing<T>::value
            , PrintingNode
        >::type
    {
        if (config.force_variant_strategy() && is_variant<remove_ref_t< T>>::value)
        {
            return do_print_variant(std::forward<T>(value), fmt, config);
        }

        auto mb_ostrm = build_ostream(fmt);
        if (!mb_ostrm)
        {
            return PrintingNode("*Error* in formatting string");
        }

        if (std::is_integral<remove_cvref_t<T>>::value)
        {
            return do_print_integral(value, config, *mb_ostrm);
        }
        else
        {
            *mb_ostrm << value;
            return PrintingNode(mb_ostrm->str());
        }
    }

  #if defined(ICECREAM_STL_FORMAT_RANGES)

    // This class will wrap a range T, signaling it as "hijacked" to avoid a circular
    // inheritance in the `std::formatter` specialization below.
    template <std::ranges::forward_range T>
    struct HijackTag
    {
        T const& value;

        HijackTag(T const& t)
            : value(t)
        {}

        auto begin() const
        {
            return std::begin(value);
        }

        auto end() const
        {
            return std::end(value);
        }
    };

}} namespace std {

    // Starting from C++23 the formatting library has a concept specialization to print
    // any *range*. Because of this any *range* that Icecream-cpp could try to print would
    // be handled by the STL's Formatting function overload of the *baseline printable*
    // strategy.
    //
    // We would like that the generic range case would continue being handled by the
    // *range types* strategy, but other than that, any `formatter` struct further
    // specializing a range should take precedence and the *baseline printable* strategy
    // be the one used. For example, when printing a `std::vector<int>` the *range types*
    // strategy should be used, but if there exist a `std::formatter<std::vector<T>>`
    // specialization, the *baseline printable* strategy should be used instead.
    //
    // Here we will hijack any instantiation of the generic range specialization, inherit
    // from the hijacked STL version so that we have the required methods implemented, and
    // insert an `Hijacked` typedef to signalize that this specialization was used, so
    // that Icecream-cpp knows that the type `T` is a range, and is free to delegate the
    // printing of `T` to the ranges strategy.
    template< ranges::forward_range T>
    requires (format_kind<remove_cvref_t<T>> != range_format::disabled) &&
        formattable<ranges::range_reference_t<T>, char>
        && (!::icecream::detail::is_instantiation<::icecream::detail::HijackTag, remove_cvref_t<T>>::value)
        && (!::icecream::detail::is_c_string<T>::value)
        && (!::icecream::detail::is_std_string<remove_cvref_t<T>>::value)
        && (!::icecream::detail::is_string_view<remove_cvref_t<T>>::value)
        && (!std::is_array<remove_cvref_t<T>>::value)
    struct formatter<T, char>
        : public formatter<::icecream::detail::HijackTag<remove_cvref_t<T>>, char>
    {
        using Hijacked = int;
    };

} namespace icecream { namespace detail {

  #endif  // defined(ICECREAM_STL_FORMAT_RANGES)

  #if defined(ICECREAM_STL_FORMAT)
    template <typename T, typename = void>
    struct has_hijacked_tag : std::false_type {};

    template <typename T>
    struct has_hijacked_tag<
        T,
        typename std::enable_if<
            !std::is_void<typename std::formatter<remove_cvref_t<T>>::Hijacked>::value
        >::type
    > : std::true_type {};

    // Print any class that specializes std::formatter<T, char>
    template <typename T>
    auto make_printing_branch(
        T&& value, StringView fmt, Config_ const& config
    ) ->
        typename std::enable_if<
            is_stl_formattable<T>::value
            && !is_fmt_formattable<T>::value
            && !bypass_baseline_printing<T>::value
            , PrintingNode
        >::type
    {
        if (config.force_range_strategy() && has_hijacked_tag<T>::value)
        {
            return do_print_range(std::forward<T>(value), fmt, config);
        }
        else if (config.force_tuple_strategy() && is_tuple<remove_cvref_t<T>>::value)
        {
            return do_print_tuple(std::forward<T>(value), fmt, config);
        }
        else if (config.force_variant_strategy() && is_variant<remove_ref_t< T>>::value)
        {
            return do_print_variant(std::forward<T>(value), fmt, config);
        }

        try
        {
            return PrintingNode(
                std::vformat("{:" + fmt.to_string() + "}", std::make_format_args(value))
            );
        }
        catch (std::format_error const&)
        {
            return PrintingNode("*Error* in formatting string");
        }
    }
  #endif // defined(ICECREAM_STL_FORMAT)

  #if defined(ICECREAM_FMT_ENABLED)
    // Print any class that specializes fmt::formatter<T, char>
    template <typename T>
    auto make_printing_branch(
        T&& value, StringView fmt, Config_ const& config
    ) ->
        typename std::enable_if<
            is_fmt_formattable<T>::value
            && !bypass_baseline_printing<T>::value
            , PrintingNode
        >::type
    {
        if (config.force_range_strategy() && is_range<remove_cvref_t<T>>::value)
        {
            return do_print_range(std::forward<T>(value), fmt, config);
        }
        else if (config.force_tuple_strategy() && is_tuple<remove_cvref_t<T>>::value)
        {
            return do_print_tuple(std::forward<T>(value), fmt, config);
        }
        else if (config.force_variant_strategy() && is_variant<remove_cvref_t< T>>::value)
        {
            return do_print_variant(std::forward<T>(value), fmt, config);
        }

        return PrintingNode(
            fmt::vformat("{:" + fmt.to_string() + "}", fmt::make_format_args(value))
        );
    }
  #endif

    // Prints all characters encoded in "execution encoding"
    template <char Quote>
    auto print_text(
        StringView code_units, Config_ const&, std::ostringstream& ostrm
    ) -> void
    {
        switch (getOstreamTypeMode(ostrm))
        {
        case OstreamTypeMode::character:
        case OstreamTypeMode::string:
            ostrm << code_units.to_string();
            break;

        case OstreamTypeMode::none:
        case OstreamTypeMode::debug:
            for (auto cu : code_units)
            {
                switch (cu)
                {
                case '\a':
                    ostrm << "\\a";
                    break;
                case '\b':
                    ostrm << "\\b";
                    break;
                case '\t':
                    ostrm << "\\t";
                    break;
                case '\n':
                    ostrm << "\\n";
                    break;
                case '\v':
                    ostrm << "\\v";
                    break;
                case '\f':
                    ostrm << "\\f";
                    break;
                case '\r':
                    ostrm << "\\r";
                    break;
                case '\0':
                    ostrm << "\\u{0}";
                    break;
                case Quote:
                    ostrm << "\\" << Quote;
                    break;
                default:
                    ostrm << cu;
                }
            }
            break;

        default:
            ICECREAM_UNREACHABLE;

        }
    }

    // Prints char32_t characters
    template <char quote>
    auto print_text(
        U32StringView code_units, Config_ const& config, std::ostringstream& ostrm
    ) -> void
    {
        switch (getOstreamTypeMode(ostrm))
        {
        case OstreamTypeMode::character:
        case OstreamTypeMode::string:
            ostrm << config.unicode_transcoder()(code_units);
            break;

        case OstreamTypeMode::none:
        case OstreamTypeMode::debug:
            for (auto cu : code_units)
            {
                if (is_code_point_valid(cu))
                {
                    print_text<quote>(
                        config.unicode_transcoder()(U32StringView(&cu, 1)), config, ostrm
                    );
                }
                else
                {
                    ostrm << "\\x{"
                          << std::hex
                          << std::nouppercase
                          << std::char_traits<char32_t>::to_int_type(cu)
                          << "}";
                }
            }
            break;

        default:
            ICECREAM_UNREACHABLE;
        }
    }

    // Prints char8_t and char16_t characters
    template <char quote, typename CharT>
    auto print_text(
        BasicStringView<CharT> code_units, Config_ const& config, std::ostringstream& ostrm
    ) -> void
    {
        switch (getOstreamTypeMode(ostrm))
        {
        case OstreamTypeMode::character:
        case OstreamTypeMode::string:
            ostrm << config.unicode_transcoder()(to_utf32_u32string(code_units));
            break;

        case OstreamTypeMode::none:
        case OstreamTypeMode::debug:
            while (!code_units.empty())
            {
                auto result = Variant<char32_t, CharT>{};
                std::tie(code_units, result) = to_codepoint(code_units);

                if (result.index() == 0)
                {
                    print_text<quote>(
                        U32StringView(&get<char32_t>(result), 1), config, ostrm
                    );
                }
                else
                {
                    ostrm << "\\x{"
                          << std::hex
                          << std::nouppercase
                          << std::char_traits<CharT>::to_int_type(get<CharT>(result))
                          << "}";
                }
            }
            break;

        default:
            ICECREAM_UNREACHABLE;
        }
    }

    // Prints wchar_t characters
    template <char quote>
    auto print_text(
        WStringView code_units, Config_ const& config, std::ostringstream& ostrm
    ) -> void
    {
        switch (getOstreamTypeMode(ostrm))
        {
        case OstreamTypeMode::character:
        case OstreamTypeMode::string:
            ostrm << config.wide_string_transcoder()(code_units);
            break;

        case OstreamTypeMode::none:
        case OstreamTypeMode::debug:
            print_text<quote>(config.wide_string_transcoder()(code_units), config, ostrm);
            break;

        default:
            ICECREAM_UNREACHABLE;
        }
    }


    template <typename CharT>
    auto do_print_string(
        BasicStringView<CharT> value, Config_ const& config, std::ostringstream& ostrm
    ) -> PrintingNode
    {
        switch (getOstreamTypeMode(ostrm))
        {
        case OstreamTypeMode::string:
            print_text<'"'>(value, config, ostrm);
            return PrintingNode(ostrm.str());

        case OstreamTypeMode::none:
        case OstreamTypeMode::debug:
            ostrm << '"';
            print_text<'"'>(value, config, ostrm);
            ostrm << '"';
            return PrintingNode(ostrm.str());

        default:
            return PrintingNode("*Error* in formatting string");
        }
    }

    // Print C string
    template <typename T>
    auto make_printing_branch(
        T&& value, StringView fmt, Config_ const& config
    ) -> typename std::enable_if<is_c_string<remove_ref_t<T>>::value, PrintingNode>::type
    {
        using CharT =
            remove_cvref_t<
                typename std::remove_pointer<
                    typename std::decay<T>::type
                >::type
            >;

        auto mb_ostrm = build_ostream(fmt);
        if (!mb_ostrm)
        {
            return PrintingNode("*Error* in formatting string");
        }

        // If config.decay_char_array() is not true, any character array with a known size
        // should be printed as a range
        if (is_bounded_array<remove_ref_t<T>>::value && !config.decay_char_array())
        {
            return do_print_range(std::forward<T>(value), fmt, config);
        }

        if (config.show_c_string())
        {
            do_print_string<CharT>(value, config, *mb_ostrm);
        }
        else
        {
            *mb_ostrm << reinterpret_cast<void const*>(value);
        }

        return PrintingNode(mb_ostrm->str());
    }

    // Print std::string and std::string_view
    template <typename T>
    auto make_printing_branch(
        T&& value, StringView fmt, Config_ const& config
    ) ->
        typename std::enable_if<
            is_std_string<remove_ref_t<T>>::value || is_string_view<remove_ref_t<T>>::value,
            PrintingNode
        >::type
    {
        using CharT = typename remove_ref_t<T>::value_type;

        auto mb_ostrm = build_ostream(fmt);
        if (!mb_ostrm)
        {
            return PrintingNode("*Error* in formatting string");
        }

        return do_print_string<CharT>(value, config, *mb_ostrm);
    }

    template <typename T>
    auto do_print_char(T value, Config_ const& config, std::ostringstream& ostrm) -> PrintingNode
    {
        using CharT = remove_cvref_t<T>;

        switch (getOstreamTypeMode(ostrm))
        {
        case OstreamTypeMode::character:
            print_text<'\''>(BasicStringView<CharT>(&value, 1), config, ostrm);
            return PrintingNode(ostrm.str());

        case OstreamTypeMode::none:
        case OstreamTypeMode::debug:
            ostrm << '\'';
            print_text<'\''>(BasicStringView<CharT>(&value, 1), config, ostrm);
            ostrm << '\'';
            return PrintingNode(ostrm.str());

        case OstreamTypeMode::non_binary_integer:
        case OstreamTypeMode::binary:
        case OstreamTypeMode::BINARY:
            return do_print_integral(std::char_traits<CharT>::to_int_type(value), config, ostrm);

        default:
            return PrintingNode("*Error* in formatting string");
        }
    }

    // Print character
    template <typename T>
    auto make_printing_branch(
        T&& value, StringView fmt, Config_ const& config
    ) -> typename std::enable_if<
            is_character<remove_ref_t<T>>::value, PrintingNode
        >::type
    {
        auto mb_ostrm = build_ostream(fmt);
        if (!mb_ostrm)
        {
            return PrintingNode("*Error* in formatting string");
        }

        return do_print_char(value, config, *mb_ostrm);
    }

    // Print signed and unsigned char
    template <typename T>
    auto make_printing_branch(
        T&& value, StringView fmt, Config_ const&
    ) -> typename std::enable_if<is_xsig_char<T>::value, PrintingNode>::type
    {
        using T0 =
            typename std::conditional<
                std::is_signed<T>::value, int, unsigned int
            >::type;

        auto mb_ostrm = build_ostream(fmt);
        if (!mb_ostrm)
        {
            return PrintingNode("*Error* in formatting string");
        }

        *mb_ostrm << static_cast<T0>(value);
        return PrintingNode(mb_ostrm->str());
    }

    // Print smart pointers without an operator<<(ostream&) overload.
    template <typename T>
    auto make_printing_branch(
        T&& value, StringView fmt, Config_ const& config
    ) -> typename std::enable_if<
        // On C++20 unique_ptr will have a << overload.
        is_unstreamable_ptr<remove_ref_t<T>>::value && !is_baseline_printable<T>::value,
        PrintingNode
    >::type
    {
        return make_printing_branch(
            reinterpret_cast<void const*>(value.get()), fmt, config
        );
    }

    // Print weak pointer classes
    template <typename T>
    auto make_printing_branch(
        T&& value, StringView fmt, Config_ const& config
    ) -> typename std::enable_if<is_weak_ptr<remove_ref_t<T>>::value, PrintingNode>::type
    {
        return value.expired() ?
            PrintingNode("expired") : make_printing_branch(value.lock(), fmt, config);
    }

  #if defined(ICECREAM_OPTIONAL_HEADER)
    // Print std::optional<> classes
    template <typename T>
    auto make_printing_branch(
        T&& value, StringView fmt, Config_ const& config
    ) -> typename std::enable_if<
        is_optional<remove_ref_t<T>>::value && !is_baseline_printable<T>::value,
        PrintingNode
    >::type
    {
        auto container_fmt = StringView{};
        auto element_fmt = StringView{};

        auto const cut_idx = fmt.find(":");
        if (cut_idx != StringView::npos)
        {
            auto const fmt_parts = split(fmt, std::vector<size_t>{cut_idx});
            container_fmt = fmt_parts[0];
            element_fmt = fmt_parts[1];
        }
        else
        {
            container_fmt = fmt;
        }

        // std::optional hasn't any container formatting string
        if (!container_fmt.empty())
        {
            return PrintingNode("*Error* in formatting string");
        }

        return value.has_value() ?
            make_printing_branch(*value, element_fmt, config) : PrintingNode("nullopt");
    }
  #endif

    template <typename Variant>
    struct Visitor
    {
        Visitor(Variant const& variant, StringView fmt, Config_ const& config)
            : variant_(variant)
            , elements_fmt(
                [&]() -> std::vector<StringView>
                {
                    if (fmt.empty())
                    {
                        // No element specifications is equivalent to all them having an empty string
                        return std::vector<StringView>(variant_size<Variant>::value, StringView{});
                    }

                    auto const sep = fmt[0];
                    fmt.remove_prefix(1);
                    return split(fmt, sep);
                }()
            )
            , config_(config)
        {}

        template <typename T>
        auto operator()(T const& arg) -> PrintingNode
        {
            if (this->elements_fmt.size() != variant_size<Variant>::value)
            {
                return PrintingNode("*Error* in formatting string");
            }

            auto const fmt = this->elements_fmt.at(this->variant_.index());
            return make_printing_branch(arg, fmt, this->config_);
        }

        Variant const& variant_;
        std::vector<StringView> elements_fmt;
        Config_ const& config_;
    };

    template <typename T>
    auto do_print_variant(
        T&& value, StringView fmt, Config_ const& config
    ) -> typename std::enable_if<!is_variant<remove_ref_t<T>>::value, PrintingNode>::type
    {
        ICECREAM_UNREACHABLE;
        return PrintingNode("");
    }

    template <typename T>
    auto do_print_variant(
        T&& value, StringView fmt, Config_ const& config
    ) -> typename std::enable_if<is_variant<remove_ref_t<T>>::value, PrintingNode>::type
    {
        return visit(Visitor<remove_cvref_t<T>>(value, fmt, config), value);
    }

    // Print *::variant<Ts...> classes
    template <typename T>
    auto make_printing_branch(
        T&& value, StringView fmt, Config_ const& config
    ) -> typename std::enable_if<
        is_variant<remove_ref_t<T>>::value && !is_baseline_printable<T>::value,
        PrintingNode
    >::type
    {
        return do_print_variant(std::forward<T>(value), fmt, config);
    }

    template <int... N, typename T>
    inline auto tuple_traverser(
        int_sequence<N...>,
        T const& t,
        std::vector<StringView> const& fmt,
        Config_ const& config
    ) -> std::vector<PrintingNode>
    {
        auto result = std::vector<PrintingNode>{};

        (void) std::initializer_list<int>{
            (
                result.push_back(
                    make_printing_branch(std::get<N>(t), fmt[N], config)
                ),
                0
            )...
        };

        return result;
    }

    template <typename T>
    auto do_print_tuple(
        T&&, StringView, Config_ const&
    ) ->
        typename std::enable_if<
            !is_tuple<remove_cvref_t<T>>::value
            , PrintingNode
        >::type
    {
        ICECREAM_UNREACHABLE;
        return PrintingNode("");
    }

    template <typename T>
    auto do_print_tuple(
        T&& value, StringView fmt, Config_ const& config
    ) ->
        typename std::enable_if<
            is_tuple<remove_cvref_t<T>>::value
            , PrintingNode
        >::type
    {
        auto const tuple_size = std::tuple_size<remove_cvref_t<T>>::value;

        auto opening = std::string{"("};
        auto separator = std::string{", "};
        auto closing = std::string{")"};

        if (!fmt.empty() && fmt[0] == 'n')
        {
            opening = "";
            separator = ", ";
            closing = "";
            fmt.remove_prefix(1);
        }
        else if (!fmt.empty() && fmt[0] == 'm')
        {
            if (tuple_size != 2)
            {
                return PrintingNode(
                    "<*Error*: the `m` specifier is only valid for pairs or 2-tuples>"
                );
            }
            opening = "";
            separator = ": ";
            closing = "";
            fmt.remove_prefix(1);
        }

        auto const elements_fmt =
            [&]() -> std::vector<StringView>
            {
                if (fmt.empty())
                {
                    // No element specifications is equivalent to all them having an empty string
                    return std::vector<StringView>(tuple_size, StringView{});
                }

                auto const sep = fmt[0];
                fmt.remove_prefix(1);
                return split(fmt, sep);
            }();

        if (elements_fmt.size() != tuple_size)
        {
            return PrintingNode(
                "<*Error*: expected "
                + std::to_string(tuple_size)
                + " element formatting specifiers>"
            );
        }

        return PrintingNode(
            opening,
            separator,
            closing,
            tuple_traverser(
                make_int_sequence<tuple_size>(),
                value,
                elements_fmt,
                config
            )
        );
    }

    // Print tuple like classes
    template <typename T>
    auto make_printing_branch(
        T&& value, StringView fmt, Config_ const& config
    ) -> typename std::enable_if<
        is_tuple<remove_cvref_t<T>>::value && !is_baseline_printable<T>::value,
        PrintingNode
    >::type
    {
        return do_print_tuple(std::forward<T>(value), fmt, config);
    }


    // -------------------------------------------------- Range classes

    // Direct representation of a slicing string.
    struct Slice
    {
        Optional<ptrdiff_t> start;
        Optional<ptrdiff_t> stop;
        Optional<ptrdiff_t> step;

        // Receives a slicing string, like "[:-2:2]" and returns an instance of `Slice`
        // class that represents it. If the string is an invalid slicing, returns nothing.
        static auto build(StringView fmt) -> Optional<Slice>
        {
            if (fmt.empty())
            {
                return Slice{{}, {}, {}};
            }

            // All iterable slicings must start with '[' and finish with ']'
            if (fmt.front() != '[' || fmt.back() != ']')
            {
                return {};
            }

            // Remove the opening '[' and the closing ']'
            fmt.remove_prefix(1);
            fmt.remove_suffix(1);

            // An empty '[]' slicing is invalid
            if (fmt.empty())
            {
                return {};
            }

            auto indexes = std::vector<Optional<ptrdiff_t>>{};
            for (auto idx_string : split(fmt, ':'))
            {
                idx_string.trim();

                if (idx_string.empty())
                {
                    indexes.push_back({});
                    continue;
                }

                auto end = static_cast<char*>(nullptr);
                auto const null_terminated = idx_string.to_string();
                auto const lnum = strtol(null_terminated.data(), &end, 10);
                if (
                    // if idx_string in its whole isn't a number
                    end != (null_terminated.data() + null_terminated.size())

                    // or if there is an overflow on long int
                    || (
                        (
                         lnum == std::numeric_limits<long>::max()
                         || lnum == std::numeric_limits<long>::lowest()
                        )
                        && errno == ERANGE
                    )

                    // or if there is an overflow to convert from long to ptrdiff
                    || (
                        lnum > std::numeric_limits<ptrdiff_t>::max()
                        || lnum < std::numeric_limits<ptrdiff_t>::lowest()
                    )
                ) {
                    return {};
                }

                indexes.push_back(lnum);
            }

            // If indexing an element ("[3]") instead of a proper slicing.
            if (indexes.size() == 1 && indexes[0])
            {
                auto const idx = *indexes[0];
                return Slice{idx, idx+1, {}};
            }

            while (indexes.size() < 3)
            {
                indexes.push_back({});
            }

            if (indexes.size() != 3) return {};

            return Slice{indexes[0], indexes[1], indexes[2]};
        }
    };

    // Advances `it` for `n` steps. Will stop before advancing `n` steps if `sentinel` is
    // reached. Returns the actual displacement.
    template <typename I, typename S>
    auto advance_it(I it, S const& sentinel, size_t n) -> I
    {
        auto offset = size_t{0};
        while (it != sentinel && offset != n)
        {
            std::advance(it, 1);
            offset += 1;
        }

        return it;
    }

    // A functor whose nullary function call operator will return an Optional element in
    // the range [it, sentinel) spaced by `step`. After exhausting the range, any call
    // will return an empty Optional.
    template <typename I, typename S>
    class SliceFunctorImpl
    {
    private:
        I it;
        S sentinel;
        size_t step;

    public:
        SliceFunctorImpl(I it, S sentinel, size_t step)
            : it(it)
            , sentinel(sentinel)
            , step(step)
        {}

        auto operator()() -> Optional<get_reference_t<I>>
        {
            if (this->it == this->sentinel) return {};

            auto old_it = this->it;
            this->it = advance_it(this->it, this->sentinel, this->step);

            return {*old_it};
        }
    };

    template <typename R>
    using SliceFunctor = std::function<Optional<get_reference_t<get_iterator_t<R>>>()>;

    template <typename R, typename I, typename S>
    auto make_slice_functor(I iterator, S sentinel, size_t step) -> SliceFunctor<R>
    {
        return SliceFunctorImpl<I, S>{iterator, sentinel, step};
    }

    template <typename R>
    auto build_slice_functor_p(
        R&& range,
        Optional<size_t> mb_start,
        Optional<size_t> mb_stop,
        size_t step
    ) -> SliceFunctor<R>
    {
        auto const start_it = mb_start ? advance_it(begin(range), end(range), *mb_start) : begin(range);

        if (!mb_stop)
        {
            return make_slice_functor<R>(start_it, end(range), step);
        }
        else if (is_sized<R>::value)  // So it has a value and it is normalized
        {
            auto stop_it = begin(range);
            std::advance(stop_it, *mb_stop);
            return make_slice_functor<R>(start_it, stop_it, step);
        }
        else
        {
            return make_slice_functor<R>(
                start_it,
                advance_it(begin(range), end(range), *mb_stop),
                step
            );
        }
    }

    template <typename R>
    auto build_slice_functor_n(
        R&& range,
        Optional<size_t> mb_start,
        Optional<size_t> mb_stop,
        size_t step
    ) ->
        typename std::enable_if<
            is_bidirectional_range<R>::value,
            SliceFunctor<R>
        >::type
    {
        auto make_reverse_iterator = [](get_iterator_t<R> it)
        {
            return std::reverse_iterator<get_iterator_t<R>>(it);
        };

        auto start_it = mb_start ?
            advance_it(begin(range), end(range), *mb_start) :

            // An Iterator instance evaluating equal to the range Sentinel. Since we will
            // backwardly iterate over the range, that is the default starting point.
            advance_it(begin(range), end(range), std::numeric_limits<size_t>::max());

        // When derreferencing a `reverse_iterator<IT>`, the retrieved element will be one
        // before the element pointed by IT. So here we fix that offset.
        if (start_it != end(range)) ++start_it;

        // If we don't have a stop value, set it to the beginning of the range
        if (!mb_stop)
        {
            auto stop_it = begin(range);
            return make_slice_functor<R>(
                make_reverse_iterator(start_it), make_reverse_iterator(stop_it), step
            );
        }

        // If we do have a stop value, and its value is normalized within [0, range_size]
        else if (is_sized<R>::value)
        {
            auto stop_it = begin(range);
            std::advance(stop_it, *mb_stop);
            if (stop_it != end(range)) ++stop_it;

            return make_slice_functor<R>(
                make_reverse_iterator(start_it), make_reverse_iterator(stop_it), step
            );
        }

        // If we do have a stop value, and its value is not normalized
        else
        {
            auto stop_it = advance_it(begin(range), end(range), *mb_stop);
            return make_slice_functor<R>(
                make_reverse_iterator(start_it), make_reverse_iterator(stop_it), step
            );
        }
    }

    template <typename R>
    auto build_slice_functor_n(R&& r, Optional<size_t>, Optional<size_t>, size_t) ->
        typename std::enable_if<
            !is_bidirectional_range<R>::value,
            SliceFunctor<R>
        >::type
    {
        ICECREAM_UNREACHABLE;
        return make_slice_functor<R>(begin(r), begin(r), 0);
    }

    template <typename R>
    auto maybe_get_size(
        R&& range
    ) -> typename std::enable_if<
        has_size_function_overload<R>::value,
        Optional<size_t>
    >::type
    {
        return size(range);
    }

    template <typename R>
    auto maybe_get_size(
        R&& range
    ) -> typename std::enable_if<
        has_size_method<R>::value && !has_size_function_overload<R>::value,
        Optional<size_t>
    >::type
    {
        return range.size();
    }

    template <typename R>
    auto maybe_get_size(
        R&&
    ) -> typename std::enable_if<
        !is_sized<R>::value,
        Optional<size_t>
    >::type
    {
        return {};
    }

    template <typename R>
    auto maybe_make_slice_functor(
        R&& range, Slice const& slice
    ) -> Variant<std::string,  SliceFunctor<R>>
    {
        auto const mb_range_size = maybe_get_size(range);

        if (
            mb_range_size
            && *mb_range_size > static_cast<size_t>(std::numeric_limits<ptrdiff_t>::max())
        ) {
            return std::string("<this range size greater than the maximum supported>");
        }

        // If the range size is unknown it is not possible to normalize a negative index.
        // So here we return an error message if we have that situation.
        if (
            !mb_range_size
            && ((slice.start && *slice.start < 0) || (slice.stop && *slice.stop < 0))
        ) {
            return std::string{"<this range supports only non-negative start and stop slice indexes>"};
        }

        auto const step = slice.step ? *slice.step : 1;
        if (step == 0)
        {
            return std::string{"<slice step cannot be zero>"};
        }
        if (!is_bidirectional_range<R>::value && step < 0)
        {
            return std::string{"<this range supports only strictly positive slice steps>"};
        }

        auto const mb_start =
            [&]() -> Optional<size_t> {
                if (!slice.start)
                {
                    return {};
                }
                else if (mb_range_size)
                {
                    auto const range_size = static_cast<ptrdiff_t>(*mb_range_size);
                    auto const idx = (*slice.start >= 0) ? *slice.start : *slice.start + range_size;
                    return (idx < 0) ?
                        0
                        : (idx > range_size) ? static_cast<size_t>(range_size) : static_cast<size_t>(idx);
                }
                else
                {
                    // If range is unsized, start was ensured as non-negative above
                    return static_cast<size_t>(*slice.start);
                }
            }();

        auto const mb_stop =
            [&]() -> Optional<size_t> {
                if (!slice.stop)
                {
                    return {};
                }
                else if (mb_range_size)
                {
                    auto const range_size = static_cast<ptrdiff_t>(*mb_range_size);
                    auto const idx = (*slice.stop >= 0) ? *slice.stop : *slice.stop + range_size;

                    if (step < 0 && idx < 0)
                    {
                        return {};
                    }
                    else
                    {
                        return (idx < 0) ?
                            0
                            : (idx > range_size) ? static_cast<size_t>(range_size) : static_cast<size_t>(idx);
                    }
                }
                else
                {
                    // If range is unsized, stop was ensured as non-negative above
                    return static_cast<size_t>(*slice.stop);
                }
            }();

        // If the stop point would be placed before the start point. Return an empty slicing.
        if (
            mb_start && mb_stop &&
            (
                (step > 0 && *mb_stop <= *mb_start) ||
                (step < 0 && *mb_stop >= *mb_start)
            )
        ) {
            return build_slice_functor_p(range, 0, 0, 1);
        }

        // At here, we know for sure that for all start, stop, and step; if they have a
        // value, that value is non-negative and step non zero too. In addition to that,
        // stop for sure is placed at the same location as start or in a place after
        // start.

        if (step > 0)
        {
            return build_slice_functor_p(range, mb_start, mb_stop, static_cast<size_t>(step));
        }
        else  // step < 0
        {
            return build_slice_functor_n(range, mb_start, mb_stop, static_cast<size_t>(-step));
        }
    }


    // Receives a range formatting string, "[:3]:#x" for instance, and splits it in a
    // pair: the range formatting itself ("[:3]") and the elements formatting ("#x"). The
    // cut point is the leftmost colon that is outside of a square bracket pair.
    inline auto split_range_fmt_string(StringView fmt) -> std::tuple<StringView, StringView>
    {
        auto is_inside_square_brackets = false;
        for (auto i = size_t{0}; i < fmt.size(); ++i)
        {
            if (fmt[i] == '[')
            {
                is_inside_square_brackets = true;
            }
            else if (fmt[i] == ']')
            {
                is_inside_square_brackets = false;
            }
            else if (!is_inside_square_brackets && fmt[i] == ':')
            {
                auto const iterable_fmt = fmt.substr(0, i);
                auto const element_fmt = fmt.substr(i + 1, StringView::npos);
                return {iterable_fmt, element_fmt};
            }
        }

        // If there isn't a colon cut point, all the input `fmt` string is the iterable
        // formatting
        return {fmt, ""};
    }

    // --------------------------------------------------

    template <typename T>
    auto do_print_range(
        T&&, StringView, Config_ const&
    ) -> typename std::enable_if<!is_range<T>::value, PrintingNode>::type
    {
        ICECREAM_UNREACHABLE;
        return PrintingNode("");
    }

    template <typename T>
    auto do_print_range(
        T&& value, StringView fmt, Config_ const& config
    ) -> typename std::enable_if<is_range<T>::value , PrintingNode>::type
    {
        auto range_fmt = StringView{};
        auto elements_fmt = StringView{};
        std::tie(range_fmt, elements_fmt) = split_range_fmt_string(fmt);

        auto const mb_slice = Slice::build(range_fmt);
        if (!mb_slice)
        {
            return PrintingNode("<invalid range slicing>");
        }

        auto mb_slice_functor = maybe_make_slice_functor(value, *mb_slice);

        // If there was any error while creating the SliceFunctor
        if (mb_slice_functor.index() == 0)
        {
            return PrintingNode(get<std::string>(mb_slice_functor));
        }

        auto slice_functor = get<SliceFunctor<T const>>(mb_slice_functor);

        auto children = std::vector<PrintingNode>{};
        auto mb_element = slice_functor();
        while (mb_element)
        {
            children.push_back(make_printing_branch(*mb_element, elements_fmt, config));
            mb_element = slice_functor();
        }

        auto const opening = range_fmt.empty() ? "[" : range_fmt.to_string() + "->[";
        return PrintingNode(opening, ", ", "]", std::move(children));
    }

    // Print all elements of a range
    template <typename T>
    auto make_printing_branch(
        T&& value, StringView fmt, Config_ const& config
    ) -> typename std::enable_if<
        (
            is_range<T>::value                      // it is range
            && !is_baseline_printable<T>::value     // but it is not printable by baseline strategies
            && !bypass_baseline_printing<T>::value  // and it hasn't its own strategy
         ) || (
            // Except by arrays of anything other than character, which are baseline
            // printable, but should be printed by this strategy instead.
            std::is_array<remove_ref_t<T>>::value
            && !is_c_string<remove_ref_t<T>>::value
        )
        , PrintingNode
    >::type
    {
        return do_print_range(std::forward<T>(value), fmt, config);
    }

    // ---------------------------------------------------------------------------------------------

    // Print classes deriving from only std::exception and not from boost::exception
    template <typename T>
    auto make_printing_branch(
        T&& value, StringView, Config_ const&
    ) -> typename std::enable_if<
        std::is_base_of<std::exception, remove_cvref_t<T>>::value
        && !std::is_base_of<boost::exception, remove_cvref_t<T>>::value
        && !is_baseline_printable<T>::value,
        PrintingNode
    >::type
    {
        return PrintingNode(value.what());
    }

    // Print classes deriving from both std::exception and boost::exception
    template <typename T>
    auto make_printing_branch(
        T&& value, StringView, Config_ const&
    ) -> typename std::enable_if<
        std::is_base_of<std::exception, remove_cvref_t<T>>::value
        && std::is_base_of<boost::exception, remove_cvref_t<T>>::value
        && !is_baseline_printable<T>::value,
        PrintingNode
    >::type
    {
        return PrintingNode(
            value.what()
            + std::string {"\n"}
            + boost::exception_detail::diagnostic_information_impl(
                  &value, &value, true, true
            )
        );
    }

  #if defined(ICECREAM_DUMP_STRUCT_CLANG)
    static auto ds_this = static_cast<PrintingNode*>(nullptr);
    static auto ds_delayed_structs = std::vector<std::pair<std::string, std::function<void()>>>{};
    static auto ds_fmt_ref = StringView{};
    static auto ds_config_ref = static_cast<Config_ const*>(nullptr);
    static auto ds_call_count = int{0};
    template<typename... T> auto parse_dump_struct(char const* format, T&& ...args) -> int;

    // Print classes using clang's __builtin_dump_struct (clang >= 15).
    template <typename T>
    auto make_printing_branch(
        T&& value, StringView fmt, Config_ const& config
    ) -> typename std::enable_if<is_handled_by_clang_dump_struct<T>::value, PrintingNode>::type
    {
        // If this is the outermost class being printed
        if (!ds_this)
        {
            auto branch_node = PrintingNode("");

            ds_this = &branch_node;  // Put `this` on scope to be assigned inside `parse_dump_struct`
            ds_fmt_ref = fmt;
            ds_config_ref = &config;
            __builtin_dump_struct(&value, &parse_dump_struct);  // Print the outermost class

            // Loop on each class that is an internal attribute of the outermost class
            while (!ds_delayed_structs.empty())
            {
                auto const delayed_struct = ds_delayed_structs.back();
                ds_delayed_structs.pop_back();

                // Put that attribute class on scope to `parse_struct_dump`
                ds_this = branch_node.find_leaf(delayed_struct.first);

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

            return branch_node;
        }

        // Otherwise, the class at hand is an internal attribute from the outermost
        // class. If this is the case, this present calling of Tree constructor is
        // being made from inside a __builtin_dump_struct calling. So here we delay
        // the calling of __builtin_dump_struct to avoid a reentrant function call.
        else
        {
            auto const unique_id =
                "icecream_415a8a88-aa51-44d5-8ccb-377d953413ef_" + std::to_string(ds_call_count);

            ds_delayed_structs.emplace_back(
                unique_id,
                [&value]()
                {
                    __builtin_dump_struct(&value, &parse_dump_struct);
                }
            );
            ds_call_count += 1;

            // Return a placeholder node that will be replaced by the `if` above at the right time.
            return PrintingNode(unique_id);
        }
    }

    // Receives an argument from parse_dump_struct and builds a Tree using it.
    //
    // When clang doesn't know how to format a value within __builtin_dump_struct, it will
    // pass a pointer to that value instead of a reference. This function specialization
    // will handle that scenario, as it specializes on pointers. However, to distinguish
    // between actual pointer values and a "don't know how to format" value, the `deref`
    // boolean is used.
    template<typename T>
    auto build_tree(bool deref, T* const& t) -> PrintingNode
    {
        if (deref)
        {
            return make_printing_branch(*t, ds_fmt_ref, *ds_config_ref);
        }
        else
        {
            return make_printing_branch(t, ds_fmt_ref, *ds_config_ref);
        }
    }

    // Receives an argument from parse_dump_struct and builds a Tree using it.
    template<typename T>
    auto build_tree(bool, T const& t) -> PrintingNode
    {
        return make_printing_branch(t, ds_fmt_ref, *ds_config_ref);
    }

    // Receives all the variadic inputs from parse_dump_struct and returns a pair with the
    // argument name and its Tree representation.
    // This overload accepts zero, one, or two values from the variadic inputs. It is
    // here just so the code compiles and never will be called. When receiving these
    // number of inputs an argument to be printed will never be among them.
    inline auto build_name_tree_pair(
        bool, std::string const& = "", std::string const& = ""
    ) -> std::pair<std::string, PrintingNode>
    {
        ICECREAM_UNREACHABLE;
        return std::pair<std::string, PrintingNode>("", PrintingNode(""));
    }

    // Receives all the variadic inputs from parse_dump_struct and returns a pair with the
    // argument name and its Tree representation.
    // This overload will be called when starting the printing of an argument that is
    // itself a struct with other attributes. The empty `Tree` is a placeholder that will
    // be replaced when the actual tree is built.
    inline auto build_name_tree_pair(
        bool, std::string const&, std::string const&, std::string const& arg_name
    ) -> std::pair<std::string, PrintingNode>
    {
        return std::pair<std::string, PrintingNode>(arg_name, PrintingNode(""));
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
    ) -> std::pair<std::string, PrintingNode>
    {
        return std::pair<std::string, PrintingNode>(arg_name, build_tree(deref, value));
    }

    // Each call to `parse_dump_struct` will deal with at most one attribute from the
    // struct being printed. This `attributes_stack` will hold the pairs (argument name,
    // Tree) constructed to each attribute until all of them are processed.
    static auto attributes_stack = std::vector<std::vector<std::pair<std::string, PrintingNode>>>{};

    // When printing the attributes of a base class (inheritance), Clang will open and
    // close braces, similarly to when printing an attribute that is a struct
    // (composition). It is possible distinguish the two cases when at opening, but they
    // are identical when closing. This vector mark if the current context is inside an
    // inheritance attributes or a composition attributes.
    static auto is_inside_baseclass = std::vector<bool>{};

    template<typename... T>
    int parse_dump_struct(char const* format_, T&&... args)
    {
        // Removes any left and right white spaces
        auto const trim =
            [](std::string const& str) -> std::string
            {
                auto const left = str.find_first_not_of(" \t\n");
                if (left == std::string::npos) return "";

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
        // printing the content of an atrribute that is a struct.
        else if (sizeof...(args) == 3)
        {
            is_inside_baseclass.push_back(false);

            attributes_stack.back().push_back(
                build_name_tree_pair(!knows_how_to_format(format), args...)
            );

            // The next attributes will be from this struct attribute being printed.
            // Collect them all before merging on a Tree.
            attributes_stack.emplace_back();
        }

        // When true, it is printing an actual value. Like "int i = 7".
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

            auto top_atrributes = std::move(attributes_stack.back());
            attributes_stack.pop_back();

            auto children = std::vector<PrintingNode>{};
            for (auto& att : top_atrributes)
            {
                auto t_pair = std::vector<PrintingNode>{};
                t_pair.push_back(PrintingNode(std::get<0>(att)));
                t_pair.push_back(std::move(std::get<1>(att)));

                children.emplace_back("", ": ", "", std::move(t_pair));
            }

            auto built_tree = PrintingNode("{", ", ", "}", std::move(children));

            if (attributes_stack.empty())
            {
                *ds_this = std::move(built_tree);
            }
            else
            {
                std::get<1>(attributes_stack.back().back()) = std::move(built_tree);
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


    // -------------------------------------------------- is_printable

    template <typename T>
    auto is_printable_impl(int) ->
        decltype(
            make_printing_branch(
                std::declval<T&>(),
                std::declval<StringView&>(),
                std::declval<Config_ const&>()
            ),
            std::true_type{}
        );

    template <typename T>
    auto is_printable_impl(...) -> std::false_type;

    // Check if IceCream can print the type T.
    template <typename T>
    using is_printable = decltype(is_printable_impl<T>(0));


    // -------------------------------------------------- Icecream

    // Holds a pair of argument and formatting string. Created by the macro `IC_`.
    template <typename T>
    struct FormattingArgument
    {
        std::string fmt;
        T&& value;
    };

    template <typename T>
    struct formatting_argumet_type_impl {using type = T;};

    template <typename T>
    struct formatting_argumet_type_impl<FormattingArgument<T>&> {using type = T;};

    template <typename T>
    using formatting_argumet_type =
        typename formatting_argumet_type_impl<T>::type;


    // If the type has an overload of to_string, call it.
    template <typename T>
    auto call_to_string(T const& t) ->
        typename std::enable_if<
            has_to_string<T>::value,
            std::string
        >::type
    {
        return to_string(t);
    }

    // If the type is a string like, just return it.
    template <typename T>
    auto call_to_string(T const& t) ->
        typename std::enable_if<
            !has_to_string<T>::value
            && (
                is_character<remove_ref_t<T>>::value
                || is_c_string<remove_ref_t<T>>::value
                || is_std_string<remove_ref_t<T>>::value
                || is_string_view<remove_ref_t<T>>::value
            ),
            std::string
        >::type
    {
        return t;
    }

    // Just to compile, should not be called.
    template <typename T>
    auto call_to_string(T const&) ->
        typename std::enable_if<
            !has_to_string<T>::value
            && !(
                is_character<remove_ref_t<T>>::value
                || is_c_string<remove_ref_t<T>>::value
                || is_std_string<remove_ref_t<T>>::value
                || is_string_view<remove_ref_t<T>>::value
            ),
            std::string
        >::type
    {
        ICECREAM_UNREACHABLE;
        return "";
    }

    // Would be better if this function could be on calling site as a lambda
    // function, however older compilers (gcc<=9 and clang<=10 for example) fail
    // to see a parameter pack expansion when inside of a lambda.
    template <typename T>
    auto concat_fmt(T const& t, std::string& fmt, size_t& i_arg, size_t n_args) -> void
    {
        ++i_arg;
        if (i_arg < n_args)
        {
            fmt += call_to_string(t);
        }
    }

    // Returns a FormattingArgument where the value type is the same as the type
    // of the last `args`, and the formatting string is the concatenation of all
    // arguments except by the last.
    template <typename... Ts>
    auto make_formatting_argument(
        Ts&&... args
    ) -> FormattingArgument<typename std::tuple_element<sizeof...(Ts)-1, std::tuple<Ts&&...>>::type>
    {
        auto constexpr value_idx = sizeof...(Ts) - 1;
        using T = typename std::tuple_element<value_idx, std::tuple<Ts&&...>>::type;

        auto i_arg = size_t{0};
        auto fmt = std::string{};

        // Build `fmt` as the concatenation of all args, except by the last,
        // after converting them to a string through `call_to_string`.
        (void) std::initializer_list<int>{
            (concat_fmt(args, fmt, i_arg, sizeof...(Ts)), 0)...
        };

        T value = std::get<value_idx>(std::forward_as_tuple(std::forward<Ts>(args)...));
        return FormattingArgument<T>{fmt, std::forward<T>(value)};
    }


    // Holds a triple of argument name, formatting string, and value. Those are
    // all the info required to print an argument.
    template <typename T>
    struct PrintingArgument
    {
        StringView name;
        StringView fmt;
        T&& value;
    };


    // Print a forest that fits into a single line.
    inline auto print_one_line_forest(
        StringView prefix,
        StringView context,
        StringView delimiter,
        std::vector<std::tuple<StringView, PrintingNode>> const& forest
    ) -> std::string
    {
        auto result = prefix.to_string();
        if (!context.empty())
        {
            result += context.to_string() + delimiter.to_string();
        }

        for (auto it = forest.begin(); it != forest.end(); ++it)
        {
            auto const& arg_name = std::get<0>(*it);
            auto const& tree = std::get<1>(*it);

            result += arg_name.to_string() + ": " + tree.print();

            if (it+1 != forest.end())
            {
                    result += ", ";
            }
        }

        return result;
    }

    // Print a forest that will be broken on multiple lines.
    inline auto print_multi_line_forest(
        StringView prefix,
        StringView context,
        std::vector<std::tuple<StringView, PrintingNode>> const& forest,
        size_t const line_wrap_width
    ) -> std::string
    {
        auto result = prefix.to_string();
        if (!context.empty())
        {
            result += context.to_string();
        }
        result +=  + "\n";

        for (auto it = forest.begin(); it != forest.end(); ++it)
        {
            auto const& arg_name = std::get<0>(*it);
            auto const& tree = std::get<1>(*it);

            result += std::string(Config_::INDENT_BASE, ' ') + arg_name.to_string() + ": ";
            if (count_utf8_code_point(result) + tree.code_point_length() < line_wrap_width)
            {
                result += tree.print();
            }
            else
            {
                result += tree.print(2, line_wrap_width);
            }

            if (it+1 != forest.end())
            {
                result += ",\n";
            }
        }

        return result;
    }

    template <typename... Ts>
    auto build_forest(
        Config_ const& config, PrintingArgument<Ts>&... args
    ) -> std::vector<std::tuple<StringView, PrintingNode>>
    {
        auto forest = std::vector<std::tuple<StringView, PrintingNode>>{};
        (void) std::initializer_list<int>{
            (
                (void) forest.emplace_back(
                    args.name, make_printing_branch(args.value, args.fmt, config)
                ),
                0
            )...
        };

        return forest;
    }

    template <typename... Ts>
    auto print_args(
        Config_ const& config,
        StringView file,
        int line,
        StringView function,
        PrintingArgument<Ts>... args
    ) -> typename std::enable_if<
            detail::conjunction<detail::is_printable<Ts>...>::value
        >::type
    {
        if (!config.is_enabled()) return;

        auto const prefix = config.prefix()();
        auto const context =
            [&]() -> std::string
            {
                // If used an empty IC macro, i.e.: IC().
                if (config.include_context())
                {
                  #if defined(_MSC_VER)
                    auto const n = file.rfind('\\');
                  #else
                    auto const n = file.rfind('/');
                  #endif
                    return
                        file.substr(n+1).to_string()
                        + ":"
                        + std::to_string(line)
                        + " in \""
                        + function.to_string()
                        + '"';
                }
                else
                {
                    return "";
                }
            }();
        auto const delimiter = config.context_delimiter();

        auto const forest = build_forest(config, args...);

        // The number of codepoints used if the whole forest would be printed in an one
        // line.
        auto const one_line_forest_n_code_points =
            [&]() -> size_t
            {
                auto n = count_utf8_code_point(prefix);

                for (auto const& name_tree : forest)
                {
                    n +=
                        count_utf8_code_point(std::get<0>(name_tree))
                        + count_utf8_code_point(": ")
                        + std::get<1>(name_tree).code_point_length();
                }

                // The ", " separators between variables.
                if (forest.size() > 1)
                {
                    n += (forest.size() - 1) * count_utf8_code_point(", ");
                }

                return n;
            }();

        if (one_line_forest_n_code_points > config.line_wrap_width())
        {
            config.output()(
                config.output_transcoder()(
                    print_multi_line_forest(prefix, context, forest, config.line_wrap_width())
                )
            );
        }
        else
        {
            config.output()(
                config.output_transcoder()(
                    print_one_line_forest(prefix, context, delimiter, forest)
                )
            );
        }

        config.output()(config.output_transcoder()("\n"));
    }

    // Handles the printing of a nullary IC() call.
    inline auto print_nullary(
        Config_ const& config,
        StringView file,
        int line,
        StringView function
    ) -> void
    {
        if (!config.is_enabled()) return;

        auto const prefix = config.prefix()();
        auto const context =
            [&]() -> std::string
            {
              #if defined(_MSC_VER)
                auto const n = file.rfind('\\');
              #else
                auto const n = file.rfind('/');
              #endif

                return
                    file.substr(n+1).to_string()
                    + ":"
                    + std::to_string(line)
                    + " in \""
                    + function.to_string()
                    + '"';
            }();

        auto const str = prefix + context + "\n";
        config.output()(config.output_transcoder()(str));
    }

    /** This function will receive a string as "foo, bar, baz" and return a vector with
     * all the arguments split, such as ["foo", "bar", "baz"].
     */
    inline auto split_arguments(StringView all_names) -> std::vector<StringView>
    {
        if (all_names.empty())
        {
            return {};
        }

        // Check if the '>' char is the closing of a template arguments listing. It will
        // be a template closing at `std::is_same<int, int>::value` but not at `5 > 2`
        auto is_closing_template =
            [&all_names](size_t idx, size_t const last_idx) -> bool
            {
                auto curr_char = all_names[idx];
                if (curr_char != '>' || idx == last_idx)
                {
                    return false;
                }

                do
                {
                    ++idx;
                    curr_char = all_names[idx];
                } while (
                    idx != last_idx
                    && (
                        curr_char == ' '
                        || curr_char == '\t'
                        || curr_char == '\n'
                        || curr_char == '\r'
                        || curr_char == '\f'
                        || curr_char == '\v'
                    )
                );

                return curr_char == ':' && idx != last_idx && all_names[idx + 1] == ':';
            };

        auto parenthesis_count = int{0};
        auto angle_bracket_count = int{0};
        auto curly_bracket_count = int{0};
        auto is_within_double_quote = false;
        auto is_within_single_quote = false;

        // Parse the arguments string backward. It is easier this way to check if a '<' or
        // '>' character is either a comparison operator, or a opening and closing of
        // templates arguments.
        auto comma_idxs = std::vector<size_t>{};
        auto last_idx = all_names.size() - 1;
        for (auto idx = all_names.size(); idx-- > 0;)
        {
            auto const curr_char = all_names[idx];
            auto const prev_char = idx > 0 ? all_names[idx-1] : '\0';

            if (is_within_double_quote)
            {
                // if current char is the left double quote
                if (curr_char == '"' && prev_char != '\\') is_within_double_quote = false;
            }

            else if (is_within_single_quote)
            {
                // if current char is the left single quote
                if (curr_char == '\'' && prev_char != '\\') is_within_single_quote = false;
            }

            else if (curr_char == '"' && prev_char != '\\')
            {
                is_within_double_quote = true;
            }

            else if (curr_char == '\'' && prev_char != '\\')
            {
                is_within_single_quote = true;
            }

            // If it have found the comma separating two arguments
            else if (
                curr_char == ','
                && parenthesis_count == 0
                && angle_bracket_count == 0
                && curly_bracket_count == 0
            ) {
                comma_idxs.push_back(idx);
                last_idx = idx - 1;
            }

            // It won't cut on a comma within parentheses, such as when the argument is a
            // function call, as in IC(foo(1, 2))
            else if (curr_char == ')')
            {
                ++parenthesis_count;
            }

            else if (curr_char == '(')
            {
                --parenthesis_count;
            }

            else if (curr_char == '}')
            {
                ++curly_bracket_count;
            }

            else if (curr_char == '{')
            {
                --curly_bracket_count;
            }

            // It won't cut on a comma within a template argument list, such as in
            // IC(std::is_same<int, int>::value)
            else if (is_closing_template(idx, last_idx))
            {
                ++angle_bracket_count;
            }

            else if (curr_char == '<' && angle_bracket_count > 0)
            {
                --angle_bracket_count;
            }
        }

        auto result = split(all_names, {comma_idxs.rbegin(), comma_idxs.rend()});
        for (auto& name : result)
        {
            name.trim();
        }

        return result;
    }

    template <typename T>
    auto get_value(T&& t) ->
        typename std::enable_if<
            !is_instantiation<FormattingArgument, remove_cvref_t<T>>::value,
            T&&
        >::type
    {
        return std::forward<T>(t);
    }

    template <typename T>
    auto get_value(T&& t) ->
        typename std::enable_if<
            is_instantiation<FormattingArgument, remove_cvref_t<T>>::value,
            decltype(t.value)
        >::type
    {
        return std::forward<decltype(t.value)>(t.value);
    }

    template <typename T>
    auto get_fmt(T const&, StringView default_format) -> StringView
    {
        return default_format;
    }

    template <typename T>
    auto get_fmt(FormattingArgument<T> const& t, StringView) -> StringView
    {
        return t.fmt;
    }

    // The use of this struct instead of a free function is a needed hack because of the
    // trailing comma problem with __VA_ARGS__ expansion. A macro like:
    //
    // IC(...) print(__FILE__, __LINE__, ICECREAM_FUNCTION, #__VA_ARGS__, __VA_ARGS__)
    //
    // when used with no arguments would expand to one of:
    //
    // print("foo.cpp", 42, "void bar()", "",)
    // print("foo.cpp", 42, "void bar()", ,)
    //
    // A trailing comma is invalid when calling a function with parentheses, but allowed
    // when constructing an object with braces.
    //
    class Dispatcher
    {
    public:
        // Used by compilers that expand an empyt __VA_ARGS__ in
        // Dispatcher{bla, #__VA_ARGS__} to Dispatcher{bla, ""}
        Dispatcher(
            bool is_ic_apply,
            Config_ const& config,
            StringView file,
            int line,
            StringView function,
            StringView default_format,
            StringView arg_names
        )
            : is_ic_apply_(is_ic_apply)
            , config_(config)
            , file_(file)
            , line_{line}
            , function_(function)
            , default_format_(default_format)
            , arg_names_(arg_names)
        {}

        // Used by compilers that expand an empyt __VA_ARGS__ in
        // Dispatcher{bla, #__VA_ARGS__} to Dispatcher{bla, }
        Dispatcher(
            bool is_ic_apply,
            Config_ const& config,
            StringView file,
            int line,
            StringView function,
            StringView default_format
        )
            : Dispatcher(is_ic_apply, config, file, line, function, default_format, "")
        {}

        // Return a std::tuple with all the args
        template <typename... Ts>
        auto ret(Ts&&... args) -> decltype(std::forward_as_tuple(std::forward<Ts>(args)...))
        {
            this->dispatch(make_int_sequence<sizeof...(Ts)>(), args...);
            return std::forward_as_tuple(std::forward<Ts>(args)...);
        }

        // Return the unique arg
        template <typename T>
        auto ret(T&& arg) -> decltype(arg)
        {
            this->dispatch(make_int_sequence<1>(), arg);
            return std::forward<decltype(arg)>(arg);
        }

    private:
        template <int... N, typename... Ts>
        auto dispatch(int_sequence<N...>, Ts&&... args) -> void
        {
            // Pick the name of an IC macro's "to be printed" argument. Usually that would
            // just return the argument string itself. However, when using the IC_ macro
            // to set a formatting string to an argument, all that information will be
            // mixed on that `IC_` call as a whole, and the argument name will need to be
            // extracted from there.
            auto pick_argument_name =
                [](StringView ic_argument) -> StringView
                {
                    constexpr char prefix[] = "IC_(";
                    constexpr auto n_prefix = sizeof(prefix);

                    if (ic_argument.find(prefix) == StringView::npos)
                    {
                        return ic_argument;
                    }
                    else
                    {
                        auto const count = ic_argument.size() - n_prefix - 1;
                        auto const fmt_args = split_arguments(ic_argument.substr(n_prefix, count));
                        return fmt_args.back();
                    }
                };

            auto arg_names = std::vector<StringView>{};
            for (auto const& argument : split_arguments(this->arg_names_))
            {
                arg_names.push_back(pick_argument_name(argument));
            }

            if (this->is_ic_apply_)
            {
                // Remove the callable name from the arguments
                arg_names.erase(arg_names.begin());
            }

            if (sizeof...(Ts) == 0 && !this->is_ic_apply_)
            {
                // Even if it has no arguments (besides the callable name), an `IC_A`
                // macro isn't a nullary `IC()` call.
                print_nullary(config_, file_, line_, function_);
            }
            else
            {
                print_args(
                    config_,
                    file_,
                    line_,
                    function_,
                    PrintingArgument<formatting_argumet_type<Ts>>{
                        arg_names.at(N),
                        get_fmt(args, this->default_format_),
                        get_value(std::forward<Ts>(args))
                    }...
                );
            }
        }

        bool is_ic_apply_;
        Config_ const& config_;
        StringView file_;
        int line_;
        StringView function_;
        StringView default_format_;
        StringView arg_names_;
    };

    // --------------------------------------------------- Range View

    template <typename Proj>
    struct RangeViewArgs
    {
        Optional<std::string> mb_name;
        Proj proj;
        std::string elements_fmt;
        Optional<Slice> mb_slice;
        Config_ const* config_ = nullptr;
        int line_;
        std::string src_location;
        std::string file_;
        std::string function_;

        RangeViewArgs(Optional<std::string> const& name, std::string const& fmt, Proj&& proj)
            : mb_name(name)
            , proj(proj)
        {
            auto view_fmt = StringView{};
            auto elements_fmt_ = StringView{};
            std::tie(view_fmt, elements_fmt_) = split_range_fmt_string(fmt);
            this->elements_fmt = elements_fmt_.to_string();
            this->mb_slice = Slice::build(view_fmt);
        }

        auto complete(
            Config_& config,
            int line,
            std::string file,
            std::string function
          #if defined(ICECREAM_SOURCE_LOCATION)
            ,std::source_location const location = std::source_location::current()
          #endif
        ) -> RangeViewArgs&
        {
            this->config_ = &config;
            this->line_ = line;
            this->file_ = file;
            this->function_ = function;

          #if defined(ICECREAM_SOURCE_LOCATION)
            (void)line;
            this->src_location =
                std::to_string(location.line()) + ":" + std::to_string(location.column());
          #else
            this->src_location = std::to_string(line);
          #endif

            return *this;
        }
    };

    template <typename Proj=Identity>
    auto IC_FV_(
        std::string const& fmt, std::string const& name, Proj&& proj = Identity{}
    ) -> RangeViewArgs<Proj>
    {
        return RangeViewArgs<Proj>(name, fmt, std::forward<Proj>(proj));
    }

    template <typename Proj=Identity>
    auto IC_FV_(
        std::string const& fmt, Proj&& proj = Identity{}
    ) -> typename std::enable_if<
        !is_string_convertible<Proj>::value,
        RangeViewArgs<Proj>
    >::type
    {
        return RangeViewArgs<Proj>({}, fmt, std::forward<Proj>(proj));
    }

    template <typename Proj=Identity>
    auto IC_V_(
        std::string const& name, Proj&& proj = Identity{}
    ) -> RangeViewArgs<Proj>
    {
        return RangeViewArgs<Proj>(name, "", std::forward<Proj>(proj));
    }

    template <typename Proj=Identity>
    auto IC_V_(
        Proj&& proj = Identity{}
    ) -> typename std::enable_if<
        !is_string_convertible<Proj>::value,
        RangeViewArgs<Proj>
    >::type
    {
        return RangeViewArgs<Proj>({}, "", std::forward<Proj>(proj));
    }


    template <typename Proj>
    struct RangeView
    {
        std::string name;
        Proj proj;
        Optional<Slice> mb_slice;
        std::string elements_fmt;
        Config_ const& config;
        int line;
        std::string file;
        std::string function;

        int current_idx = 0;
        ptrdiff_t start;
        ptrdiff_t stop;
        ptrdiff_t step;
        Optional<std::string> slice_error;

        explicit RangeView(RangeViewArgs<Proj> par)
            : name(
                par.mb_name ? *par.mb_name : std::string{"range_view_"} + par.src_location
            )
            , proj(par.proj)
            , mb_slice(par.mb_slice)
            , elements_fmt(par.elements_fmt)
            , config(*par.config_)
            , line(par.line_)
            , file(par.file_)
            , function(par.function_)
        {}

        auto normalize_slice() -> void
        {
            if (!this->mb_slice)
            {
                this->slice_error = std::string{"<invalid slice formatting string>"};
                return;
            }

            auto const mb_start = this->mb_slice->start;
            auto const mb_stop = this->mb_slice->stop;
            auto const mb_step = this->mb_slice->step;

            if ((mb_start && *mb_start < 0) || (mb_stop && *mb_stop < 0))
            {
                this->slice_error = std::string{
                    "<partial views supports only non-negative slice indexes>"
                };
            }

            if (mb_step && *mb_step < 0)
            {
                this->slice_error = std::string{"<slice steps must be strictly positive>"};
            }

            this->start = mb_start ? *mb_start : 0;
            this->stop = mb_stop ? *mb_stop : std::numeric_limits<ptrdiff_t>::max();
            this->step = mb_step ? *mb_step : 1;
        }

        // Will be called by the operator `Range | IC_VIEW_`. Here we will know what is the
        // ranges and we can inspect its capabilities.
        template <typename R>
        auto normalize_slice(R&& range) -> void
        {
            if (!this->mb_slice)
            {
                this->slice_error = std::string{"<invalid slice formatting string>"};
                return;
            }

            auto const mb_size = maybe_get_size(range);

            auto const mb_start = this->mb_slice->start;
            auto const mb_stop = this->mb_slice->stop;
            auto const mb_step = this->mb_slice->step;

            if (!mb_size && ((mb_start && *mb_start < 0) || (mb_stop && *mb_stop < 0)))
            {
                this->slice_error = std::string{"<this range view supports only non-negative slice indexes>"};
            }

            if (mb_step && *mb_step < 0)
            {
                this->slice_error = std::string{"<slice steps must be strictly positive>"};
            }

            auto normalize_idx = [&](ptrdiff_t idx) -> ptrdiff_t
            {
                if (mb_size)
                {
                    auto const range_size = static_cast<ptrdiff_t>(*mb_size);
                    idx = (idx >= 0) ? idx : idx + range_size;
                    return (idx < 0) ? 0 : idx;
                }
                else
                {
                    return idx;
                }
            };

            this->start = mb_start ? normalize_idx(*mb_start) : 0;
            this->stop = mb_stop ? normalize_idx(*mb_stop) : std::numeric_limits<ptrdiff_t>::max();
            this->step = mb_step ? *mb_step : 1;
        }

        template <typename T>
        auto operator()(T&& element) -> T&&
        {
            auto const idx = this->current_idx++;
            auto const arg_name = this->name + "[" + std::to_string(idx) + "]";
            auto dispatcher = Dispatcher{
                false,
                this->config,
                this->file,
                this->line,
                this->function,
                this->elements_fmt,
                arg_name
            };

            if (this->slice_error)
            {
                dispatcher.ret(*this->slice_error);
            }
            else if (
                this->start <= idx && idx < this->stop
                && ((idx - this->start) % this->step) == 0
            ) {
                using TConst = typename std::add_const<remove_ref_t<T>>::type;
                dispatcher.ret(this->proj(const_cast<TConst&>(element)));
            }

            return std::forward<T>(element);
        }
    };

  #if defined(ICECREAM_LIB_RANGES)
    // View | IC_V
    template <typename T, typename Proj>
    requires std::ranges::view<T>
    auto operator|(T&& t, RangeViewArgs<Proj> view_args)
    {
        auto rv = RangeView<Proj>(view_args);
        rv.normalize_slice(t);
        return std::forward<T>(t) | std::views::transform(std::move(rv));
    }

    // PartialView | IC_V
    template <typename T, typename Proj>
    requires std::ranges::view<resolve_view_t<T>>
    auto operator|(T&& t, RangeViewArgs<Proj> view_args)
    {
        auto rv = RangeView<Proj>(view_args);
        rv.normalize_slice();
        return std::forward<T>(t) | std::views::transform(std::move(rv));
    }

    // IC_V | PartialView
    template <typename T, typename Proj>
    requires std::ranges::view<resolve_view_t<T>>
    auto operator|(RangeViewArgs<Proj> view_args, T&& t)
    {
        auto rv = RangeView<Proj>(view_args);
        rv.normalize_slice();
        return std::views::transform(std::move(rv)) | std::forward<T>(t);
    }

    // Range | IC_V
    // Here we don't know yet if we are in a Range-v3 or STL Ranges pipeline.
    template <typename T, typename Proj>
    requires (!std::ranges::view<T> && !std::ranges::view<resolve_view_t<T>>)
    auto operator|(T& t, RangeViewArgs<Proj> view_args) -> std::pair<T&, RangeViewArgs<Proj>>
    {
        return {t, std::move(view_args)};
    }

    // Pair (Range, IC_V) | PartialView
    template <typename T0, typename T1, typename Proj>
    requires std::ranges::view<resolve_view_t<T1>>
    auto operator|(std::pair<T0&, RangeViewArgs<Proj>> t0, T1&& t1)
    {
        auto rv = RangeView<Proj>(t0.second);
        rv.normalize_slice(t0.first);
        return t0.first | std::views::transform(std::move(rv)) | std::forward<T1>(t1);
    }
  #endif


  #if defined(ICECREAM_RANGE_V3_ENABLED)
    // View | IC_V
    template <typename T, typename Proj>
    auto operator|(T&& t, RangeViewArgs<Proj> view_args)
        -> typename std::enable_if<
            std::is_base_of<ranges::view_base, T>::value,
            decltype(std::forward<T>(t) | rv3v::transform(std::declval<RangeView<Proj>&>()))
        >::type
    {
        auto rv = RangeView<Proj>(view_args);
        rv.normalize_slice(t);
        return std::forward<T>(t) | rv3v::transform(std::move(rv));
    }

    // PartialView | IC_V
    template <typename T, typename Proj>
    auto operator|(T&& t, RangeViewArgs<Proj> view_args)
        -> typename std::enable_if<
            std::is_base_of<ranges::view_base, resolve_view_t<T>>::value,
            decltype(std::forward<T>(t) | rv3v::transform(std::declval<RangeView<Proj>&>()))
        >::type
    {
        auto rv = RangeView<Proj>(view_args);
        rv.normalize_slice();
        return std::forward<T>(t) | rv3v::transform(std::move(rv));
    }

    // IC_V | PartialView
    template <typename T, typename Proj>
    auto operator|(RangeViewArgs<Proj> view_args, T&& t)
        -> typename std::enable_if<
            std::is_base_of<ranges::view_base, resolve_view_t<T>>::value,
            decltype(rv3v::transform(std::declval<RangeView<Proj>&>()) | std::forward<T>(t))
        >::type
    {
        auto rv = RangeView<Proj>(view_args);
        rv.normalize_slice();
        return rv3v::transform(std::move(rv)) | std::forward<T>(t);
    }

    // Range | IC_V
    // Here we don't know yet if we are in a Range-v3 or STL Ranges pipeline.
    template <typename T, typename Proj>
    auto operator|(T& t, RangeViewArgs<Proj> view_args)
        -> typename std::enable_if<
            !std::is_base_of<ranges::view_base, T>::value && !std::is_base_of<ranges::view_base, resolve_view_t<T>>::value,
            std::pair<T&, RangeViewArgs<Proj>>
        >::type
    {
        return {t, std::move(view_args)};
    }

    // Pair (Range, IC_V) | PartialView
    template <typename T0, typename T1, typename Proj>
    auto operator|(std::pair<T0&, RangeViewArgs<Proj>> t0, T1&& t1)
        -> typename std::enable_if<
            std::is_base_of<ranges::view_base, resolve_view_t<T1>>::value,
            decltype(t0.first | rv3v::transform(std::declval<RangeView<Proj>&>()) | std::forward<T1>(t1))
        >::type
    {
        auto rv = RangeView<Proj>(t0.second);
        rv.normalize_slice(t0.first);
        return t0.first | rv3v::transform(std::move(rv)) | std::forward<T1>(t1);
    }

  #endif

}} // namespace icecream::detail


namespace {
    auto& icecream_private_config_5f803a3bcdb4 = icecream::detail::Config_::global();
    auto& icecream_public_config_5f803a3bcdb4 = static_cast<::icecream::Config&>(icecream_private_config_5f803a3bcdb4);
}

#endif // ICECREAM_HPP_INCLUDED
