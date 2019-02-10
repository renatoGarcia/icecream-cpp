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

#include <iostream>
#include <iterator>
#include <string>
#include <utility>
#include <vector>
#include <type_traits>
#include <memory>


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
        // ---------- Check if a type T is a instantiation of a template class U
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
            std::true_type{}
        );

        template <typename T>
        auto is_iterable_impl(...) -> std::false_type;

        template<typename T>
        using is_iterable = decltype(detail::is_iterable_impl<T>(0));


        // -------------------------------------------------- has_insertion
        template <typename T>
        auto has_insertion_impl(int) -> decltype (
            std::declval<std::ostream&>() << std::declval<T&>(),
            std::true_type{}
        );

        template <typename T>
        std::false_type has_insertion_impl(...);

        template <typename T>
        using has_insertion = decltype(detail::has_insertion_impl<T>(0));


        // -------------------------------------------------- is_optional
        template <typename T>
        struct is_optional: is_instantiation<std::optional, T> {};


        // -------------------------------------------------- is_pointer_like
        template <typename T>
        struct is_pointer_like: disjunction<
            std::is_pointer<T>,
            is_instantiation<std::shared_ptr, T>,
            is_instantiation<boost::scoped_ptr, T>,
            is_instantiation<boost::shared_ptr, T>
        > {};

        // Until C++20 std::unique_ptr has not an operator<<(ostream&) overload, so it
        // must have an own print method overload too.
        template <typename T>
        struct is_unique_ptr: is_instantiation<std::unique_ptr, T> {};


        // -------------------------------------------------- is_weak_pointer
        template <typename T>
        struct is_weak_pointer: disjunction<
            is_instantiation<std::weak_ptr, T>,
            is_instantiation<boost::weak_ptr, T>
        > {};

    } // namespace detail


    class Icecream
    {
    public:
        Icecream() = default;
        Icecream(Icecream const&) = delete;
        Icecream(Icecream&&) = delete;
        Icecream& operator=(Icecream const&) = delete;
        Icecream& operator=(Icecream&&) = delete;

        auto show_pointed_value() const noexcept -> bool
        {
            return this->show_pointed_value_;
        }

        auto show_pointed_value(bool value) noexcept -> Icecream&
        {
            this->show_pointed_value_ = value;
            return *this;
        }

        template<typename... Ts>
        auto print(
            std::string const& file,
            int line,
            std::string const& function,
            std::vector<std::string> const& arg_names,
            Ts&&... args
        ) -> void
        {
            std::cout << "ic| ";

            // If used an empty IC macro, i.e.: IC().
            if (sizeof...(Ts) == 0)
            {
                std::string::size_type const n = file.rfind('/');
                std::cout << file.substr(n+1) << ':' << line << " in \"" << function << '"';
            }
            else
            {
                this->print_all_args(std::begin(arg_names), std::forward<Ts>(args)...);
            }

            std::cout << std::endl;
        }

    private:
        bool show_pointed_value_ = true;


        // Print pointer like classes
        template <typename T>
        auto print_value(T const& value) -> typename
            std::enable_if<
                detail::is_pointer_like<T>::value
            >::type
        {

            if (!this->show_pointed_value_)
            {
                std::cout << value;
            }
            else if (value == nullptr)
            {
                std::cout << "nullptr";
            }
            else
            {
                this->print_value(*value);
                std::cout << " at " << value;
            }
        }

        // Until C++20 std::unique_ptr has not an operator<<(ostream&) overload
        template <typename T>
        auto print_value(T const& value) -> typename
            std::enable_if<
                detail::is_unique_ptr<T>::value
            >::type
        {
            this->print_value(value.get());
        }

        // Print weak pointer classes
        template <typename T>
        auto print_value(T const& value) -> typename
            std::enable_if<
                detail::is_weak_pointer<T>::value
            >::type
        {
            this->print_value(value.lock());
        }

        // Print any class that overloads operator<<(std::ostream&, T)
        template <typename T>
        auto print_value(T const& value) -> typename
            std::enable_if<
                detail::has_insertion<T>::value
                && !detail::is_pointer_like<T>::value
                && !detail::is_unique_ptr<T>::value
                && !detail::is_weak_pointer<T>::value
            >::type
        {
            std::cout << value;
        }

        // Print std::optional<> classes
        template <typename T>
        auto print_value(T const& value) -> typename
            std::enable_if<
                detail::is_optional<T>::value
                && !detail::has_insertion<T>::value
            >::type
        {
            if (value.has_value())
            {
                this->print_value(*value);
            }
            else
            {
                std::cout << "nullopt";
            }
        }

        // Print all items of any iterable class
        template <typename T>
        auto print_value(T const& value) -> typename
            std::enable_if<
                detail::is_iterable<T>::value
                && !detail::has_insertion<T>::value
            >::type
        {
            auto it = std::begin(value);
            auto const e_it = std::end(value);
            std::cout << "[";
            if (it != e_it)
            {
                this->print_value(*it);
                ++it;
                for (; it != e_it; ++it)
                {
                    std::cout << ", ";
                    this->print_value(*it);
                }
            }
            std::cout << "]";
        }

        // Print the pair argument name and argument value
        template<typename T>
        auto print_arg(std::string const& name, T const& value) -> void
        {
            std::cout << name << ": ";
            this->print_value(value);
        }

        template<typename TArg, typename... Ts>
        auto print_all_args(std::vector<std::string>::const_iterator arg_name, TArg&& arg_value, Ts&&... args_tail) -> void
        {
            this->print_arg(*arg_name, std::forward<TArg>(arg_value));
            if (sizeof...(Ts) > 0)
            {
                std::cout << ", ";
                this->print_all_args(++arg_name, std::forward<Ts>(args_tail)...);
            }
        }

        auto print_all_args(std::vector<std::string>::const_iterator) -> void {}
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
        template<typename... Ts>
        print(std::string const& file, int line, std::string const& function, std::string const& arg_names, Ts&&... args)
        {
            auto split_names = std::vector<std::string> {};
            auto b_it = std::begin(arg_names);
            auto it = std::begin(arg_names);
            int par_count = 0;

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
