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

#include <codecvt>
#include <exception>
#include <functional>
#include <iostream>
#include <iterator>
#include <locale>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
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

#define ICECREAM_MAJOR_VERSION 0
#define ICECREAM_MINOR_VERSION 3
#define ICECREAM_PATCH_VERSION 1
#define ICECREAM_LIB_VERSION "0.3.1"


#if defined(__GNUC__)
    #define ICECREAM_FUNCTION __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
    #define ICECREAM_FUNCTION __FUNCSIG__
#else
    #define ICECREAM_FUNCTION __func__
#endif


#if defined(ICECREAM_LONG_NAME)
    #define ICECREAM(...) ::icecream::detail::Dispatcher{__FILE__, __LINE__, ICECREAM_FUNCTION, #__VA_ARGS__}.ret(__VA_ARGS__)
    #define ICECREAM0() ::icecream::detail::Dispatcher{__FILE__, __LINE__, ICECREAM_FUNCTION, ""}.ret()
#else
    #define IC(...) ::icecream::detail::Dispatcher{__FILE__, __LINE__, ICECREAM_FUNCTION, #__VA_ARGS__}.ret(__VA_ARGS__)
    #define IC0() ::icecream::detail::Dispatcher{__FILE__, __LINE__, ICECREAM_FUNCTION, ""}.ret()
#endif


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
            boost::exception const*,
            std::exception const*,
            bool with_what,
            bool verbose
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
    template<> struct FIX<char> {using type = uint8_t;};
    template<> struct FIX<signed char> {using type = int8_t;};
    template<> struct FIX<unsigned char> {using type = uint8_t;};
    template<> struct FIX<wchar_t> {using type = uint16_t;};
    template<> struct FIX<char16_t> {using type = uint16_t;};
    template<> struct FIX<char32_t> {using type = uint32_t;};
#else
    template<typename T> struct FIX {using type = T;};
