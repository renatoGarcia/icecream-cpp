/*
 * Copyright (c) 2019 The IceCream-Cpp Developers. See the AUTHORS file at the
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
#include <functional>
#include <iostream>
#include <iterator>
#include <locale>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>


#define ICECREAM_MAJOR_VERSION 0
#define ICECREAM_MINOR_VERSION 1
#define ICECREAM_PATCH_VERSION 0
#define ICECREAM_LIB_VERSION "0.1.0"


#if defined(__GNUC__)
    #define ICECREAM_FUNCTION __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
    #define ICECREAM_FUNCTION __FUNCSIG__
#else
    #define ICECREAM_FUNCTION __func__
#endif


#if defined(ICECREAM_LONG_NAME)
    #define ICECREAM(...) ::icecream::print{__FILE__, __LINE__, ICECREAM_FUNCTION, #__VA_ARGS__, __VA_ARGS__}
#else
    #define IC(...) ::icecream::print{__FILE__, __LINE__, ICECREAM_FUNCTION, #__VA_ARGS__, __VA_ARGS__}
#endif


namespace std
{
    template <typename T> class optional;
    template <typename T1, typename T2> struct pair;
}

namespace boost
{
    template <typename T> class scoped_ptr;
    template <typename T> class shared_ptr;
    template <typename T> class weak_ptr;
}


namespace icecream{ namespace detail
{
    // utility wrapper to adapt locale-bound facets for wstring/wbuffer convert
    template<class Facet>
    struct deletable_facet : Facet
    {
        template<class ...Args>
        deletable_facet(Args&& ...args) : Facet(std::forward<Args>(args)...) {}
        ~deletable_facet() {}
    };


    // ---------- Check if a type T is an instantiation of a template class U
    template <template<typename...> class, typename...>
    struct is_instantiation: std::false_type {};

    template <template<typename...> class U, typename... T>
    struct is_instantiation<U, U<T...>>: std::true_type {};


    // ---------- Logical AND
    template <typename... Ts>
    struct conjunction: std::true_type {};

    template <typename T, typename... Ts>
    struct conjunction<T, Ts...>: std::conditional<T::value, conjunction<Ts...>, std::false_type>::type {};


    // ---------- Logical OR
    template <typename... Ts>
    struct disjunction: std::false_type { };

    template <typename T, typename... Ts>
    struct disjunction<T, Ts...>: std::conditional<T::value, std::true_type, disjunction<Ts...>>::type {};


    // ---------- Logical NOT
    template <typename T>
    struct negation: std::conditional<T::value, std::false_type, std::true_type>::type {};


    // ---------- To allow ADL with custom begin/end
    using std::begin;
    using std::end;


    // -------------------------------------------------- is_bounded_array
    template <typename T>
    struct is_bounded_array_impl: std::false_type {};

    template <typename T, std::size_t N>
    struct is_bounded_array_impl<T[N]>: std::true_type {};

    template <typename T>
    using is_bounded_array = typename is_bounded_array_impl<
        typename std::remove_reference<T>::type
    >::type;


    // -------------------------------------------------- is_invocable
    template <typename T>
    auto is_invocable_impl(int) -> decltype (
        std::declval<T&>()(),
        std::true_type {}
    );

    template <typename T>
    auto is_invocable_impl(...) -> std::false_type;

    template <typename T>
    using is_invocable = decltype(is_invocable_impl<T>(0));


    // -------------------------------------------------- is_iterable
    template <typename T>
    auto is_iterable_impl(int) -> decltype (
        begin(std::declval<T&>()) != end(std::declval<T&>()),   // begin end operator!=
        ++std::declval<decltype(begin(std::declval<T&>()))&>(), // operator++
        *begin(std::declval<T&>()),                             // operator*
        std::true_type {}
    );

    template <typename T>
    auto is_iterable_impl(...) -> std::false_type;

    template <typename T>
    using is_iterable = decltype(is_iterable_impl<T>(0));


    // -------------------------------------------------- has_insertion
    template <typename T>
    auto has_insertion_impl(int) -> decltype (
        std::declval<std::ostream&>() << std::declval<T&>(),
        std::true_type {}
    );

    template <typename T>
    auto has_insertion_impl(...) -> std::false_type;

    template <typename T>
    using has_insertion = decltype(has_insertion_impl<T>(0));


    // -------------------------------------------------- is_optional
    template <typename T>
    struct is_optional: is_instantiation<std::optional, T> {};


    // -------------------------------------------------- is_pair
    template <typename T>
    struct is_pair: is_instantiation<std::pair, T> {};


    // -------------------------------------------------- is_character
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
    // char* and char[] are C strings, char[N] is not.
    template <typename T>
    using is_c_string = typename conjunction<
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
    >::type;


    // -------------------------------------------------- is_std_string
    template <typename T>
    using is_std_string = is_instantiation<
        std::basic_string,
        typename std::decay<T>::type
    >;


    // -------------------------------------------------- is_unique_pointer
    // Until C++20 std::unique_ptr has not an operator<<(ostream&) overload, so it
    // must have an own print method overload too.
    template <typename T>
    struct is_unique_ptr: is_instantiation<std::unique_ptr, T> {};


    // -------------------------------------------------- is_weak_ptr
    template <typename T>
    struct is_weak_ptr: disjunction<
        is_instantiation<std::weak_ptr, T>,
        is_instantiation<boost::weak_ptr, T>
    > {};


    // -------------------------------------------------- is_valid_prefix
    template <typename T>
    struct is_valid_prefix: disjunction<
        is_std_string<T>,
        is_c_string<typename std::decay<T>::type>,
        is_invocable<T>
    > {};


    // -------------------------------------------------------------------------------
    struct Node
    {
        std::string str;
        std::vector<Node> children;
        bool is_leaf;
    };

    // Returns the sum of characters of the whole tree defined by `node` as root.
    inline auto count_chars(Node const& node) -> int
    {
        if (node.is_leaf)
        {
            return node.str.size();
        }
        else
        {
            // The enclosing [].
            auto result = 2;

            // count the size of each child
            for (auto const& child : node.children)
                result += count_chars(child);

            // The ", " separators.
            if (node.children.size() > 1)
               result += (node.children.size() - 1) * 2;

            return result;
        }
    }

    // --------------------------------------------------

    // If value is invocable, do nothing, If it is a string, returns an function that
    // returns it.
    template <
        typename T,
        typename std::enable_if <
            is_std_string<T>::value
            || is_c_string<typename std::decay<T>::type>::value,
        int>::type = 0
    >
    auto to_invocable(T&& value) -> std::function<std::string()>
    {
        auto str = std::string {value};
        return [str](){return str;};
    }

    template <
        typename T,
        typename std::enable_if<
            is_invocable<T>::value,
        int>::type = 0
    >
    auto to_invocable(T&& value) -> T&&
    {
        return std::forward<T>(value);
    }


    // --------------------------------------------------
    class Prefix
    {
        struct ErasedFunction
        {
            virtual std::string operator()() = 0;
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

            std::string operator()() override
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
        Prefix(Ts&& ...func)
            : functions {}
        {
            (void) std::initializer_list<int> {
                (
                    (void) this->functions.emplace_back(
                        new Function<typename std::decay<Ts>::type> {
                            std::forward<Ts>(func)
                        }
                    ),
                    0
                 )...
            };
        }

        std::string operator()()
        {
            auto result = std::string {};
            for (auto const& func : this->functions)
            {
                result.append((*func)());
            }

            return result;
        }
    };


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

        auto stream() -> std::ostream&
        {
            return this->stream_;
        }

        template <typename... Ts>
        auto prefix(Ts&& ...value) -> void
        {
            this->prefix_ = Prefix {
                to_invocable(std::forward<Ts>(value))...
            };
        }

        auto show_c_string() const noexcept -> bool
        {
            return this->show_c_string_;
        }

        auto show_c_string(bool value) noexcept -> void
        {
            this->show_c_string_ = value;
        }

        auto lineWrapWidth() const noexcept -> std::size_t
        {
            return this->lineWrapWidth_;
        }

        auto lineWrapWidth(std::size_t value) noexcept -> void
        {
            this->lineWrapWidth_ = value;
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
            auto const prefix = this->prefix_();

            this->stream_ << prefix;

            // If used an empty IC macro, i.e.: IC().
            if (sizeof...(Ts) == 0)
            {
                auto const n = file.rfind('/');
                this->stream_ << file.substr(n+1) << ':' << line << " in \"" << function << '"';
            }
            else
            {
                auto const forest = this->build_forest(
                    std::begin(arg_names), std::forward<Ts>(args)...
                );
                this->print_forest(forest, prefix.size());
            }

            this->stream_ << std::endl;
        }

    private:

        constexpr static size_type INDENT_BASE = 4;


        std::ostream stream_ {std::cout.rdbuf()};

        Prefix prefix_ {[]{return "ic| ";}};

        std::size_t lineWrapWidth_ = 70;

        bool show_c_string_ = true;

        Icecream() = default;

        // Print any class that overloads operator<<(std::ostream&, T)
        template <typename T>
        auto value_to_tree(T const& value) -> typename
            std::enable_if<
                has_insertion<T>::value
                && !is_c_string<T>::value
                && !is_character<T>::value
                && !is_std_string<T>::value
                && !std::is_array<T>::value,
                Node
            >::type
        {
            auto buf = std::ostringstream {};
            buf << value;
            return {buf.str(), {}, true};
        }

        // Print C string
        template <typename T>
        auto value_to_tree(T const& value) -> typename
            std::enable_if<
                is_c_string<T>::value,
                Node
            >::type
        {
            using DT = typename std::decay<
                typename std::remove_pointer<
                    typename std::decay<T>::type
                >::type
            >::type;

            std::wstring_convert<
                deletable_facet<std::codecvt<DT, char, std::mbstate_t>>, DT
            > cv {};

            auto buf = std::ostringstream {};

            if (this->show_c_string_)
                buf << '"' << cv.to_bytes(value) << '"';
            else
                buf << reinterpret_cast<void const*>(value);

            return {buf.str(), {}, true};
        }

        // Print std::string
        template <typename T>
        auto value_to_tree(T const& value) -> typename
            std::enable_if<
                is_std_string<T>::value,
                Node
            >::type
        {
            std::wstring_convert<
                deletable_facet<
                    std::codecvt<typename T::value_type, char, std::mbstate_t>
                >,
                typename T::value_type
            > cv {};

            auto buf = std::ostringstream {};
            buf << '"' << cv.to_bytes(value) << '"';
            return {buf.str(), {}, true};
        }

        // Print character
        template <typename T>
        auto value_to_tree(T const& value) -> typename
            std::enable_if<
                is_character<T>::value,
                Node
            >::type
        {
            std::wstring_convert<
                deletable_facet<
                    std::codecvt<typename std::decay<T>::type, char, std::mbstate_t>
                >,
                typename std::decay<T>::type
            > cv {};

            auto str = std::string {};
            switch (value)
            {
            case T{'\0'}:
                str = "\\0";
                break;

            default:
                str = cv.to_bytes(value);
                break;
            }

            auto buf = std::ostringstream {};
            buf << '\'' << str << '\'';
            return {buf.str(), {}, true};
        }


        // Until C++20 std::unique_ptr had not an operator<<(ostream&) overload
        template <typename T>
        auto value_to_tree(T const& value) -> typename
            std::enable_if<
                is_unique_ptr<T>::value,
                Node
            >::type
        {
            auto buf = std::ostringstream {};
            buf << reinterpret_cast<void const*>(value.get());
            return {buf.str(), {}, true};
        }

        // Print weak pointer classes
        template <typename T>
        auto value_to_tree(T const& value) -> typename
            std::enable_if<
                is_weak_ptr<T>::value,
                Node
            >::type
        {
            if (value.expired())
                return {"expired", {}, true};
            else
                return this->value_to_tree(value.lock());
        }

        // Print std::optional<> classes
        template <typename T>
        auto value_to_tree(T const& value) -> typename
            std::enable_if<
                is_optional<T>::value
                && !has_insertion<T>::value,
                Node
            >::type
        {
            if (value.has_value())
                return this->value_to_tree(*value);
            else
                return {"nullopt", {}, true};
        }

        // Print std::pair<> class
        template <typename T>
        auto value_to_tree(T const& value) -> typename
            std::enable_if<
                is_pair<T>::value
                && !has_insertion<T>::value,
                Node
            >::type
        {
            auto node = Node {"", {}, false};
            node.children.push_back(this->value_to_tree(value.first));
            node.children.push_back(this->value_to_tree(value.second));
            return node;
        }

        // Print all items of any iterable class
        template <typename T>
        auto value_to_tree(T const& value) -> typename
            std::enable_if<
                (
                    is_iterable<T>::value
                    && !has_insertion<T>::value
                    && !is_std_string<T>::value
                )
                || std::is_array<T>::value,
                Node
            >::type
        {
            using std::begin;
            using std::end;

            auto node = Node {"", {}, false};

            auto it = begin(value);
            auto const e_it = end(value);
            if (it != e_it)
            {
                node.children.push_back(this->value_to_tree(*it));
                ++it;
                for (; it != e_it; ++it)
                {
                    node.children.push_back(this->value_to_tree(*it));
                }
            }

            return node;
        }

        template <typename TArg, typename... Ts>
        auto build_forest(
            std::vector<std::string>::const_iterator arg_name,
            TArg&& arg_value,
            Ts&&... args_tail
        ) -> std::vector<std::tuple<std::string, Node>>
        {
            auto tree = std::make_tuple(
               *arg_name, this->value_to_tree(std::forward<TArg>(arg_value))
            );

            auto forest = this->build_forest(
                ++arg_name,
                std::forward<Ts>(args_tail)...
            );

            forest.push_back(std::move(tree));
            return forest;
        }

        auto build_forest(
            std::vector<std::string>::const_iterator
        ) -> std::vector<std::tuple<std::string, Node>>
        {
            return std::vector<std::tuple<std::string, Node>> {};
        }

        auto print_tree(Node const& node, size_type indent) -> void
        {
            if (node.is_leaf)
            {
                this->stream_ << node.str;
            }
            else
            {
                auto const multiline = indent + count_chars(node) > this->lineWrapWidth_;

                this->stream_ << "[";

                if (multiline)
                    indent += 1;

                for (auto it = node.children.begin(); it != node.children.end(); ++it)
                {
                    this->print_tree(*it, indent);
                    if (it+1 != node.children.end())
                    {
                        if (multiline)
                            this->stream_ << ",\n" << std::string(indent, ' ');
                        else
                            this->stream_ << ", ";
                    }
                }
                this->stream_ << "]";
            }
        }

        auto print_forest(
            std::vector<std::tuple<std::string, Node>> const& forest,
            size_type const prefix_width
        ) -> void
        {
            auto line_width = prefix_width;
            for (auto const& t : forest)
                line_width += std::get<0>(t).size() + 2 + count_chars(std::get<1>(t));

            // The ", " separators.
            if (forest.size() > 1)
                line_width += (forest.size() - 1) * 2;

            // The forest is built with trees in reverse order.
            for (auto it = forest.rbegin(); it != forest.rend(); ++it)
            {
                auto const& arg_name = std::get<0>(*it);
                this->stream_ << arg_name << ": ";

                auto indent = arg_name.size() + 2;
                if (it == forest.rbegin())
                    indent += prefix_width;
                else
                    indent += Icecream::INDENT_BASE;

                this->print_tree(std::get<1>(*it), indent);

                if (it+1 != forest.rend())
                {
                    if (line_width > this->lineWrapWidth_)
                        this->stream_ <<  ",\n" << std::string(Icecream::INDENT_BASE, ' ');
                    else
                        this->stream_ <<  ", ";
                }

            }
        }
    };
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

        auto stream() -> std::ostream&
        {
            return detail::Icecream::instance().stream();
        }

        template <
            typename... Ts,
            typename std::enable_if<
                detail::conjunction<detail::is_valid_prefix<Ts>...>::value,
            int>::type = 0
        >
        auto prefix(Ts&& ...value) -> IcecreamAPI&
        {
            detail::Icecream::instance().prefix(std::forward<Ts>(value)...);
            return *this;
        }

        auto show_c_string() const noexcept -> bool
        {
            return detail::Icecream::instance().show_c_string();
        }

        auto show_c_string(bool value) noexcept -> IcecreamAPI&
        {
            detail::Icecream::instance().show_c_string(value);
            return *this;
        }

        auto lineWrapWidth() const noexcept -> std::size_t
        {
            return detail::Icecream::instance().lineWrapWidth();
        }

        auto lineWrapWidth(std::size_t value) noexcept -> IcecreamAPI&
        {
            detail::Icecream::instance().lineWrapWidth(value);
            return *this;
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
            detail::Icecream::instance().print(
                file, line, function, arg_names, std::forward<Ts>(args)...
            );
        }
    };

    namespace
    {
        IcecreamAPI ic {};
    }

    struct print
    {
        // An empty IC() macro will expand to
        // ::icecream::print{__FILE__, __LINE__, ICECREAM_FUNCTION, "",}
        // A macro like IC(foo, bar) will expand to
        // ::icecream::print{__FILE__, __LINE__, ICECREAM_FUNCTION, "foo, bar", foo, bar}
        template <typename... Ts>
        print(std::string const& file, int line, std::string const& function, std::string const& arg_names, Ts&&... args)
        {
            auto split_names = std::vector<std::string> {};
            auto b_it = std::begin(arg_names);
            auto it = std::begin(arg_names);
            auto par_count = int {0};

            // Split the the arg_names
            while (true)
            {
                if (it == std::end(arg_names) || (*it == ',' && par_count == 0))
                {
                    // Remove the trailing spaces
                    auto e_it = std::prev(it);
                    while (*e_it == ' ') --e_it;
                    ++e_it;

                    // Remove the leading spaces
                    while (*b_it == ' ') ++b_it;

                    split_names.emplace_back(b_it, e_it);
                    b_it = std::next(it);
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
    };

} // namespace icecream


#endif // ICECREAM_HPP_INCLUDED
