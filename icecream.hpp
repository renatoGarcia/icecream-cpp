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

#include <functional>
#include <iostream>
#include <iterator>
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


namespace icecream
{
    namespace detail
    {
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


        // -------------------------------------------------- is_c_str
        template <typename T>
        struct is_c_str: disjunction<
            std::is_same<T, char const*>,
            std::is_same<T, char*>
        > {};


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

    } // namespace detail


    class Icecream
    {
    public:
        using size_type = std::size_t;

        Icecream() = default;
        Icecream(Icecream const&) = delete;
        Icecream(Icecream&&) = delete;
        Icecream& operator=(Icecream const&) = delete;
        Icecream& operator=(Icecream&&) = delete;

        auto stream() -> std::ostream&
        {
            return this->stream_;
        }

        auto stream() const -> std::ostream const&
        {
            return this->stream_;
        }

        auto prefix(std::string const& value) -> Icecream&
        {
            this->str_prefix = value;
            this->func_prefix = nullptr;
            return *this;
        }

        auto prefix(std::function<std::string()> const& value) -> Icecream&
        {
            this->str_prefix.clear();
            this->func_prefix = value;
            return *this;
        }

        auto show_c_string() const noexcept -> bool
        {
            return this->show_c_string_;
        }

        auto show_c_string(bool value) noexcept -> Icecream&
        {
            this->show_c_string_ = value;
            return *this;
        }

        auto lineWrapWidth() const noexcept -> std::size_t
        {
            return this->lineWrapWidth_;
        }

        auto lineWrapWidth(std::size_t value) noexcept -> Icecream&
        {
            this->lineWrapWidth_ = value;
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
            auto const prefix = this->func_prefix ? this->func_prefix() : this->str_prefix;

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

        // The prefix will be one and only one of this two.
        std::string str_prefix = "ic| ";
        std::function<std::string()> func_prefix = nullptr;

        std::size_t lineWrapWidth_ = 70;

        bool show_c_string_ = true;


        // Print any class that overloads operator<<(std::ostream&, T)
        template <typename T>
        auto value_to_tree(T const& value) -> typename
            std::enable_if<
                detail::has_insertion<T>::value
                && !detail::is_c_str<T>::value,
                detail::Node
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
                detail::is_c_str<T>::value,
                detail::Node
            >::type
        {
            auto buf = std::ostringstream {};

            if (this->show_c_string_)
                buf << value;
            else
                buf << reinterpret_cast<void const*>(value);

            return {buf.str(), {}, true};
        }

        // Until C++20 std::unique_ptr had not an operator<<(ostream&) overload
        template <typename T>
        auto value_to_tree(T const& value) -> typename
            std::enable_if<
                detail::is_unique_ptr<T>::value,
                detail::Node
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
                detail::is_weak_ptr<T>::value,
                detail::Node
            >::type
        {
            auto buf = std::ostringstream {};

            if (value.expired())
                buf << "expired weak_ptr";
            else
                buf << "valid weak_ptr";

            return {buf.str(), {}, true};
        }

        // Print std::optional<> classes
        template <typename T>
        auto value_to_tree(T const& value) -> typename
            std::enable_if<
                detail::is_optional<T>::value
                && !detail::has_insertion<T>::value,
                detail::Node
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
                detail::is_pair<T>::value
                && !detail::has_insertion<T>::value,
                detail::Node
            >::type
        {
            auto node = detail::Node {"", {}, false};
            node.children.push_back(this->value_to_tree(value.first));
            node.children.push_back(this->value_to_tree(value.second));
            return node;
        }

        // Print all items of any iterable class
        template <typename T>
        auto value_to_tree(T const& value) -> typename
            std::enable_if<
                detail::is_iterable<T>::value
                && !detail::has_insertion<T>::value,
                detail::Node
            >::type
        {
            using std::begin;
            using std::end;

            auto node = detail::Node {"", {}, false};

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
        ) -> std::vector<std::tuple<std::string, detail::Node>>
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
        ) -> std::vector<std::tuple<std::string, detail::Node>>
        {
            return std::vector<std::tuple<std::string, detail::Node>> {};
        }

        auto print_tree(detail::Node const& node, size_type indent) -> void
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
            std::vector<std::tuple<std::string, detail::Node>> const& forest,
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

    namespace
    {
        Icecream ic {};
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