#endif

    // Utility wrapper to adapt locale-bound facets for wstring/wbuffer convert
    template<class Facet>
    struct deletable_facet: Facet
    {
        template<typename... Ts>
        deletable_facet(Ts&& ...args) : Facet(std::forward<Ts>(args)...) {}
        ~deletable_facet() {}
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
    struct negation: std::conditional<T::value, std::false_type, std::true_type>::type {};


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
    static auto returned_type_impl(int) -> decltype(
        std::declval<T&>()()
    );

    template <typename T>
    static auto returned_type_impl(...) -> void;

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
        is_instantiation<std::pair, T>,
        is_instantiation<std::tuple, T>
    >;


    // -------------------------------------------------- is_character

    // Checks if T is character type (char, char16_t, etc).
    template <typename T>
    using is_character = disjunction<
        std::is_same<typename std::decay<T>::type, char>,
        std::is_same<typename std::decay<T>::type, signed char>,
        std::is_same<typename std::decay<T>::type, unsigned char>,
        std::is_same<typename std::decay<T>::type, wchar_t>,
    #if defined(__cpp_char8_t)
        std::is_same<typename std::decay<T>::type, char8_t>,
    #endif
        std::is_same<typename std::decay<T>::type, char16_t>,
        std::is_same<typename std::decay<T>::type, char32_t>
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

    // Checks if T is a std::basic_string<typename U> or
    // std::basic_string_view<typename U> instantiation.
    template <typename T>
    using is_std_string = disjunction<
        is_instantiation<std::basic_string, T>
    #if defined(ICECREAM_STRING_VIEW_HEADER)
        ,is_instantiation<std::basic_string_view, T>
    #endif
    >;


    // -------------------------------------------------- is_collection

    // Checks if T is a collection, i.e.: an iterable type that is not a std::string.
    template <typename T>
    using is_collection = conjunction<
        is_iterable<T>,
        negation<is_std_string<T>>
    >;


    // -------------------------------------------------- elements_type

    // Returns the type of a collection elements.
    template <typename T>
    static auto elements_type_impl(int) -> decltype(
        *begin(std::declval<T&>())
    );

    template <typename T>
    static auto elements_type_impl(...) -> void;

    template <typename T>
    using elements_type = decltype(elements_type_impl<T>(0));


    // -------------------------------------------------- is_not_streamable_ptr

    // Checks if T is either std::unique_ptr<typename U> instantiation (Until C++20), or a
    // boost::scoped_ptr<typename U>. Both are without an operator<<(ostream&) overload.
    template <typename T>
    using is_not_streamable_ptr = disjunction<
        is_instantiation<std::unique_ptr, T>,
        is_instantiation<boost::scoped_ptr, T>
    >;


    // -------------------------------------------------- is_weak_ptr

    // Checks if T is a instantiation if either std::weak_ptr<typename U> or
    // boost::weak_ptr<typename U>.
    template <typename T>
    using is_weak_ptr = disjunction<
        is_instantiation<std::weak_ptr, T>,
        is_instantiation<boost::weak_ptr, T>
    >;


    // -------------------------------------------------- is_valid_prefix

    // Checks if T can be used as prefix, i.e.: T is a string or a nullary function
    // returning a type that has a "ostream <<" overload.
    template <typename T>
    using is_valid_prefix = disjunction<
        is_std_string<T>,
        is_c_string<typename std::decay<T>::type>,
        conjunction<
            is_invocable<T>,
            has_insertion<returned_type<T>>
        >
    >;


    // -------------------------------------------------- Tree

    // Needed to access the Icecream::show_c_string() method before the Icecream class
    // declaration.
    auto show_c_string() -> bool;

    struct Tree
    {
        bool is_leaf;

        union U
        {
            std::string leaf;
            struct Stem
            {
                char open;
                std::vector<Tree> children;
                char close;
            } stem;

            U(bool is_leaf)
            {
                if (is_leaf)
                    new (&leaf) std::string;
                else
                    new (&stem) Stem;
            }

            ~U() {}

        } content;


        Tree() = delete;
        Tree(Tree const&) = delete;
        Tree& operator=(Tree const&) = delete;

        Tree(Tree&& other)
            : is_leaf {other.is_leaf}
            , content {other.is_leaf}
        {
            if (this->is_leaf)
                this->content.leaf = std::move(other.content.leaf);
            else
                this->content.stem = std::move(other.content.stem);
        }

        Tree& operator=(Tree&& other)
        {
            if (this->is_leaf != other.is_leaf)
            {
                this->~Tree();
                this->is_leaf = other.is_leaf;
                new (&this->content) U {this->is_leaf};
            }

            if (this->is_leaf)
                this->content.leaf = std::move(other.content.leaf);
            else
                this->content.stem = std::move(other.content.stem);

            return *this;
        }

        ~Tree()
        {
            if (this->is_leaf)
                this->content.leaf.~basic_string();
            else
                this->content.stem.~Stem();
        }

        // Returns the sum of characters of the whole tree.
        auto count_chars() const -> int
        {
            if (this->is_leaf)
            {
                return this->content.leaf.size();
            }
            else
            {
                // The enclosing chars.
                auto result = 2;

                // count the size of each child
                for (auto const& child : this->content.stem.children)
                    result += child.count_chars();

                // The ", " separators.
                if (this->content.stem.children.size() > 1)
                    result += (this->content.stem.children.size() - 1) * 2;

                return result;
            }
        }

        // Print any class that overloads operator<<(std::ostream&, T)
        template <typename T>
        Tree(T const& value,
            typename std::enable_if<
                has_insertion<T>::value
                && !is_c_string<T>::value
                && !is_character<T>::value
                && !is_std_string<T>::value
                && !std::is_array<T>::value
            >::type* = 0
        )
            : is_leaf {true}
            , content {true}
        {
            auto buf = std::ostringstream {};
            buf << value;
            this->content.leaf = buf.str();
        }

        // Print C string
        template <typename T>
        Tree(T const& value,
            typename std::enable_if<
                is_c_string<T>::value
            >::type* = 0
        )
            : is_leaf {true}
            , content {true}
        {
            using DT = typename std::decay<
                typename std::remove_pointer<
                    typename std::decay<T>::type
                >::type
            >::type;

            using FF = typename FIX<DT>::type;
            std::wstring_convert<
                deletable_facet<std::codecvt<FF, char, std::mbstate_t>>,
                FF
            > cv {};

            auto buf = std::ostringstream {};

            if (show_c_string())
            {
        #if defined(_MSC_VER) && _MSC_VER <= 1916
                FF const* e = (FF const*)value;
                while (*e != 0) ++e;
                buf << '"' << cv.to_bytes((FF const*)value, e) << '"';
        #elif defined(__APPLE__)
                buf << '"' << value << '"';
        #else
                buf << '"' << cv.to_bytes(value) << '"';
        #endif
            }
            else
                buf << reinterpret_cast<void const*>(value);

            this->content.leaf = buf.str();
        }

        // Print std::string
        template <typename T>
        Tree(T const& value,
            typename std::enable_if<
                is_std_string<T>::value
            >::type* = 0
        )
            : is_leaf {true}
            , content {true}
        {
            using FF = typename FIX<typename T::value_type>::type;
            std::wstring_convert<
                deletable_facet<std::codecvt<FF, char, std::mbstate_t>>,
                FF
            > cv {};

            auto buf = std::ostringstream {};
        #if defined(_MSC_VER) && _MSC_VER <= 1916
            FF const* b = (FF const*)value.data();
            FF const* e = (FF const*)value.data();
            while (*e != 0) ++e;
            buf << '"' << cv.to_bytes(b, e) << '"';
        #elif defined(__APPLE__)
            buf << '"' << value << '"';
        #else
            buf << '"' << cv.to_bytes(value.data()) << '"';
        #endif
            this->content.leaf = buf.str();
        }

        // Print character
        template <typename T>
        Tree(T const& value,
            typename std::enable_if<
                is_character<T>::value
            >::type* = 0
        )
            : is_leaf {true}
            , content {true}
        {
            using FF = typename FIX<typename std::decay<T>::type>::type;
            std::wstring_convert<
                deletable_facet<
                    std::codecvt<FF, char, std::mbstate_t>
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
            #if defined(__APPLE__)
                str = value;
            #else
                str = cv.to_bytes(value);
            #endif
                break;
            }

            auto buf = std::ostringstream {};
            buf << '\'' << str << '\'';
            this->content.leaf = buf.str();
        }

        // Print smart pointers without an operator<<(ostream&) overload.
        template <typename T>
        Tree(T const& value,
            typename std::enable_if<
                is_not_streamable_ptr<T>::value
                && !has_insertion<T>::value // On C++20 unique_ptr will have a << overload.
            >::type* = 0
        )
            : is_leaf {true}
            , content {true}
        {
            auto buf = std::ostringstream {};
            buf << reinterpret_cast<void const*>(value.get());
            this->content.leaf = buf.str();
        }

        // Print weak pointer classes
        template <typename T>
        Tree(T const& value,
            typename std::enable_if<
                is_weak_ptr<T>::value
            >::type* = 0
        )
            : is_leaf {true}
            , content {true}
        {
            if (value.expired())
                this->content.leaf = "expired";
            else
                *this = Tree {value.lock()};
        }

    #if defined(ICECREAM_OPTIONAL_HEADER)
        // Print std::optional<> classes
        template <typename T>
        Tree(T const& value,
            typename std::enable_if<
                is_instantiation<std::optional, T>::value
                && !has_insertion<T>::value
            >::type* = 0
        )
            : is_leaf {true}
            , content {true}
        {
            if (value.has_value())
                *this = Tree {*value};
            else
                this->content.leaf = "nullopt";
        }
    #endif

        struct Visitor
        {
            template <typename T>
            auto operator()(T const& arg) -> Tree
            {
             return Tree {arg};
            }
        };


        // Print boost::variant2::variant<> classes
        template <typename T>
        Tree(T const& value,
            typename std::enable_if<
             (
                is_instantiation<boost::variant2::variant, T>::value
             #if defined(ICECREAM_VARIANT_HEADER)
                || is_instantiation<std::variant, T>::value
             #endif
             )
                && !has_insertion<T>::value
            >::type* = 0
        )
            : is_leaf {true}
            , content {true}
        {
            *this = visit(Tree::Visitor{}, value);
        }

        // Fill this->content.stem.children with all the tuple elements
        template<typename T, std::size_t N = std::tuple_size<T>::value-1>
        auto tuple_traverser(T const& t) -> void
        {
            if (N > 0)
                tuple_traverser<T, (N > 0) ? N-1 : 0>(t);
            this->content.stem.children.emplace_back(std::get<N>(t));
        }

        // Print tuple like classes
        template <typename T>
        Tree(T const& value,
            typename std::enable_if<
                is_tuple<T>::value
                && !has_insertion<T>::value
            >::type* = 0
        )
            : is_leaf {false}
            , content {false}
        {
            this->content.stem.open = '(';
            this->content.stem.close = ')';
            this->tuple_traverser(value);
        }

        // Print all items of any iterable class
        template <typename T>
        Tree(T const& value,
            typename std::enable_if<
                (
                    is_iterable<T>::value
                    && !has_insertion<T>::value
                    && !is_std_string<T>::value
                )
                || std::is_array<T>::value
            >::type* = 0
        )
            : is_leaf {false}
            , content {false}
        {
            using std::begin;
            using std::end;

            this->content.stem.open = '[';
            this->content.stem.close = ']';

            auto it = begin(value);
            auto const e_it = end(value);
            for (; it != e_it; ++it)
                this->content.stem.children.emplace_back(*it);
        }

        // Print classes deriving from only std::exception and not from boost::exception
        template <typename T>
        Tree(T const& value,
            typename std::enable_if<
                std::is_base_of<std::exception, T>::value
                && !std::is_base_of<boost::exception, T>::value
                && !has_insertion<T>::value
             >::type* = 0
        )
            : is_leaf {true}
            , content {true}
        {
            this->content.leaf = value.what();
        }

        // Print classes deriving from both std::exception and boost::exception
        template <typename T>
        Tree(T const& value,
            typename std::enable_if<
                std::is_base_of<std::exception, T>::value
                && std::is_base_of<boost::exception, T>::value
                && !has_insertion<T>::value
            >::type* = 0
        )
            : is_leaf {true}
            , content {true}
        {
            this->content.leaf =
                value.what()
                + std::string {"\n"}
                + boost::exception_detail::diagnostic_information_impl(
                      &value, &value, true, true
                );
        }
    };


    // -------------------------------------------------- is_tree_argument

    template <typename T>
    auto is_tree_argument_impl(int) -> decltype (
        Tree {std::declval<T&>()},
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

    template <typename T0, typename T1>
    struct is_printable<std::pair<T0, T1>&>: conjunction<
        is_printable<T0>,
        is_printable<T1>
    > {};

    template <typename... Ts>
    struct is_printable<std::tuple<Ts...>&>: conjunction<
        is_printable<Ts>...
    > {};


    // -------------------------------------------------- to_invocable

    // If value is a string returns an function that returns it.
    template <typename T>
    auto to_invocable(
        T&& value,
        typename std::enable_if <
            is_std_string<T>::value
            || is_c_string<typename std::decay<T>::type>::value
        >::type* = 0
    ) -> std::function<std::string()>
    {
        auto str = std::string {value};
        return [str](){return str;};
    }

    // If value is invocable do nothing.
    template <typename T>
    auto to_invocable(
        T&& value,
        typename std::enable_if<
            is_invocable<T>::value
        >::type* = 0
    ) -> T&&
    {
        return std::forward<T>(value);
    }


    // -------------------------------------------------- Prefix
    class Prefix
    {
        struct ErasedFunction
        {
            virtual auto operator()() -> std::string = 0;
            virtual ~ErasedFunction() {};
        };

        template <typename T>
        struct Function
            : public ErasedFunction
        {
            static_assert(is_invocable<T>::value, "");
            static_assert(!std::is_reference<T>::value, "");

            Function() = delete;
            Function(Function const&) = delete;
            Function(Function&&) = default;
            Function& operator=(Function const&) = delete;
            Function& operator=(Function&&) = default;

            Function (T&& func)
                : func {std::move(func)}
            {}

            auto operator()() -> std::string override
            {
                auto buf = std::ostringstream {};
                buf << this->func();
                return buf.str();
            }

            T func;
        };

        std::vector<std::unique_ptr<ErasedFunction>> functions;

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
                    (void) this->functions.emplace_back(
                        new Function<typename std::decay<Ts>::type> {
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
            {
                result.append((*func)());
            }

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

        auto stream() -> std::ostream&
        {
            std::lock_guard<std::mutex> guard(this->mutex);
            return this->stream_;
        }

        template <typename... Ts>
        auto prefix(Ts&& ...value) -> void
        {
            std::lock_guard<std::mutex> guard(this->mutex);
            this->prefix_ = Prefix {
                to_invocable(std::forward<Ts>(value))...
            };
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
                this->stream_ << prefix << context;
            }
            else
            {
                auto const forest = Icecream::build_forest(
                    std::begin(arg_names), std::forward<Ts>(args)...
                );
                this->print_forest(prefix, context, forest);
            }

            this->stream_ << std::endl;
        }

    private:
        friend auto show_c_string() -> bool;

        constexpr static size_type INDENT_BASE = 4;

        mutable std::mutex mutex;

        bool enabled_ = true;

        std::ostream stream_ {std::cerr.rdbuf()};

        Prefix prefix_ {[]{return "ic| ";}};

        std::string context_delimiter_ = "- ";

        std::size_t line_wrap_width_ = 70;

        bool show_c_string_ = true;

        bool include_context_ = false;


        Icecream() = default;

        auto print_tree(
            Tree const& node,
            size_type const indent_level,
            bool const multiline
        ) -> void
        {
            auto const break_line = [&](int indent)
            {
                this->stream_
                    << "\n"
                    << std::string(indent * Icecream::INDENT_BASE, ' ');
            };

            if (node.is_leaf)
            {
                this->stream_ << node.content.leaf;
            }
            else
            {
                this->stream_ << node.content.stem.open;
                if (multiline)
                    break_line(indent_level+1);

                for (
                    auto it = node.content.stem.children.begin();
                    it != node.content.stem.children.end();
                    ++it
                ){
                    auto const child_multiline = [&]
                    {
                        // If the whole tree is single line all childs are too.
                        if (!multiline)
                            return false;

                        auto const line_width =
                            (indent_level+1) * Icecream::INDENT_BASE
                            + it->count_chars();

                        return line_width > this->line_wrap_width_;
                    }();

                    this->print_tree(*it, indent_level+1, child_multiline);

                    if (it+1 != node.content.stem.children.end())
                    {
                        this->stream_ << ",";
                        if (multiline)
                            break_line(indent_level+1);
                        else
                            this->stream_ << " ";
                    }
                    else if (multiline)
                        break_line(indent_level);
                }

                this->stream_ << node.content.stem.close;
            }
        }

        auto print_forest(
            std::string const& prefix,
            std::string const& context,
            std::vector<std::tuple<std::string, Tree>> const& forest
        ) -> void
        {
            auto const forest_multiline = [&]
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
            }();

            // Print prefix and context
            this->stream_ << prefix;
            if (!context.empty())
            {
                this->stream_ << context;
                if (forest_multiline)
                    this->stream_ << '\n' << std::string(Icecream::INDENT_BASE, ' ');
                else
                    this->stream_ << this->context_delimiter_;
            }

            // The forest is built with trees in reverse order.
            for (auto it = forest.rbegin(); it != forest.rend(); ++it)
            {
                auto const& arg_name = std::get<0>(*it);
                auto const& node = std::get<1>(*it);

                auto const tree_multiline = [&]
                {
                    // If the whole forest is single line all trees are too.
                    if (!forest_multiline)
                        return false;

                    auto const line_width = [&]
                    {
                        if (it == forest.rbegin() && context.empty())
                            return
                                prefix.size()
                                + arg_name.size()
                                + 2
                                + node.count_chars();
                        else
                            return
                                Icecream::INDENT_BASE
                                + arg_name.size()
                                + 2
                                + node.count_chars();
                    }();

                    return line_width > this->line_wrap_width_;
                }();

                this->stream_ << arg_name << ": ";
                this->print_tree(node, 1, tree_multiline);

                if (it+1 != forest.rend())
                {
                    if (forest_multiline)
                        this->stream_ <<  "\n" << std::string(Icecream::INDENT_BASE, ' ');
                    else
                        this->stream_ <<  ", ";
                }
            }
        }

        static auto build_forest(
            std::vector<std::string>::const_iterator
        ) -> std::vector<std::tuple<std::string, Tree>>
        {
            return std::vector<std::tuple<std::string, Tree>> {};
        }

        template <typename T, typename... Ts>
        auto build_forest(
            std::vector<std::string>::const_iterator arg_name,
            T&& arg_value,
            Ts&&... args_tail
        ) -> std::vector<std::tuple<std::string, Tree>>
        {
            auto tree = std::make_tuple(
                *arg_name, Tree {std::forward<T>(arg_value)}
            );

            auto forest = Icecream::build_forest(
                ++arg_name,
                std::forward<Ts>(args_tail)...
            );

            forest.push_back(std::move(tree));
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
    // syntactic sugar to make possible call ic.stream() instead of
    // Icecream::instance().stream().
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

        auto stream() -> std::ostream&
        {
            return detail::Icecream::instance().stream();
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

    namespace
    {
        IcecreamAPI ic {};
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
            std::string const& file,
            int line,
            std::string const& function,
            std::string const& arg_names
        )
            : file {file}
            , line {line}
            , function {function}
            , arg_names {arg_names}
        {}

        // Used by compilers that expand an empyt __VA_ARGS__ in
        // Dispatcher{bla, #__VA_ARGS__} to Dispatcher{bla, }
        Dispatcher(
            std::string const& file,
            int line,
            std::string const& function
        )
            : file {file}
            , line {line}
            , function {function}
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

        template <typename... Ts>
        auto ret(Ts&&... args) -> std::tuple<Ts...>
        {
            this->print(args...);
            return std::tuple<Ts...>{std::forward<Ts>(args)...};
        }

        template <typename T>
        auto ret(T&& arg) -> T
        {
            this->print(arg);
            return std::forward<T>(arg);
        }

        auto ret() -> void
        {
            this->print();
        }
    };

}} // namespace icecream::detail


#endif // ICECREAM_HPP_INCLUDED
